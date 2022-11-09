#include "Character/MLSPlayerCameraManager.h"

#include "Character/MLSBaseCharacter.h"
#include "Character/MLSPlayerController.h"
#include "Character/Animation/MLSPlayerCameraBehavior.h"
#include "Components/ALSDebugComponent.h"
#include "Character/MLSFixedCamera.h"
#include "Library/MLSCharacterEnumLibrary.h"
#include "Kismet/KismetMathLibrary.h"


const FName NAME_CameraBehavior(TEXT("CameraBehavior"));
const FName NAME_CameraOffset_X(TEXT("CameraOffset_X"));
const FName NAME_CameraOffset_Y(TEXT("CameraOffset_Y"));
const FName NAME_CameraOffset_Z(TEXT("CameraOffset_Z"));
const FName NAME_Override_Debug(TEXT("Override_Debug"));
const FName NAME_PivotLagSpeed_X(TEXT("PivotLagSpeed_X"));
const FName NAME_PivotLagSpeed_Y(TEXT("PivotLagSpeed_Y"));
const FName NAME_PivotLagSpeed_Z(TEXT("PivotLagSpeed_Z"));
const FName NAME_PivotOffset_X(TEXT("PivotOffset_X"));
const FName NAME_PivotOffset_Y(TEXT("PivotOffset_Y"));
const FName NAME_PivotOffset_Z(TEXT("PivotOffset_Z"));
const FName NAME_RotationLagSpeed(TEXT("RotationLagSpeed"));
const FName NAME_Weight_FixedCamera(TEXT("Weight_FixedCamera"));


AMLSPlayerCameraManager::AMLSPlayerCameraManager()
{
	CameraBehavior = CreateDefaultSubobject<USkeletalMeshComponent>(NAME_CameraBehavior);
	CameraBehavior->SetupAttachment(GetRootComponent());
	CameraBehavior->bHiddenInGame = true;
}

void AMLSPlayerCameraManager::OnPossess(AMLSBaseCharacter* NewCharacter)
{
	// Set "Controlled Pawn" when Player Controller Possesses new character. (called from Player Controller)
	check(NewCharacter);
	ControlledCharacter = NewCharacter;

	// Update references in the Camera Behavior AnimBP.
	CameraAnimInst = Cast<UMLSPlayerCameraBehavior>(CameraBehavior->GetAnimInstance());
	if (CameraAnimInst)
	{
		NewCharacter->SetCameraBehavior(CameraAnimInst);
		CameraAnimInst->MovementState = NewCharacter->GetMovementState();
		CameraAnimInst->bRightShoulder = NewCharacter->IsRightShoulder();
		CameraAnimInst->Gait = NewCharacter->GetGait();
		CameraAnimInst->SetRotationMode(NewCharacter->GetRotationMode());
		CameraAnimInst->Stance = NewCharacter->GetStance();
		CameraAnimInst->SetViewMode(NewCharacter->GetViewMode());
	}

	// Initial position
	FVector CameraLoc = ControlledCharacter->GetThirdPersonPivotTarget().GetLocation();

	// Override for FixedCamera view mode
	/*if (CameraAnimInst->bFixedCamera)
	{
		ActivateClosestFixedCamera(ControlledCharacter);

		if (CurrentActiveFixedCamera)
		{
			CameraLoc = CurrentActiveFixedCamera->GetActorLocation();
		}
	}*/

	SetActorLocation(CameraLoc);
	SmoothedPivotTarget.SetLocation(CameraLoc);

	ALSDebugComponent = ControlledCharacter->FindComponentByClass<UALSDebugComponent>();
}

float AMLSPlayerCameraManager::GetCameraBehaviorParam(FName CurveName) const
{
	UAnimInstance* Inst = CameraBehavior->GetAnimInstance();
	if (Inst)
	{
		return Inst->GetCurveValue(CurveName);
	}

	return 0.0f;
}

void AMLSPlayerCameraManager::AddFixedCamera(AMLSFixedCamera* FixedCameraObject)
{
	check(FixedCameraObject);
	FixedCameras.Add(FixedCameraObject);

	//// Test Code below
	//FVector CameraLoc = ControlledCharacter->GetThirdPersonPivotTarget().GetLocation();

	//// TODO: Remove this hard coding for fixed camera
	//// Override for FixedCamera view mode
	//if (CameraAnimInst->bFixedCamera /*&& CurrentActiveFixedCamera*/)
	//{
	//	ActivateClosestFixedCamera(ControlledCharacter);

	//	if (CurrentActiveFixedCamera)
	//	{
	//		CameraAnimInst->SetViewMode(ECharacterViewMode::FixedCamera);
	//		CameraLoc = CurrentActiveFixedCamera->GetActorLocation();
	//	}
	//}

	//SetActorLocation(CameraLoc);
	//SmoothedPivotTarget.SetLocation(CameraLoc);
}

bool AMLSPlayerCameraManager::GetClosestFixedCamera()
{
	if (FixedCameras.IsEmpty())
	{
		return false;
	}

	FVector CharacterLocation = ControlledCharacter->GetActorLocation();

	float CurrentCamDisToPlayer = DOUBLE_BIG_NUMBER;

	TArray<AActor*> ActorsToIgnore;
	
	for (auto& i : FixedCameras)
	{
		if (!i->bUseBoxToActivateOrDeactivate)
		{
			FHitResult HitResult;
			FVector CamLoc = i->GetActorLocation();

			// Check if target is actually in the vicinity of the camera
			// Trace from Camera to Player to see if there isn't any object in between.
			i->TraceToLookTarget(HitResult, ControlledCharacter);

			AActor* HitActor = HitResult.GetActor();

			if (HitActor == ControlledCharacter)
			{
				float CamDisToPlayer = (CamLoc - CharacterLocation).Length();
			
				if (CamDisToPlayer < CurrentCamDisToPlayer && IsValid(i))
				{
					CurrentActiveFixedCamera = i;
					CurrentCamDisToPlayer = CamDisToPlayer;
				}
			}
		}
		else
		{
			if (i->bIsActive)
			{
				CurrentActiveFixedCamera = i;
			}
		}
	}

	return (CurrentActiveFixedCamera) ?  true : false;
}

bool AMLSPlayerCameraManager::ActivateClosestFixedCamera(AActor* LookTarget)
{
	PreviousCam = CurrentActiveFixedCamera;
	
	CurrentActiveFixedCamera = nullptr;

	bool FoundClosestCameraInView = GetClosestFixedCamera();
	
	// If there are no fixed cameras in the vicinity then switch to third person
	if (!FoundClosestCameraInView)
	{
		return false;
	}
	
	// If there's is a previous fixed camera and CurrentActiveFixedCamera is same as previous.
	if (PreviousCam != nullptr && PreviousCam == CurrentActiveFixedCamera && !CurrentActiveFixedCamera->bUseBoxToActivateOrDeactivate)
	{
		return true;
	}

	// If there's already a camera activated then deactivate it.
	if (CurrentActiveFixedCamera)
	{
		// Don't deativate a camera which is using trigger boxes
		/*if (PreviousCam && !PreviousCam->bUseBoxToActivateOrDeactivate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Deactivated previous cam"));
			PreviousCam->Deactivate();
			PreviousCam = nullptr;
		}*/

		if (!CurrentActiveFixedCamera->bUseBoxToActivateOrDeactivate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Without trigger cam active"));

			CurrentActiveFixedCamera->Activate(LookTarget);
			return true;
		}
	}

	return false;
}

bool AMLSPlayerCameraManager::HaveFixedCamera() const
{
	for (const auto& i : FixedCameras)
	{
		return true;
	}

	return false;
}

void AMLSPlayerCameraManager::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	// Partially taken from base class

	if (OutVT.Target)
	{
		FVector OutLocation;
		FRotator OutRotation;
		float OutFOV;

		if (OutVT.Target->IsA<AMLSBaseCharacter>())
		{
			// If there's no fixed camera active then find closest one and activate it.
			if (HaveFixedCamera())
			{
				CameraAnimInst->bFixedCamera = ActivateClosestFixedCamera(OutVT.Target);
			}

			if (CustomCameraBehavior(DeltaTime, OutLocation, OutRotation, OutFOV))
			{
				OutVT.POV.Location = OutLocation;
				OutVT.POV.Rotation = OutRotation;
				OutVT.POV.FOV = OutFOV;
			}
			else
			{
				OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
			}
		}
		else
		{
			OutVT.Target->CalcCamera(DeltaTime, OutVT.POV);
		}
	}
}

FVector AMLSPlayerCameraManager::CalculateAxisIndependentLag(FVector CurrentLocation, FVector TargetLocation,
                                                             FRotator CameraRotation, FVector LagSpeeds,
                                                             float DeltaTime)
{
	CameraRotation.Roll = 0.0f;
	CameraRotation.Pitch = 0.0f;
	const FVector UnrotatedCurLoc = CameraRotation.UnrotateVector(CurrentLocation);
	const FVector UnrotatedTargetLoc = CameraRotation.UnrotateVector(TargetLocation);

	const FVector ResultVector(
		FMath::FInterpTo(UnrotatedCurLoc.X, UnrotatedTargetLoc.X, DeltaTime, LagSpeeds.X),
		FMath::FInterpTo(UnrotatedCurLoc.Y, UnrotatedTargetLoc.Y, DeltaTime, LagSpeeds.Y),
		FMath::FInterpTo(UnrotatedCurLoc.Z, UnrotatedTargetLoc.Z, DeltaTime, LagSpeeds.Z));

	return CameraRotation.RotateVector(ResultVector);
}

bool AMLSPlayerCameraManager::CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV)
{
	if (!ControlledCharacter)
	{
		return false;
	}

	// Step 1: Get Camera Parameters from CharacterBP via the Camera Interface
	float TPFOV = 90.0f;
	float FCFOV = 90.0f;
	bool bRightShoulder = false;

	ControlledCharacter->GetCameraParameters(TPFOV, bRightShoulder);
	
	// Set PivotTarget according the fixedcam/Character Transform
	FTransform PivotTarget = ControlledCharacter->GetThirdPersonPivotTarget();
	FRotator CameraRotationTarget = GetOwningPlayerController()->GetControlRotation();
	
	if (CurrentActiveFixedCamera)
	{
		ECharacterViewMode PreviousViewMode = ControlledCharacter->GetViewMode();

		if (PreviousViewMode == ECharacterViewMode::FixedCamera && PreviousCam && CurrentActiveFixedCamera != PreviousCam)
		{
			ControlledCharacter->SetViewMode(ECharacterViewMode::FixedCamera, true);

			// Don't take the roll rotation
			FRotator NewControlRotation(PreviousCam->GetActorRotation().Pitch, PreviousCam->GetActorRotation().Yaw, 0.0f);
			ControlledCharacter->GetController()->SetControlRotation(NewControlRotation);
		}

		ControlledCharacter->SetViewMode(ECharacterViewMode::FixedCamera);

		PivotTarget = CurrentActiveFixedCamera->GetActorTransform();
		CameraRotationTarget = CurrentActiveFixedCamera->GetActorRotation();

		CameraAnimInst->SetViewMode(ECharacterViewMode::FixedCamera);
	}

	else
	{
		ECharacterViewMode PreviousViewMode = ControlledCharacter->GetViewMode();

		if (PreviousViewMode == ECharacterViewMode::FixedCamera && PreviousCam)
		{
			// Don't take the roll rotation
			FRotator NewControlRotation(PreviousCam->GetActorRotation().Pitch, PreviousCam->GetActorRotation().Yaw, 0.0f);
			ControlledCharacter->GetController()->SetControlRotation(NewControlRotation);
		}

		ControlledCharacter->SetViewMode(ECharacterViewMode::ThirdPerson);
		CameraAnimInst->SetViewMode(ECharacterViewMode::ThirdPerson);
	}

	// Step 2: Calculate Target Camera Rotation. Use the Control Rotation and interpolate for smooth camera rotation.
	const FRotator& InterpResult = CameraRotationTarget;

	TargetCameraRotation = UKismetMathLibrary::RLerp(InterpResult, DebugViewRotation,
	                                                 GetCameraBehaviorParam(TEXT("Override_Debug")), true);

	// Step 3: Calculate the Smoothed Pivot Target (Orange Sphere).
	// Get the 3P Pivot Target (Green Sphere) and interpolate using axis independent lag for maximum control.
	/*const FVector LagSpd(GetCameraBehaviorParam(NAME_PivotLagSpeed_X),
	                     GetCameraBehaviorParam(NAME_PivotLagSpeed_Y),
	                     GetCameraBehaviorParam(NAME_PivotLagSpeed_Z));*/

	// TODO: LagSpeed made 0
	const FVector LagSpd = FVector::ZeroVector;

	const FVector& AxisIndpLag = CalculateAxisIndependentLag(SmoothedPivotTarget.GetLocation(),
	                                                         PivotTarget.GetLocation(), TargetCameraRotation, LagSpd,
	                                                         DeltaTime);
	
	SmoothedPivotTarget.SetRotation(PivotTarget.GetRotation());
	SmoothedPivotTarget.SetLocation(AxisIndpLag);
	SmoothedPivotTarget.SetScale3D(FVector::OneVector);

	// Step 4: Calculate Pivot Location (BlueSphere). Get the Smoothed
	// Pivot Target and apply local offsets for further camera control.
	PivotLocation =
		SmoothedPivotTarget.GetLocation() +
		UKismetMathLibrary::GetForwardVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(
			NAME_PivotOffset_X) +
		UKismetMathLibrary::GetRightVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(
			NAME_PivotOffset_Y) +
		UKismetMathLibrary::GetUpVector(SmoothedPivotTarget.Rotator()) * GetCameraBehaviorParam(
			NAME_PivotOffset_Z);

	// Step 5: Calculate Target Camera Location. Get the Pivot location and apply camera relative offsets.
	TargetCameraLocation = UKismetMathLibrary::VLerp(
		PivotLocation +
		UKismetMathLibrary::GetForwardVector(TargetCameraRotation) * GetCameraBehaviorParam(
			NAME_CameraOffset_X) +
		UKismetMathLibrary::GetRightVector(TargetCameraRotation) * GetCameraBehaviorParam(NAME_CameraOffset_Y)
		+
		UKismetMathLibrary::GetUpVector(TargetCameraRotation) * GetCameraBehaviorParam(NAME_CameraOffset_Z),
		PivotTarget.GetLocation() + DebugViewOffset,
		GetCameraBehaviorParam(NAME_Override_Debug));

	// Step 6: Trace for an object between the camera and character to apply a corrective offset.
	// Trace origins are set within the Character BP via the Camera Interface.
	// Functions like the normal spring arm, but can allow for different trace origins regardless of the pivot
	FVector TraceOrigin;
	float TraceRadius;
	ECollisionChannel TraceChannel = ControlledCharacter->GetThirdPersonTraceParams(TraceOrigin, TraceRadius);

	UWorld* World = GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(ControlledCharacter);

	FHitResult HitResult;
	const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(TraceRadius);
	const bool bHit = World->SweepSingleByChannel(HitResult, TraceOrigin, TargetCameraLocation, FQuat::Identity,
	                                              TraceChannel, SphereCollisionShape, Params);

	if (ALSDebugComponent && ALSDebugComponent->GetShowTraces())
	{
		UALSDebugComponent::DrawDebugSphereTraceSingle(World,
		                                               TraceOrigin,
		                                               TargetCameraLocation,
		                                               SphereCollisionShape,
		                                               EDrawDebugTrace::Type::ForOneFrame,
		                                               bHit,
		                                               HitResult,
		                                               FLinearColor::Red,
		                                               FLinearColor::Green,
		                                               5.0f);
	}

	// TODO: Do you wanna consider spring arm collision effect?
	/*if (HitResult.IsValidBlockingHit())
	{
		TargetCameraLocation += HitResult.Location - HitResult.TraceEnd;
	}*/

	const FTransform TargetTransform(TargetCameraRotation, TargetCameraLocation, FVector::OneVector);

	Location = TargetTransform.GetLocation();
	Rotation = TargetTransform.Rotator();
	FOV = FMath::Lerp(TPFOV, FCFOV, GetCameraBehaviorParam(NAME_Weight_FixedCamera));

	return true;
}
