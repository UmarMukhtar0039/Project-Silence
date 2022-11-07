// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Animation/Notify/MLSAnimNotifyGunEquip.h"
#include "Character/MLSCharacter.h"
#include "Character/Animation/MLSCharacterAnimInstance.h"

void UMLSAnimNotifyGunEquip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AActor* MeshOwner = MeshComp->GetOwner();
	if (!MeshOwner)
	{
		return;
	}

	AMLSCharacter* Character = Cast<AMLSCharacter>(MeshOwner);

	if (Character)
	{
		Character->EquipGunEventUpdateVisibility();

		UMLSCharacterAnimInstance* AnimInst = Cast<UMLSCharacterAnimInstance>(MeshComp->GetAnimInstance());

		if (AnimInst)
		{
			AnimInst->bHolsterFinishedPlaying = false;
		}
	}
}	