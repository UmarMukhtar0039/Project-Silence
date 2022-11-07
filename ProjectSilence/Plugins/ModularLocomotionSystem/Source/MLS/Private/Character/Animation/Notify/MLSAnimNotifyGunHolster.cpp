#include "Character/Animation/Notify/MLSAnimNotifyGunHolster.h"
#include "Character/MLSCharacter.h"
#include "Character/Animation/MLSCharacterAnimInstance.h"

void UMLSAnimNotifyGunHolster::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
		Character->HolsterGunEventUpdateVisibility();

		UMLSCharacterAnimInstance* AnimInst = Cast<UMLSCharacterAnimInstance>(MeshComp->GetAnimInstance());

		if (AnimInst)
		{
			AnimInst->bHolsterFinishedPlaying = true;
		}
	}
}
