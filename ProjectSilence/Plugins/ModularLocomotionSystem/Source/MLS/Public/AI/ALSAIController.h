



#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "ALSAIController.generated.h"

/**
 * 
 */
UCLASS()
class MLS_API AALSAIController : public AAIController
{
	GENERATED_BODY()

public:
	AALSAIController();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> Behaviour = nullptr;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual FVector GetFocalPointOnActor(const AActor *Actor) const override;
};
