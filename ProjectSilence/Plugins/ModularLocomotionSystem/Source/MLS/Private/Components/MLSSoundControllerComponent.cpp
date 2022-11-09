#include "Components/MLSSoundControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UMLSSoundControllerComponent::UMLSSoundControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMLSSoundControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>(GetOwner());

	if (OwningCharacter == nullptr)
	{
		SetComponentTickEnabled(false);
	}
	// ...
	
}


// Called every frame
void UMLSSoundControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMLSSoundControllerComponent::PlayBulletImpactSound(const FTransform& SpawnTransform, float Volume, const FMLSHitFX& HitImpactFX)
{
	if (UKismetSystemLibrary::IsValidSoftObjectReference(HitImpactFX.Sound) && 
		HitImpactFX.Sound.LoadSynchronous())
	{
		UAudioComponent* AudioComponent = 
			UGameplayStatics::SpawnSoundAtLocation(GetOwner()->GetWorld(),
												   HitImpactFX.Sound.Get(), SpawnTransform.GetLocation(),
												   SpawnTransform.GetRotation().Rotator(), Volume);
		if (AudioComponent)
		{
			AudioComponent->SetIntParameter(FName("SurfaceType"), HitImpactFX.SurfaceType);
		}
	}
}

void UMLSSoundControllerComponent::PlayWeaponEquipSound(const FTransform& SpawnTransform, float Volume)
{
	if (WeaponEquipSound)
	{
		UAudioComponent* AudioComponent =
			UGameplayStatics::SpawnSoundAtLocation(GetOwner()->GetWorld(),
												   WeaponEquipSound, SpawnTransform.GetLocation(),
												   SpawnTransform.GetRotation().Rotator(), Volume);

		if (AudioComponent)
		{
			AudioComponent->SetIntParameter(FName("WeaponType"), 1);
		}
	}
}

void UMLSSoundControllerComponent::PlayWeaponHolsterSound(const FTransform& SpawnTransform, float Volume)
{
	if (WeaponHolsterSound)
	{
		UAudioComponent* AudioComponent =
			UGameplayStatics::SpawnSoundAtLocation(GetOwner()->GetWorld(),
				WeaponHolsterSound, SpawnTransform.GetLocation(),
				SpawnTransform.GetRotation().Rotator(), Volume);

		if (AudioComponent)
		{
			AudioComponent->SetIntParameter(FName("WeaponType"), 1);
		}
	}
}
