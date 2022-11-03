#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MLSFixedCamera.generated.h"

class UBillboardComponent;

UENUM(BlueprintType)
enum class EMLSFixedCameraState : uint8
{
	InActive		UMETA(DisplayName = "InActive"),
	Active			UMETA(DisplayName = "Active")
};


UCLASS()
class MLS_API AMLSFixedCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMLSFixedCamera();
	
	virtual void PostActorCreated() override;

	UPROPERTY(VisibleAnywhere, Category = "MLS|FixedCamera")
	UBillboardComponent* CameraLocationDebug;

	UFUNCTION()
		void OnActivateBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDeactivateBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetState(EMLSFixedCameraState NewState);

	void Activate(AActor* NewLookTarget);

	bool TraceToLookTarget(FHitResult& HitResultOut, const AActor* LookTargetIn) const;

	void Deactivate();
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MLS|FixedCamera")
	bool bIsActive = false;

	UPROPERTY(VisibleAnywhere, Category = "MLS|FixedCamera")
	EMLSFixedCameraState FixedCameraState;
	
	UPROPERTY(VisibleAnywhere, Category = "MLS|FixedCamera")
	bool bIsStaticCamera;

	UPROPERTY()
	AActor* LookTarget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MLS|FixedCamera")
	float FCFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|FixedCamera")
	bool bUseBoxToActivateOrDeactivate;

	// TODO: This is absolutely shitty way to find out that all fixed cameras are inactive.
	static bool bAllCameraInactive;
};
