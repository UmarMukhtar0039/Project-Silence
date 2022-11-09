#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Library/MLSCharacterEnumLibrary.h"
#include "Library/MLSCharacterStructLibrary.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"

#include "MLSBaseCharacter.generated.h"

// forward declarations
class UALSDebugComponent;
class UAnimMontage;
class UMLSPlayerCameraBehavior;
enum class EVisibilityBasedAnimTickOption : uint8;

/*
 * Base character class
 */
UCLASS(BlueprintType)
class MLS_API AMLSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMLSBaseCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "MLS|Movement")
	FORCEINLINE class UMLSCharacterMovementComponent* GetMyMovementComponent() const
	{
		return MyCharacterMovementComponent;
	}

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	/** Character States */

	UFUNCTION(BlueprintCallable, Category = "MLS|Character States")
	void SetMovementState(ECharacterMovementState NewState, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "MLS|Character States")
	ECharacterMovementState GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintGetter, Category = "MLS|Character States")
	ECharacterMovementState GetPrevMovementState() const { return PrevMovementState; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Character States")
	void SetStance(ECharacterStance NewStance, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "MLS|Character States")
	ECharacterStance GetStance() const { return Stance; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Character States")
	void SetOverlayOverrideState(int32 NewState);

	UFUNCTION(BlueprintGetter, Category = "MLS|Character States")
	int32 GetOverlayOverrideState() const { return OverlayOverrideState; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Character States")
	void SetGait(ECharacterGait NewGait, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "MLS|Character States")
	ECharacterGait GetGait() const { return Gait; }

	UFUNCTION(BlueprintGetter, Category = "MLS|CharacterStates")
	ECharacterGait GetDesiredGait() const { return DesiredGait; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Character States")
	void SetRotationMode(ECharacterRotationMode NewRotationMode, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "MLS|Character States")
	ECharacterRotationMode GetRotationMode() const { return RotationMode; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Character States")
	void SetViewMode(ECharacterViewMode NewViewMode, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "MLS|Character States")
	ECharacterViewMode GetViewMode() const { return ViewMode; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Character States")
	void SetOverlayState(ECharacterOverlayState NewState, bool bForce = false);

	UFUNCTION(BlueprintGetter, Category = "MLS|Character States")
	ECharacterOverlayState GetOverlayState() const { return OverlayState; }

	/** Equip Items */

	UFUNCTION(BlueprintCallable, Category = "MLS|Items")
	virtual void EquipItem();

	/** Input */

	UFUNCTION(BlueprintGetter, Category = "MLS|Input")
	ECharacterStance GetDesiredStance() const { return DesiredStance; }

	UFUNCTION(BlueprintSetter, Category = "MLS|Input")
	void SetDesiredStance(ECharacterStance NewStance);

	UFUNCTION(BlueprintCallable, Category = "MLS|Character States")
	void SetDesiredGait(ECharacterGait NewGait);

	UFUNCTION(BlueprintGetter, Category = "MLS|Input")
	ECharacterRotationMode GetDesiredRotationMode() const { return DesiredRotationMode; }

	UFUNCTION(BlueprintSetter, Category = "MLS|Input")
	void SetDesiredRotationMode(ECharacterRotationMode NewRotMode);


	/** Rotation System */

	UFUNCTION(BlueprintCallable, Category = "MLS|Rotation System")
	void SetActorLocationAndTargetRotation(FVector NewLocation, FRotator NewRotation);

	/** Movement System */

	UFUNCTION(BlueprintGetter, Category = "MLS|Movement System")
	bool HasMovementInput() const { return bHasMovementInput; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Movement System")
	FMovementSettings GetTargetMovementSettings() const;

	UFUNCTION(BlueprintCallable, Category = "MLS|Movement System")
	ECharacterGait GetAllowedGait() const;

	UFUNCTION(BlueprintCallable, Category = "MLS|Movement System")
	ECharacterGait GetActualGait(ECharacterGait AllowedGait) const;

	/** BP implementable function that called when A Montage starts, e.g. during rolling */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Movement System")
	void PlayMontage(UAnimMontage* Montage, float PlayRate);
	//virtual void Replicated_PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate);
	virtual void PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate);

	/** Utility */

	UFUNCTION(BlueprintCallable, Category = "MLS|Utility")
	float GetAnimCurveValue(FName CurveName) const;

	UFUNCTION(BlueprintCallable, Category = "MLS|Utility")
	void SetVisibleMesh(USkeletalMesh* NewSkeletalMesh);

	/** Camera System */

	UFUNCTION(BlueprintGetter, Category = "MLS|Camera System")
	bool IsRightShoulder() const { return bRightShoulder; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera System")
	void SetRightShoulder(bool bNewRightShoulder);

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera System")
	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius);

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera System")
	virtual FTransform GetThirdPersonPivotTarget();

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera System")
	void GetCameraParameters(float& TPFOVOut, bool& bRightShoulderOut) const;

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera System")
	void SetCameraBehavior(UMLSPlayerCameraBehavior* CamBeh) { CameraBehavior = CamBeh; }

	/** Essential Information Getters/Setters */

	UFUNCTION(BlueprintGetter, Category = "MLS|Essential Information")
	FVector GetAcceleration() const { return Acceleration; }

	UFUNCTION(BlueprintGetter, Category = "MLS|Essential Information")
	bool IsMoving() const { return bIsMoving; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Essential Information")
	FVector GetMovementInput() const;

	UFUNCTION(BlueprintGetter, Category = "MLS|Essential Information")
	float GetMovementInputAmount() const { return MovementInputAmount; }

	UFUNCTION(BlueprintGetter, Category = "MLS|Essential Information")
	float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Essential Information")
	FRotator GetAimingRotation() const { return AimingRotation; }

	UFUNCTION(BlueprintGetter, Category = "MLS|Essential Information")
	float GetAimYawRate() const { return AimYawRate; }

	/** Input */

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void ForwardMovementAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void RightMovementAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void CameraUpAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void CameraRightAction(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void AimAction(bool bValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void CameraTapAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void CameraHeldAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void StanceAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void WalkAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void VelocityDirectionAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void LookingDirectionAction();

protected:
	/** Ragdoll System */

	void RagdollUpdate(float DeltaTime);

	void SetActorLocationDuringRagdoll(float DeltaTime);

	/** State Changes */

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void OnMovementStateChanged(ECharacterMovementState PreviousState);

	virtual void OnStanceChanged(ECharacterStance PreviousStance);

	virtual void OnRotationModeChanged(ECharacterRotationMode PreviousRotationMode);

	virtual void OnGaitChanged(ECharacterGait PreviousGait);

	virtual void OnViewModeChanged(ECharacterViewMode PreviousViewMode);

	virtual void OnOverlayStateChanged(ECharacterOverlayState PreviousState);

	virtual void OnVisibleMeshChanged(const USkeletalMesh* PreviousSkeletalMesh);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void SetEssentialValues(float DeltaTime);

	void UpdateCharacterMovement();

	void UpdateGroundedRotation(float DeltaTime);

	/** Utils */

	void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed, float DeltaTime);

	float CalculateGroundedRotationRate() const;

	void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed, float DeltaTime);

	void SetMovementModel();

	void ForceUpdateCharacterState();

	void GetHitFxInfoBySurfaceType(const EPhysicalSurface& SurfaceType, FMLSHitFX& OutHitImpactFX);

protected:
	/* Custom movement component*/
	UPROPERTY()
	TObjectPtr<UMLSCharacterMovementComponent> MyCharacterMovementComponent;

	/** Input */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Input")
	ECharacterRotationMode DesiredRotationMode = ECharacterRotationMode::VelocityDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Input")
	ECharacterRotationMode PreviousRotationMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Input")
	ECharacterViewMode PreviousViewMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Input")
	ECharacterGait DesiredGait = ECharacterGait::Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Input")
	ECharacterStance DesiredStance = ECharacterStance::Standing;

	UPROPERTY(EditDefaultsOnly, Category = "MLS|Input", BlueprintReadOnly)
	float LookUpDownRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "MLS|Input", BlueprintReadOnly)
	float LookLeftRightRate = 1.25f;

	UPROPERTY(EditDefaultsOnly, Category = "MLS|Input", BlueprintReadOnly)
	float RollDoubleTapTimeout = 0.3f;

	UPROPERTY(Category = "MLS|Input", BlueprintReadOnly)
	bool bBreakFall = false;

	UPROPERTY(Category = "MLS|Input", BlueprintReadOnly)
	bool bSprintHeld = false;

	/** Camera System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MLS|Camera System")
	float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MLS|Camera System")
	bool bRightShoulder = false;
	
	/** Movement System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MLS|Movement System")
	FDataTableRowHandle MovementModel;

	/** HitFxTable */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MLS|Movement System")
	UDataTable* HitImpactFXDT;

	/** Essential Information */

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	FVector Acceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	bool bIsMoving = false;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	bool bHasMovementInput = false;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	FRotator LastVelocityRotation;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	FRotator LastMovementInputRotation;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	float MovementInputAmount = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	float AimYawRate = 0.0f;

	/** Replicated Essential Information*/

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	float EasedMaxAcceleration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	FVector CurrentAcceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Essential Information")
	FRotator ControlRotation = FRotator::ZeroRotator;

	/** Skeletal Mesh Information*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|Skeletal Mesh")
	TObjectPtr<USkeletalMesh> VisibleMesh = nullptr;


	/** Test Look at target mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Component")
	TObjectPtr<UStaticMeshComponent> LookAtTargetMesh = nullptr;

	/** State Values */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|State Values")
	ECharacterOverlayState OverlayState = ECharacterOverlayState::Default;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|State Values")
	ECharacterMovementState MovementState = ECharacterMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|State Values")
	ECharacterMovementState PrevMovementState = ECharacterMovementState::None;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|State Values")
	ECharacterRotationMode RotationMode = ECharacterRotationMode::VelocityDirection;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|State Values")
	ECharacterGait Gait = ECharacterGait::Walking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MLS|State Values")
	ECharacterStance Stance = ECharacterStance::Standing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MLS|State Values")
	ECharacterViewMode ViewMode = ECharacterViewMode::ThirdPerson;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|State Values")
	int32 OverlayOverrideState = 0;

	/** Movement System */

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Movement System")
	FMovementStateSettings MovementData;

	/** Rotation System */

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Rotation System")
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Rotation System")
	float YawOffset = 0.0f;

	/** Cached Variables */

	FVector PreviousVelocity = FVector::ZeroVector;

	float PreviousAimYaw = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Camera")
	TObjectPtr<UMLSPlayerCameraBehavior> CameraBehavior;

	/** Last time the 'first' crouch/roll button is pressed */
	float LastStanceInputTime = 0.0f;

	/* Smooth out aiming by interping control rotation*/
	FRotator AimingRotation = FRotator::ZeroRotator;

private:
	UPROPERTY()
	TObjectPtr<UALSDebugComponent> ALSDebugComponent = nullptr;
};
