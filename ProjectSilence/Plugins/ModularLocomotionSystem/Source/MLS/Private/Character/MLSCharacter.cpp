#include "Character/MLSCharacter.h"

#include "Engine/StaticMesh.h"
#include "Engine/SkeletalMesh.h"
#include "AI/ALSAIController.h"
#include "Kismet/GameplayStatics.h"

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

	AIControllerClass = AALSAIController::StaticClass();
}

void AMLSCharacter::EquipItem()
{
	Super::EquipItem();

}

void AMLSCharacter::EquipGunEventUpdateVisibility()
{
	if (OverlayState == ECharacterOverlayState::Pistol)
	{
		PistolSkeletalMesh->SetVisibility(false);
	}

	SkeletaMeshObject->SetVisibility(true);
}

void AMLSCharacter::HolsterGunEventUpdateVisibility()
{
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

FVector AMLSCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
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

void AMLSCharacter::BeginPlay()
{
	Super::BeginPlay();

	SkeletaMeshObject = HeldSkeletalMesh;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	PistolAttachmentMesh->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Pistol_Attachment")));

	UpdateHeldObject();
}
