#pragma once

#include "CoreMinimal.h"
#include "Character/MLSBaseCharacter.h"
#include "MLSCharacter.generated.h"

class UMLSSoundControllerComponent;
class AMLSItem;

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

	UFUNCTION(BlueprintCallable, Category = "MLS|Pickup System")
	void PickupItem(AMLSItem* const Item);

	UFUNCTION(BlueprintCallable, Category = "MLS|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "MLS|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	// NOTE: Only the main character will have weapon.
	/** Weapon System */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MLS|Weapon System|Pistol")
	UDataTable* PistolAssetDT;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MLS|Weapon System|Pistol")
	ECharacterCombatState CombatState = ECharacterCombatState::NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MLS|Weapon System|Pistol")
	TEnumAsByte<EPistolModel> PistolModelIndex = EPistolModel::Pistol_M9;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MLS|Weapon System|Pistol")
	FORCEINLINE FPistolAssetSetting GetCurrentPistolAsset() { return CurrentPistolAsset; }

	// Always call this function when a new weapon is picked up
	UFUNCTION(BlueprintCallable, Category = "MLS|Weapon System|Pistol")
	void UpdatePistolAsset(EPistolModel NewPistolModel);

	UFUNCTION()
	void EquipGunEventUpdateVisibility();

	UFUNCTION()
	void HolsterGunEventUpdateVisibility();

	// Sound
	UFUNCTION(BlueprintCallable, Category = "MLS|Sounds")
	void PlayShootSoundByWeaponType();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Input")
	void LMBDown();

	UFUNCTION(BlueprintCallable, Category = "MLS|Weapon")
	bool CanFireGun();

	UFUNCTION(BlueprintCallable, Category = "MLS|Weapon")
	void FireGun();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Weapon")
	void OnBulletHitImpact(const FHitResult& HitResult);

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(ECharacterOverlayState PreviousState) override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "MLS|HeldObject")
	void UpdateHeldObjectAnimations();

	void InitAmmoMap();

	UFUNCTION(BlueprintCallable, Category = "MLS|HeldObject")
	void AddAmmo(EAmmoType AmmoType, int32 Count);

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

	/** Items Pickup */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "MLS|Pickup System")
	AMLSItem* ItemToPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "MLS|Inventory System")
	TArray< AMLSItem* > Inventory;

	// TODO: Attach the blueprint version of this?
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "MLS|SoundSystem")
	UMLSSoundControllerComponent* SoundController;

	UPROPERTY(VisibleAnywhere, Category = "MLS|Weapon System")
	FPistolAssetSetting	CurrentPistolAsset;

	UPROPERTY(VisibleAnywhere, Category = "MLS|HitImpactDetails")
	FMLSHitFX HitImpactFX;
	
	/** Shooting related */
	UPROPERTY()
	FTimerHandle BulletShootDelayTimer;

	UPROPERTY(VisibleAnywhere, Category = "MLS|Weapon System")
	bool bLockShooting = false;

	/** Ammo related */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess), Category = "MLS|Weapon System")
	TMap<EAmmoType, int32> AmmoMap; // Contains total no. of all ammo types player has.

	// TODO: Might not need it.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess), Category = "MLS|Weapon System")
	int32 TotalPistolAmmo = 9;

	/** misc */
	bool bNeedsColorReset = false;
};
