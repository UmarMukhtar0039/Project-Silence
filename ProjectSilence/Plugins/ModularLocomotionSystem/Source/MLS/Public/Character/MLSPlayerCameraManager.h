#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "MLSPlayerCameraManager.generated.h"

// forward declarations
class UALSDebugComponent;
class AMLSBaseCharacter;
class AFixedCamera;
class UMLSPlayerCameraBehavior;

/**
 * Player camera manager class
 */
UCLASS(Blueprintable, BlueprintType)
class MLS_API AMLSPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AMLSPlayerCameraManager();

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera")
	void OnPossess(AMLSBaseCharacter* NewCharacter);

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera")
	float GetCameraBehaviorParam(FName CurveName) const;

	/** Implemented debug logic in BP */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "MLS|Camera")
	void DrawDebugTargets(FVector PivotTargetLocation);

	UFUNCTION()
	void AddFixedCamera(AFixedCamera* FixedCameraObject);

	bool GetClosestFixedCamera();
	
	AFixedCamera* GetCurrentFixedCamera() { return CurrentActiveFixedCamera; }

	bool ActivateClosestFixedCamera(AActor* LookTarget);

	bool HaveFixedCamera() const;

	void GetCameraParameters() const;

protected:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera")
	static FVector CalculateAxisIndependentLag(
		FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "MLS|Camera")
	bool CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MLS|Camera")
	TObjectPtr<AMLSBaseCharacter> ControlledCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MLS|Camera")
	TObjectPtr<USkeletalMeshComponent> CameraBehavior = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MLS|Camera")
	FVector RootLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MLS|Camera")
	FTransform SmoothedPivotTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MLS|Camera")
	FVector PivotLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MLS|Camera")
	FVector TargetCameraLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MLS|Camera")
	FRotator TargetCameraRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MLS|Camera")
	FRotator DebugViewRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MLS|Camera")
	FVector DebugViewOffset;

private:
	UPROPERTY()
	TObjectPtr<UALSDebugComponent> ALSDebugComponent = nullptr;

	UPROPERTY()
	TArray<AFixedCamera*> FixedCameras;

	UPROPERTY()
	AFixedCamera* CurrentActiveFixedCamera = nullptr;
	
	UPROPERTY()
	AFixedCamera* PreviousCam = nullptr;

	UPROPERTY()
	UMLSPlayerCameraBehavior* CameraAnimInst;

};
