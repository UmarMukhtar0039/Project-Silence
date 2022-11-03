#include "Components/MLSActivateFixedCameraBox.h"
#include "Character/MLSFixedCamera.h"

UMLSActivateFixedCameraBox::UMLSActivateFixedCameraBox()
{
}

void UMLSActivateFixedCameraBox::BeginPlay()
{
	Super::BeginPlay();
}

void UMLSActivateFixedCameraBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AMLSFixedCamera* OwningFixedCamera = Cast<AMLSFixedCamera>(GetOwner());

	if (OwningFixedCamera)
	{
		OwningFixedCamera->OnActivateBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}