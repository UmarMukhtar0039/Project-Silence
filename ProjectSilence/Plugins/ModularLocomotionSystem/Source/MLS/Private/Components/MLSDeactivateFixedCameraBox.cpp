#include "Components/MLSDeactivateFixedCameraBox.h"
#include "Character/MLSFixedCamera.h"


UMLSDeactivateFixedCameraBox::UMLSDeactivateFixedCameraBox()
{
}

void UMLSDeactivateFixedCameraBox::BeginPlay()
{
	Super::BeginPlay();
}

void UMLSDeactivateFixedCameraBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AMLSFixedCamera* OwningFixedCamera = Cast<AMLSFixedCamera>(GetOwner());

	if (OwningFixedCamera)
	{
		OwningFixedCamera->OnDeactivateBoxBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}