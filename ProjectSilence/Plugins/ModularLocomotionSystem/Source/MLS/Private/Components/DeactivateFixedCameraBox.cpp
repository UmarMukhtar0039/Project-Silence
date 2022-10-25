#include "Components/DeactivateFixedCameraBox.h"
#include "Character/FixedCamera.h"


UDeactivateFixedCameraBox::UDeactivateFixedCameraBox()
{
}

void UDeactivateFixedCameraBox::BeginPlay()
{
	Super::BeginPlay();
}

void UDeactivateFixedCameraBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AFixedCamera* OwningFixedCamera = Cast<AFixedCamera>(GetOwner());

	if (OwningFixedCamera)
	{
		OwningFixedCamera->OnDeactivateBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}