#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/MLSCharacterStructLibrary.h"
#include "MLSSoundControllerComponent.generated.h"

class USoundCue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MLS_API UMLSSoundControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMLSSoundControllerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	ACharacter* OwningCharacter;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void PlayBulletImpactSound(const FTransform& SpawnTransform, float Volume, const FMLSHitFX& HitImpactFX);

	UFUNCTION()
	void PlayWeaponEquipSound(const FTransform& SpawnTransform, float Volume);
	
	UFUNCTION()
	void PlayWeaponHolsterSound(const FTransform& SpawnTransform, float Volume);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ItemsSounds")
	USoundCue* WeaponEquipSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ItemsSounds")
	USoundCue* WeaponHolsterSound;
};
