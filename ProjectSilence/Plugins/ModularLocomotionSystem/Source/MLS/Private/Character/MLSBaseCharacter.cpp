#include "Character/MLSBaseCharacter.h"

#include "Character/Animation/MLSPlayerCameraBehavior.h"
#include "Library/MLSMathLibrary.h"
#include "Components/ALSDebugComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveFloat.h"
#include "Character/MLSCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Character/MLSPlayerCameraManager.h"


const FName NAME_FP_Camera(TEXT("FP_Camera"));
const FName NAME_Pelvis(TEXT("Pelvis"));
const FName NAME_RagdollPose(TEXT("RagdollPose"));
const FName NAME_RotationAmount(TEXT("RotationAmount"));
const FName NAME_YawOffset(TEXT("YawOffset"));
const FName NAME_pelvis(TEXT("pelvis"));
const FName NAME_root(TEXT("root"));
const FName NAME_spine_03(TEXT("spine_03"));


AMLSBaseCharacter::AMLSBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMLSCharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = 0;

	LookAtTargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LookAtMesh"));
}

void AMLSBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MyCharacterMovementComponent = Cast<UMLSCharacterMovementComponent>(Super::GetMovementComponent());
}

void AMLSBaseCharacter::PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate)
{
	// Roll: Simply play a Root Motion Montage.
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage, PlayRate);
	}
}

void AMLSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Make sure the mesh and animbp update after the CharacterBP to ensure it gets the most recent values.
	GetMesh()->AddTickPrerequisiteActor(this);

	// Set the Movement Model
	SetMovementModel();

	// Force update states to use the initial desired values.
	ForceUpdateCharacterState();

	if (Stance == ECharacterStance::Standing)
	{
		UnCrouch();
	}
	else if (Stance == ECharacterStance::Crouching)
	{
		Crouch();
	}

	// Set default rotation values.
	TargetRotation = GetActorRotation();
	LastVelocityRotation = TargetRotation;
	LastMovementInputRotation = TargetRotation;

	if (GetMesh()->GetAnimInstance() && GetLocalRole() == ROLE_SimulatedProxy)
	{
		GetMesh()->GetAnimInstance()->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);
	}

	MyCharacterMovementComponent->SetMovementSettings(GetTargetMovementSettings());

	ALSDebugComponent = FindComponentByClass<UALSDebugComponent>();
}

void AMLSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set required values
	SetEssentialValues(DeltaTime);

	if (MovementState == ECharacterMovementState::Grounded)
	{
		UpdateCharacterMovement();
		UpdateGroundedRotation(DeltaTime);
	}

	// Cache values
	PreviousVelocity = GetVelocity();
	PreviousAimYaw = AimingRotation.Yaw;
	
}

void AMLSBaseCharacter::SetMovementState(const ECharacterMovementState NewState, bool bForce)
{
	if (bForce || MovementState != NewState)
	{
		PrevMovementState = MovementState;
		MovementState = NewState;
		OnMovementStateChanged(PrevMovementState);
	}
}

void AMLSBaseCharacter::SetStance(const ECharacterStance NewStance, bool bForce)
{
	if (bForce || Stance != NewStance)
	{
		const ECharacterStance Prev = Stance;
		Stance = NewStance;
		OnStanceChanged(Prev);
	}
}

void AMLSBaseCharacter::SetOverlayOverrideState(int32 NewState)
{
	OverlayOverrideState = NewState;
}

void AMLSBaseCharacter::SetGait(const ECharacterGait NewGait, bool bForce)
{
	if (bForce || Gait != NewGait)
	{
		const ECharacterGait Prev = Gait;
		Gait = NewGait;
		OnGaitChanged(Prev);
	}
}


void AMLSBaseCharacter::SetDesiredStance(ECharacterStance NewStance)
{
	DesiredStance = NewStance;
}

void AMLSBaseCharacter::SetDesiredGait(const ECharacterGait NewGait)
{
	DesiredGait = NewGait;
}

void AMLSBaseCharacter::SetDesiredRotationMode(ECharacterRotationMode NewRotMode)
{
	DesiredRotationMode = NewRotMode;
}

void AMLSBaseCharacter::SetRotationMode(const ECharacterRotationMode NewRotationMode, bool bForce)
{
	if (bForce || RotationMode != NewRotationMode)
	{
		PreviousRotationMode = RotationMode;

		RotationMode = NewRotationMode;
		
		OnRotationModeChanged(PreviousRotationMode);
	}
}

void AMLSBaseCharacter::SetViewMode(const ECharacterViewMode NewViewMode, bool bForce)
{
	if (bForce || ViewMode != NewViewMode)
	{
		PreviousViewMode = ViewMode;
		const ECharacterViewMode Prev = ViewMode;
		ViewMode = NewViewMode;
		OnViewModeChanged(Prev);
	}
}

void AMLSBaseCharacter::SetOverlayState(const ECharacterOverlayState NewState, bool bForce)
{
	if (bForce || OverlayState != NewState)
	{
		const ECharacterOverlayState Prev = OverlayState;
		OverlayState = NewState;
		OnOverlayStateChanged(Prev);


	}
}

void AMLSBaseCharacter::EquipItem()
{
}

void AMLSBaseCharacter::SetActorLocationAndTargetRotation(FVector NewLocation, FRotator NewRotation)
{
	SetActorLocationAndRotation(NewLocation, NewRotation);
	TargetRotation = NewRotation;
}

void AMLSBaseCharacter::SetMovementModel()
{
	const FString ContextString = GetFullName();
	FMovementStateSettings* OutRow =
		MovementModel.DataTable->FindRow<FMovementStateSettings>(MovementModel.RowName, ContextString);
	check(OutRow);
	MovementData = *OutRow;
}

void AMLSBaseCharacter::GetHitFxInfoBySurfaceType(const EPhysicalSurface& SurfaceType, FMLSHitFX& OutHitImpactFX)
{
	TArray<FName> OutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(HitImpactFXDT, OutRowNames);
	
	UE_LOG(LogTemp, Warning, TEXT("Surface type is %d"), SurfaceType);
	FName Row = OutRowNames[SurfaceType - 1];

	const FString ContextString = GetFullName();

	const FMLSHitFX* OutRow = HitImpactFXDT->FindRow<FMLSHitFX>(Row, ContextString);
	check(OutRow);

	OutHitImpactFX = *OutRow;
}

void AMLSBaseCharacter::ForceUpdateCharacterState()
{
	SetGait(DesiredGait, true);
	SetStance(DesiredStance, true);
	SetRotationMode(DesiredRotationMode, true);
	SetViewMode(ViewMode, true);
	SetOverlayState(OverlayState, true);
	SetMovementState(MovementState, true);
}

FMovementSettings AMLSBaseCharacter::GetTargetMovementSettings() const
{
	if (RotationMode == ECharacterRotationMode::VelocityDirection/* || RotationMode == ECharacterRotationMode::CharacterDirection*/)
	{
		if (Stance == ECharacterStance::Standing)
		{
			return MovementData.VelocityDirection.Standing;
		}
		if (Stance == ECharacterStance::Crouching)
		{
			return MovementData.VelocityDirection.Crouching;
		}
	}
	else if (RotationMode == ECharacterRotationMode::LookingDirection || RotationMode == ECharacterRotationMode::CharacterDirection)
	{
		if (Stance == ECharacterStance::Standing)
		{
			return MovementData.LookingDirection.Standing;
		}
		if (Stance == ECharacterStance::Crouching)
		{
			return MovementData.LookingDirection.Crouching;
		}
	}
	else if (RotationMode == ECharacterRotationMode::Aiming)
	{
		if (Stance == ECharacterStance::Standing)
		{
			return MovementData.Aiming.Standing;
		}
		if (Stance == ECharacterStance::Crouching)
		{
			return MovementData.Aiming.Crouching;
		}
	}

	// Default to velocity dir standing
	return MovementData.VelocityDirection.Standing;
}

FVector AMLSBaseCharacter::GetMovementInput() const
{
	return CurrentAcceleration;
}

float AMLSBaseCharacter::GetAnimCurveValue(FName CurveName) const
{
	if (GetMesh()->GetAnimInstance())
	{
		return GetMesh()->GetAnimInstance()->GetCurveValue(CurveName);
	}

	return 0.0f;
}

void AMLSBaseCharacter::SetVisibleMesh(USkeletalMesh* NewVisibleMesh)
{
	if (VisibleMesh != NewVisibleMesh)
	{
		const USkeletalMesh* Prev = VisibleMesh;
		VisibleMesh = NewVisibleMesh;
		OnVisibleMeshChanged(Prev);
	}
}

void AMLSBaseCharacter::SetRightShoulder(bool bNewRightShoulder)
{
	bRightShoulder = bNewRightShoulder;
	if (CameraBehavior)
	{
		CameraBehavior->bRightShoulder = bRightShoulder;
	}
}

ECollisionChannel AMLSBaseCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	TraceOrigin = GetActorLocation();
	TraceRadius = 10.0f;
	return ECC_Visibility;
}

FTransform AMLSBaseCharacter::GetThirdPersonPivotTarget()
{
	return GetActorTransform();
}

void AMLSBaseCharacter::GetCameraParameters(float& TPFOVOut, bool& bRightShoulderOut) const
{
	TPFOVOut = ThirdPersonFOV;
	bRightShoulderOut = bRightShoulder;
}

void AMLSBaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	// Use the Character Movement Mode changes to set the Movement States to the right values. This allows you to have
	// a custom set of movement states but still use the functionality of the default character movement component.

	if (GetCharacterMovement()->MovementMode == MOVE_Walking ||
		GetCharacterMovement()->MovementMode == MOVE_NavWalking)
	{
		SetMovementState(ECharacterMovementState::Grounded);
	}
}

void AMLSBaseCharacter::OnMovementStateChanged(const ECharacterMovementState PreviousState)
{
	if (CameraBehavior)
	{
		CameraBehavior->MovementState = MovementState;
	}
}

void AMLSBaseCharacter::OnStanceChanged(const ECharacterStance PreviousStance)
{
	if (CameraBehavior)
	{
		CameraBehavior->Stance = Stance;
	}

	MyCharacterMovementComponent->SetMovementSettings(GetTargetMovementSettings());
}

void AMLSBaseCharacter::OnRotationModeChanged(ECharacterRotationMode PreviousRotation)
{
	if (RotationMode == ECharacterRotationMode::VelocityDirection)
	{
		// If the new rotation mode is Velocity Direction and the character is in First Person,
		// set the viewmode to Third Person.
		// TODO: Remove This?
		//SetViewMode(ECharacterViewMode::ThirdPerson);
	}

	if (CameraBehavior)
	{
		CameraBehavior->SetRotationMode(RotationMode);
	}

	MyCharacterMovementComponent->SetMovementSettings(GetTargetMovementSettings());
}

void AMLSBaseCharacter::OnGaitChanged(const ECharacterGait PreviousGait)
{
	if (CameraBehavior)
	{
		CameraBehavior->Gait = Gait;
	}
}

void AMLSBaseCharacter::OnViewModeChanged(const ECharacterViewMode PreviousView)
{
	if (ViewMode == ECharacterViewMode::ThirdPerson)
	{
		ControlRotation = GetControlRotation();

		if (RotationMode == ECharacterRotationMode::VelocityDirection || RotationMode == ECharacterRotationMode::LookingDirection)
		{
			// If Third Person, set the rotation mode back to the desired mode.
			SetRotationMode(DesiredRotationMode);
		}
	}
	else if (ViewMode == ECharacterViewMode::FixedCamera)
	{
		if (RotationMode == ECharacterRotationMode::VelocityDirection || RotationMode == ECharacterRotationMode::LookingDirection)
		{
			// If Third Person, set the rotation mode back to the desired mode.
			SetRotationMode(DesiredRotationMode);
		}
	}
	

	if (CameraBehavior)
	{
		CameraBehavior->ViewMode = ViewMode;
	}
}

void AMLSBaseCharacter::OnOverlayStateChanged(const ECharacterOverlayState PreviousState)
{
}

void AMLSBaseCharacter::OnVisibleMeshChanged(const USkeletalMesh* PrevVisibleMesh)
{
	// Update the Skeletal Mesh before we update materials and anim bp variables
	GetMesh()->SetSkeletalMesh(VisibleMesh);

	// Reset materials to their new mesh defaults
	if (GetMesh() != nullptr)
	{
		for (int32 MaterialIndex = 0; MaterialIndex < GetMesh()->GetNumMaterials(); ++MaterialIndex)
		{
			GetMesh()->SetMaterial(MaterialIndex, nullptr);
		}
	}

	// Force set variables. This ensures anim instance & character stay synchronized on mesh changes
	ForceUpdateCharacterState();
}

void AMLSBaseCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	SetStance(ECharacterStance::Crouching);
}

void AMLSBaseCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	SetStance(ECharacterStance::Standing);
}

void AMLSBaseCharacter::SetEssentialValues(float DeltaTime)
{
	CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

	FRotator PreviousControlRotation = ControlRotation;

	// Reset the previous view mode. This is used so that we can revert to using the camera rotation instead of controls rotation
	if ((float)CurrentAcceleration.Length() == 0.0f)
	{
		PreviousViewMode = ECharacterViewMode::Default;
	}

	if (ViewMode == ECharacterViewMode::FixedCamera && PreviousViewMode != ECharacterViewMode::Default)
	{
		ControlRotation = GetControlRotation();
	}
	else if(ViewMode == ECharacterViewMode::FixedCamera)
	{	
		AMLSPlayerCameraManager* CamManager = Cast<AMLSPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
		ControlRotation = CamManager->GetCameraRotation();
	}
	else
	{
		ControlRotation = GetControlRotation();
	}

	EasedMaxAcceleration = GetCharacterMovement()->GetMaxAcceleration();

	// Interp AimingRotation to current control rotation for smooth character rotation movement. Decrease InterpSpeed
	// for slower but smoother movement.
	if (RotationMode == ECharacterRotationMode::Aiming)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAtTargetMesh->GetComponentLocation());
		AimingRotation = FMath::RInterpTo(AimingRotation, LookAtRotation, DeltaTime, 30);
	}
	else
	{
		AimingRotation = FMath::RInterpTo(AimingRotation, ControlRotation, DeltaTime, 30);
	}

	ControlRotation = FMath::RInterpTo(PreviousControlRotation, ControlRotation, DeltaTime, 30);


	// These values represent how the capsule is moving as well as how it wants to move, and therefore are essential
	// for any data driven animation system. They are also used throughout the system for various functions,
	// so I found it is easiest to manage them all in one place.

	const FVector CurrentVel = GetVelocity();

	// Set the amount of Acceleration.
	const FVector NewAcceleration = (CurrentVel - PreviousVelocity) / DeltaTime;
	Acceleration = NewAcceleration.IsNearlyZero() || IsLocallyControlled() ? NewAcceleration : Acceleration / 2;

	// Determine if the character is moving by getting it's speed. The Speed equals the length of the horizontal (x y)
	// velocity, so it does not take vertical movement into account. If the character is moving, update the last
	// velocity rotation. This value is saved because it might be useful to know the last orientation of movement
	// even after the character has stopped.
	Speed = CurrentVel.Size2D();
	bIsMoving = Speed > 1.0f;
	if (bIsMoving)
	{
		LastVelocityRotation = CurrentVel.ToOrientationRotator();
	}

	// Determine if the character has movement input by getting its movement input amount.
	// The Movement Input Amount is equal to the current acceleration divided by the max acceleration so that
	// it has a range of 0-1, 1 being the maximum possible amount of input, and 0 being none.
	// If the character has movement input, update the Last Movement Input Rotation.
	MovementInputAmount = CurrentAcceleration.Size() / EasedMaxAcceleration;
	bHasMovementInput = MovementInputAmount > 0.0f;
	if (bHasMovementInput)
	{
		LastMovementInputRotation = CurrentAcceleration.ToOrientationRotator();
	}

	// Set the Aim Yaw rate by comparing the current and previous Aim Yaw value, divided by Delta Seconds.
	// This represents the speed the camera is rotating left to right.
	AimYawRate = FMath::Abs((AimingRotation.Yaw - PreviousAimYaw) / DeltaTime);
}

void AMLSBaseCharacter::UpdateCharacterMovement()
{
	// Set the Allowed Gait
	const ECharacterGait AllowedGait = GetAllowedGait();

	// Determine the Actual Gait. If it is different from the current Gait, Set the new Gait Event.
	const ECharacterGait ActualGait = GetActualGait(AllowedGait);

	if (ActualGait != Gait)
	{
		SetGait(ActualGait);
	}

	// Update the Character Max Walk Speed to the configured speeds based on the currently Allowed Gait.
	MyCharacterMovementComponent->SetAllowedGait(AllowedGait);
}

void AMLSBaseCharacter::UpdateGroundedRotation(float DeltaTime)
{
	const bool bCanUpdateMovingRot = ((bIsMoving && bHasMovementInput) || Speed > 150.0f) && !HasAnyRootMotion();
	if (bCanUpdateMovingRot)
	{
		const float GroundedRotationRate = CalculateGroundedRotationRate();
		if (RotationMode == ECharacterRotationMode::VelocityDirection)
		{
			// Velocity Direction Rotation
			SmoothCharacterRotation({0.0f, LastVelocityRotation.Yaw, 0.0f}, 800.0f, GroundedRotationRate,
				                    DeltaTime);
		}
		else if (RotationMode == ECharacterRotationMode::LookingDirection || RotationMode == ECharacterRotationMode::CharacterDirection)
		{
			// Looking Direction Rotation
			float YawValue = 0.0f;
			if (Gait == ECharacterGait::Walking || Gait == ECharacterGait::Running)
			{
				// Walking or Running..
				const float YawOffsetCurveVal = GetAnimCurveValue(NAME_YawOffset);
				YawValue = AimingRotation.Yaw + YawOffsetCurveVal;
			}
			SmoothCharacterRotation({0.0f, YawValue, 0.0f}, 500.0f, GroundedRotationRate, DeltaTime);
		}
		else if (RotationMode == ECharacterRotationMode::Aiming)
		{
			const float ControlYaw = AimingRotation.Yaw;
			SmoothCharacterRotation({0.0f, ControlYaw, 0.0f}, 1000.0f, 20.0f, DeltaTime);
		}
	}
	else
	{
		// Not Moving

		if (RotationMode == ECharacterRotationMode::Aiming)
		{
			LimitRotation(-100.0f, 100.0f, 20.0f, DeltaTime);
		}

		// Apply the RotationAmount curve from Turn In Place Animations.
		// The Rotation Amount curve defines how much rotation should be applied each frame,
		// and is calculated for animations that are animated at 30fps.

		const float RotAmountCurve = GetAnimCurveValue(NAME_RotationAmount);

		if (FMath::Abs(RotAmountCurve) > 0.001f)
		{
			if (GetLocalRole() == ROLE_AutonomousProxy)
			{
				TargetRotation.Yaw = UKismetMathLibrary::NormalizeAxis(
					TargetRotation.Yaw + (RotAmountCurve * (DeltaTime / (1.0f / 30.0f))));
				SetActorRotation(TargetRotation);
			}
			else
			{
				AddActorWorldRotation({0, RotAmountCurve * (DeltaTime / (1.0f / 30.0f)), 0});
			}
			TargetRotation = GetActorRotation();
		}
	}

	// Other actions are ignored...
}

ECharacterGait AMLSBaseCharacter::GetAllowedGait() const
{
	// Calculate the Allowed Gait. This represents the maximum Gait the character is currently allowed to be in,
	// and can be determined by the desired gait, the rotation mode, the stance, etc. For example,
	// if you wanted to force the character into a walking state while indoors, this could be done here.

	if (Stance == ECharacterStance::Standing)
	{
		if (RotationMode != ECharacterRotationMode::Aiming)
		{
			return DesiredGait;
		}
	}

	return DesiredGait;
}

ECharacterGait AMLSBaseCharacter::GetActualGait(ECharacterGait AllowedGait) const
{
	// Get the Actual Gait. This is calculated by the actual movement of the character,  and so it can be different
	// from the desired gait or allowed gait. For instance, if the Allowed Gait becomes walking,
	// the Actual gait will still be running until the character decelerates to the walking speed.

	const float LocWalkSpeed = MyCharacterMovementComponent->CurrentMovementSettings.WalkSpeed;
	const float LocRunSpeed = MyCharacterMovementComponent->CurrentMovementSettings.RunSpeed;

	if (Speed >= LocWalkSpeed + 10.0f)
	{
		return ECharacterGait::Running;
	}

	return ECharacterGait::Walking;
}

void AMLSBaseCharacter::SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed,
                                                float DeltaTime)
{
	// Interpolate the Target Rotation for extra smooth rotation behavior
	TargetRotation =
		FMath::RInterpConstantTo(TargetRotation, Target, DeltaTime, TargetInterpSpeed);
	SetActorRotation(
		FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, ActorInterpSpeed));
}

float AMLSBaseCharacter::CalculateGroundedRotationRate() const
{
	// Calculate the rotation rate by using the current Rotation Rate Curve in the Movement Settings.
	// Using the curve in conjunction with the mapped speed gives you a high level of control over the rotation
	// rates for each speed. Increase the speed if the camera is rotating quickly for more responsive rotation.

	const float MappedSpeedVal = MyCharacterMovementComponent->GetMappedSpeed();
	const float CurveVal =
		MyCharacterMovementComponent->CurrentMovementSettings.RotationRateCurve->GetFloatValue(MappedSpeedVal);
	const float ClampedAimYawRate = FMath::GetMappedRangeValueClamped<float, float>({0.0f, 300.0f}, {1.0f, 3.0f}, AimYawRate);
	return CurveVal * ClampedAimYawRate;
}

void AMLSBaseCharacter::LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime)
{
	// Prevent the character from rotating past a certain angle.
	FRotator Delta = AimingRotation - GetActorRotation();
	Delta.Normalize();
	const float RangeVal = Delta.Yaw;

	if (RangeVal < AimYawMin || RangeVal > AimYawMax)
	{
		const float ControlRotYaw = AimingRotation.Yaw;
		const float TargetYaw = ControlRotYaw + (RangeVal > 0.0f ? AimYawMin : AimYawMax);
		SmoothCharacterRotation({0.0f, TargetYaw, 0.0f}, 0.0f, InterpSpeed, DeltaTime);
	}
}

void AMLSBaseCharacter::ForwardMovementAction_Implementation(float Value)
{
	if (MovementState == ECharacterMovementState::Grounded)
	{
		// Default camera relative movement behavior
		const FRotator DirRotator(0.0f, ControlRotation.Yaw, 0.0f);
		AddMovementInput(UKismetMathLibrary::GetForwardVector(DirRotator), Value);
	}
}

void AMLSBaseCharacter::RightMovementAction_Implementation(float Value)
{
	if (MovementState == ECharacterMovementState::Grounded)
	{
		// Default camera relative movement behavior
		const FRotator DirRotator(0.0f, ControlRotation.Yaw, 0.0f);
		AddMovementInput(UKismetMathLibrary::GetRightVector(DirRotator), Value);
	}
}

void AMLSBaseCharacter::CameraUpAction_Implementation(float Value)
{
	AddControllerPitchInput(LookUpDownRate * Value);
}

void AMLSBaseCharacter::CameraRightAction_Implementation(float Value)
{
	AddControllerYawInput(LookLeftRightRate * Value);
}

void AMLSBaseCharacter::AimAction_Implementation(bool bValue)
{
	if (bValue)
	{
		// AimAction: Hold "AimAction" to enter the aiming mode, release to revert back the desired rotation mode.
		SetRotationMode(ECharacterRotationMode::Aiming);
	}
	else
	{
		if (ViewMode == ECharacterViewMode::ThirdPerson || ViewMode == ECharacterViewMode::FixedCamera)
		{
			SetRotationMode(DesiredRotationMode);
		}
	}
}

void AMLSBaseCharacter::CameraTapAction_Implementation()
{
	if (ViewMode == ECharacterViewMode::Default)
	{
		// Don't swap shoulders on default mode
		return;
	}

	// Switch shoulders
	SetRightShoulder(!bRightShoulder);
}

void AMLSBaseCharacter::CameraHeldAction_Implementation()
{
	// Switch camera mode
	//SetViewMode(ECharacterViewMode::FixedCamera);
	SetViewMode(ECharacterViewMode::ThirdPerson);
}

void AMLSBaseCharacter::StanceAction_Implementation()
{
	// Stance Action: Press "Stance Action" to toggle Standing / Crouching, double tap to Roll.

	UWorld* World = GetWorld();
	check(World);

	const float PrevStanceInputTime = LastStanceInputTime;
	LastStanceInputTime = World->GetTimeSeconds();

	if (LastStanceInputTime - PrevStanceInputTime <= RollDoubleTapTimeout)
	{
		if (Stance == ECharacterStance::Standing)
		{
			SetDesiredStance(ECharacterStance::Crouching);
		}
		else if (Stance == ECharacterStance::Crouching)
		{
			SetDesiredStance(ECharacterStance::Standing);
		}
		return;
	}

	if (MovementState == ECharacterMovementState::Grounded)
	{
		if (Stance == ECharacterStance::Standing)
		{
			SetDesiredStance(ECharacterStance::Crouching);
			Crouch();
		}
		else if (Stance == ECharacterStance::Crouching)
		{
			SetDesiredStance(ECharacterStance::Standing);
			UnCrouch();
		}
	}
}

void AMLSBaseCharacter::WalkAction_Implementation()
{
	if (DesiredGait == ECharacterGait::Walking)
	{
		SetDesiredGait(ECharacterGait::Running);
	}
	else if (DesiredGait == ECharacterGait::Running)
	{
		SetDesiredGait(ECharacterGait::Walking);
	}
}

void AMLSBaseCharacter::VelocityDirectionAction_Implementation()
{
	// Select Rotation Mode: Switch the desired (default) rotation mode to Velocity or Looking Direction.
	// This will be the mode the character reverts back to when un-aiming
	SetDesiredRotationMode(ECharacterRotationMode::VelocityDirection);
	SetRotationMode(ECharacterRotationMode::VelocityDirection);
}

void AMLSBaseCharacter::LookingDirectionAction_Implementation()
{
	SetDesiredRotationMode(ECharacterRotationMode::LookingDirection);
	SetRotationMode(ECharacterRotationMode::LookingDirection);
}