#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoundControllerComponent.generated.h"

class USoundCue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MLS_API USoundControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoundControllerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	ACharacter* OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ImpactSounds")
	USoundCue* BulletImpactSound;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void PlayBulletImpactSound(FTransform SpawnTransform, float Volume, EPhysicalSurface Surface);
};
