#include "Components/ActivateFixedCameraBox.h"
#include "Character/FixedCamera.h"

UActivateFixedCameraBox::UActivateFixedCameraBox()
{
}

void UActivateFixedCameraBox::BeginPlay()
{
	Super::BeginPlay();
}

void UActivateFixedCameraBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AFixedCamera* OwningFixedCamera = Cast<AFixedCamera>(GetOwner());

	if (OwningFixedCamera)
	{
		OwningFixedCamera->OnActivateBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}