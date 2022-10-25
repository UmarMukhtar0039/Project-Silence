#include "Components/CameraCollisionBox.h"

// Sets default values for this component's properties
UCameraCollisionBox::UCameraCollisionBox()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}


// Called when the game starts
void UCameraCollisionBox::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UCameraCollisionBox::OnBoxBeginOverlap);

	bHiddenInGame = false;
	// ...
	
}

void UCameraCollisionBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


// Called every frame
void UCameraCollisionBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCameraCollisionBox::PostLoad()
{
	Super::PostLoad();
}

