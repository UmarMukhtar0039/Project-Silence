



#include "Character/Animation/Notify/MLSNotifyStateOverlayOverride.h"

#include "Character/MLSBaseCharacter.h"
#include "Character/Animation/MLSCharacterAnimInstance.h"

void UMLSNotifyStateOverlayOverride::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AMLSBaseCharacter* Character = Cast<AMLSBaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetOverlayOverrideState(OverlayOverrideState);
	}
}

void UMLSNotifyStateOverlayOverride::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AMLSBaseCharacter* Character = Cast<AMLSBaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetOverlayOverrideState(0);
	}
}

FString UMLSNotifyStateOverlayOverride::GetNotifyName_Implementation() const
{
	FString Name(TEXT("Overlay Override State: "));
	Name.Append(FString::FromInt(OverlayOverrideState));
	return Name;
}
