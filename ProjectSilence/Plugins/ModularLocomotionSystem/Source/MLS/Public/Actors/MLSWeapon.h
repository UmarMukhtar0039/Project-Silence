#pragma once

#include "CoreMinimal.h"
#include "Actors/MLSItem.h"
#include "Library/MLSCharacterEnumLibrary.h"
#include "Library/MLSCharacterStructLibrary.h"
#include "Engine/DataTable.h"

#include "MLSWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MLS_API AMLSWeapon : public AMLSItem
{
	GENERATED_BODY()

public:
	AMLSWeapon();

public:
	void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "MLS|Weapon System")
	void InitPistolType();

public:
	EWeaponType WeaponType = EWeaponType::NONE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MLS|Weapon System")
	FPistolAssetSetting PistolAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MLS|Weapon System")
	FDataTableRowHandle PistolDTHandle;
};
