#include "Actors/MLSWeapon.h"
#include "Components/BoxComponent.h"

AMLSWeapon::AMLSWeapon()
{
	SetRootComponent(ItemSkeletalMesh);

	ItemCollisionBox->SetupAttachment(RootComponent);
	
	ItemStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemStaticMesh->SetupAttachment(RootComponent);
}

void AMLSWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(ItemSkeletalMesh->SkeletalMesh))
	{
		ItemSkeletalMesh->SetSimulatePhysics(true);
		ItemSkeletalMesh->SetPhysicsAsset(PistolAsset.PhysicsAsset);
	}
}

void AMLSWeapon::InitPistolType()
{
	const FString ContextString = GetFullName();

	FPistolAssetSetting* OutRow =
		PistolDTHandle.DataTable->FindRow<FPistolAssetSetting>(PistolDTHandle.RowName, ContextString);
	
	if (OutRow)
	{
		PistolAsset = *OutRow;
		
		WeaponType = EWeaponType::Pistol2H;

		if (IsValid(PistolAsset.SkeletaMesh))
		{
			ItemSkeletalMesh->SkeletalMesh = PistolAsset.SkeletaMesh;
		}
	}
}
