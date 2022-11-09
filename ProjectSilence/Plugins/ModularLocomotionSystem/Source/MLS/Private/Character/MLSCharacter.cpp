#include "Character/MLSCharacter.h"

#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "AI/ALSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Interfaces/AnimationInterface.h"
#include "Components/MLSSoundControllerComponent.h"
#include "Particles/ParticleSystem.h"

AMLSCharacter::AMLSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
	HeldObjectRoot->SetupAttachment(GetMesh());

	HeldSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	HeldSkeletalMesh->SetupAttachment(HeldObjectRoot);

	HeldStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	HeldStaticMesh->SetupAttachment(HeldObjectRoot);

	PistolSkeletalRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolSkeletalRoot"));
	PistolSkeletalRoot->SetupAttachment(RootComponent);

	PistolAttachmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PistolAttachmentMesh"));
	PistolAttachmentMesh->SetupAttachment(RootComponent);
	
	PistolSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PistolSkeletalMesh"));
	PistolSkeletalMesh->SetupAttachment(PistolSkeletalRoot);
	
	SoundController = CreateDefaultSubobject<UMLSSoundControllerComponent>(TEXT("SoundController"));

	AIControllerClass = AALSAIController::StaticClass();
}

void AMLSCharacter::BeginPlay()
{
	Super::BeginPlay();

	SkeletaMeshObject = HeldSkeletalMesh;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	PistolAttachmentMesh->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Pistol_Attachment")));

	UpdateHeldObject();

	// Setup a default pistol asset
	UpdatePistolAsset(PistolModelIndex);

}

void AMLSCharacter::EquipItem()
{
	Super::EquipItem();

}

void AMLSCharacter::UpdatePistolAsset(EPistolModel NewPistolModel)
{
	TArray<FName> OutRowNames;
	UDataTableFunctionLibrary::GetDataTableRowNames(PistolAssetDT, OutRowNames);

	FName Row = OutRowNames[NewPistolModel];

	const FString ContextString = GetFullName();

	FPistolAssetSetting* OutRow = PistolAssetDT->FindRow<FPistolAssetSetting>(Row, ContextString);
	check(OutRow);

	CurrentPistolAsset = *OutRow;
}

bool AMLSCharacter::CanFireGun()
{
	// TODO: need to check if the player aiming anim is complete or not.
	// 
	// Only shoot if character is aiming
	if (OverlayState == ECharacterOverlayState::Pistol && RotationMode == ECharacterRotationMode::Aiming)
	{
		return true;
	}

	return false;
}

void AMLSCharacter::LMBDown_Implementation()
{
	if (CanFireGun())
	{
		FireGun();
	}

}

void AMLSCharacter::FireGun()
{
	// TODO: this should be called in a play fx function
	PlayShootSoundByWeaponType();
	
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

	// TODO: Move this to correct function
	// Check if inteface has been implemented
	if (AnimInst->GetClass()->ImplementsInterface(UAnimationInterface::StaticClass()))
	{
		// TODO: This var should be in Weapon class?
		EWeaponType WeaponType = EWeaponType::Pistol2H;
		float RecoilStrength = 1.0f;
		float RecoilStartDelay = 0.03f;
		float RecoilEndDelay = 0.01f;

		// TODO: WeaponType will be based on the weapon equipped.
		IAnimationInterface::Execute_ShootingAddRecoil(AnimInst, WeaponType, RecoilStrength, RecoilStartDelay, RecoilEndDelay);
	}
	
	
	// TODO: We will first check what kind of gun is equipped

	FVector TraceStart;
	FVector TraceEnd;
	FHitResult HitResult;
	
	// Trace type for world dynamic, world static and physics body
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectToQuery = { EObjectTypeQuery::ObjectTypeQuery1, 
															EObjectTypeQuery::ObjectTypeQuery2 ,
															EObjectTypeQuery::ObjectTypeQuery4 };
	
	if (OverlayState == ECharacterOverlayState::Pistol)
	{
		const FTransform SocketTransform = SkeletaMeshObject->GetSocketTransform(CurrentPistolAsset.MuzzleSocketName);
		TraceStart = SocketTransform.GetLocation();
		TraceEnd = TraceStart + SocketTransform.GetRotation().GetAxisX() * 10000.0f;
	}
	
	UKismetSystemLibrary::LineTraceSingleForObjects(this, 
													TraceStart, 
													TraceEnd,
													ObjectToQuery, 
													false, 
													TArray<AActor*>(), 
													EDrawDebugTrace::ForOneFrame,
													HitResult, 
													true);
	
	if (HitResult.bBlockingHit)
	{
		OnBulletHitImpact_Implementation(HitResult);
	}

}

void AMLSCharacter::OnBulletHitImpact_Implementation(const FHitResult& HitResult)
{
	TWeakObjectPtr<UPhysicalMaterial> PhysicalMat = HitResult.PhysMaterial;
	const EPhysicalSurface SurfaceType = PhysicalMat.Get()->SurfaceType;

	if (SurfaceType != EPhysicalSurface::SurfaceType_Default &&
		SurfaceType != EPhysicalSurface::SurfaceType_Max)
	{
		GetHitFxInfoBySurfaceType(SurfaceType, HitImpactFX);
	}

	// Check the decal material soft object reference and load the decal material.
	if (UKismetSystemLibrary::IsValidSoftObjectReference(HitImpactFX.DecalMaterial) && HitImpactFX.DecalMaterial.LoadSynchronous())
	{
		const FName AttachmentPointName("NAME_None");

		const FRotator RotFromX = UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal);

		FRotator DecalRotation;

		// Check the orientation of the hit surface
		if (HitResult.ImpactNormal.Equals(FVector(0.0f, 0.0f, 1.0f), 0.001))
		{
			// For floor and ceiling per say
			DecalRotation.Pitch = 90.0f;
			DecalRotation.Yaw = RotFromX.Yaw;
			DecalRotation.Roll = -90.0f;
		}
		else
		{
			// For walls per say
			DecalRotation.Pitch = RotFromX.Pitch;
			DecalRotation.Yaw = RotFromX.Yaw + 180.0f;
			DecalRotation.Roll = 0.0f;
		}

		UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAttached(HitImpactFX.DecalMaterial.Get(),
			HitImpactFX.DecalSize,
			HitResult.GetComponent(),
			AttachmentPointName,
			HitResult.ImpactPoint,
			DecalRotation,
			HitImpactFX.DecalAttachmentType,
			5.0f);
		
		if (!IsValid(DecalComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Impact Decal could not be spawned"));
		}

	}

	// Play Sound for hit impact
	FTransform HitTransform = FTransform(FRotator(), HitResult.Location);
	SoundController->PlayBulletImpactSound(HitTransform, 1.0f, HitImpactFX);

	if (UKismetSystemLibrary::IsValidSoftObjectReference(HitImpactFX.NiagaraSystem) && HitImpactFX.NiagaraSystem.LoadSynchronous())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpactFX.NiagaraSystem.Get(), HitResult.Location);
	}

}

void AMLSCharacter::PlayShootSoundByWeaponType()
{
	// Later we'll check which kind of weapon is it.
	//
	// Later we'll check which kind of pistol is it.

	USoundCue* SoundToPlay = CurrentPistolAsset.ShootingSound;
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation());
}

void AMLSCharacter::EquipGunEventUpdateVisibility()
{
	if (SoundController)
	{
		// TODO: Will pass the gun type as well to distinguish differnt equip sounds.
		SoundController->PlayWeaponEquipSound(GetTransform(), 0.5f);
	}

	if (OverlayState == ECharacterOverlayState::Pistol)
	{
		PistolSkeletalMesh->SetVisibility(false);
	}

	SkeletaMeshObject->SetVisibility(true);
}

void AMLSCharacter::HolsterGunEventUpdateVisibility()
{
	if (SoundController)
	{
		// TODO: Will pass the gun type as well to distinguish differnt equip sounds.
		SoundController->PlayWeaponHolsterSound(GetTransform(), 0.2f);
	}

	SkeletaMeshObject->SetVisibility(false);
	PistolSkeletalMesh->SetVisibility(true);

	UpdateHeldObject();
}


void AMLSCharacter::ClearHeldObject()
{
	HeldStaticMesh->SetStaticMesh(nullptr);
	HeldSkeletalMesh->SetSkeletalMesh(nullptr);
	HeldSkeletalMesh->SetAnimInstanceClass(nullptr);
}

void AMLSCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass,
                                 bool bLeftHand, FVector Offset)
{
	ClearHeldObject();

	if (IsValid(NewStaticMesh))
	{
		HeldStaticMesh->SetStaticMesh(NewStaticMesh);
	}
	else if (IsValid(NewSkeletalMesh))
	{
		HeldSkeletalMesh->SetSkeletalMesh(NewSkeletalMesh);
		if (IsValid(NewAnimClass))
		{
			HeldSkeletalMesh->SetAnimInstanceClass(NewAnimClass);
		}
	}

	FName AttachBone;
	if (bLeftHand)
	{
		AttachBone = TEXT("VB LHS_ik_hand_gun");
	}
	else
	{
		AttachBone = TEXT("VB RHS_ik_hand_gun");
	}

	FAttachmentTransformRules NewRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	HeldObjectRoot->AttachToComponent(GetMesh(),
	                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachBone);
	HeldObjectRoot->SetRelativeLocation(Offset);
}

ECollisionChannel AMLSCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	const FName CameraSocketName = bRightShoulder ? TEXT("TP_CameraTrace_R") : TEXT("TP_CameraTrace_L");
	TraceOrigin = GetMesh()->GetSocketLocation(CameraSocketName);
	TraceRadius = 15.0f;
	return ECC_Camera;
}

FTransform AMLSCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("root"))) / 2.0f,
	                  FVector::OneVector);
}

void AMLSCharacter::OnOverlayStateChanged(ECharacterOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);

	// Only update held object if we are not transitioning from weapons to other state
	if (PreviousState != ECharacterOverlayState::Pistol)
	{
		UpdateHeldObject();
	}
}

void AMLSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHeldObjectAnimations();
}