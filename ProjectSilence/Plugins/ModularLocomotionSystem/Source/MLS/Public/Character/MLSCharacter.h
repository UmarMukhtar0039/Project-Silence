#pragma once

#include "CoreMinimal.h"
#include "Character/MLSBaseCharacter.h"
#include "MLSCharacter.generated.h"

/**
 * Specialized character class, with additional features like held object etc.
 */
UCLASS(Blueprintable, BlueprintType)
class MLS_API AMLSCharacter : public AMLSBaseCharacter
{
	GENERATED_BODY()

public:
	AMLSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Implemented on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "MLS|HeldObject")
	void UpdateHeldObject();

	void EquipItem() override;

	UFUNCTION()
	void EquipGunEventUpdateVisibility();

	UFUNCTION()
	void HolsterGunEventUpdateVisibility();

	UFUNCTION(BlueprintCallable, Category = "MLS|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "MLS|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(ECharacterOverlayState PreviousState) override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "MLS|HeldObject")
	void UpdateHeldObjectAnimations();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Component")
	TObjectPtr<USceneComponent> HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Component")
	TObjectPtr<USkeletalMeshComponent> HeldSkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Component")
	TObjectPtr<UStaticMeshComponent> HeldStaticMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Component|Weapons")
	TObjectPtr<UStaticMeshComponent> PistolSkeletalRoot = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Component|Weapons")
	TObjectPtr<UStaticMeshComponent> PistolAttachmentMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Component|Weapons")
	TObjectPtr<USkeletalMeshComponent> PistolSkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MLS|Component")
	TObjectPtr<USkeletalMeshComponent> SkeletaMeshObject = nullptr;

private:
	bool bNeedsColorReset = false;
};
