#include "Actors/MLSItem.h"
#include "Components/BoxComponent.h"

// Sets default values
AMLSItem::AMLSItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCollisionBox"));
	ItemCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemCollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ItemCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	ItemCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ItemStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ItemStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	ItemStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);

	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));
	ItemSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ItemSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ItemSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	ItemSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);

	SetRootComponent(ItemStaticMesh);
	ItemCollisionBox->SetupAttachment(RootComponent);
	ItemSkeletalMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMLSItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMLSItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
