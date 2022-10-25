



#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Library/MLSCharacterEnumLibrary.h"

#include "MLSAnimNotifyGroundedEntryState.generated.h"

/**
 * 
 */
UCLASS()
class MLS_API UMLSAnimNotifyGroundedEntryState : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
