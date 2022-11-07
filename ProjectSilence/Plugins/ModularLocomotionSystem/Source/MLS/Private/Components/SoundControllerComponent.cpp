#include "Components/SoundControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
USoundControllerComponent::USoundControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USoundControllerComponent::BeginPlay()
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
void USoundControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USoundControllerComponent::PlayBulletImpactSound(FTransform SpawnTransform, float Volume, EPhysicalSurface Surface)
{
	if (BulletImpactSound)
	{
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetOwner()->GetWorld(), BulletImpactSound, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), Volume);
		if (AudioComponent)
		{
			AudioComponent->SetIntParameter(FName("SurfaceType"), Surface);
		}
	}
}
