#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MLSItem.generated.h"

class UBoxComponent;

UCLASS()
class MLS_API AMLSItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMLSItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//public:
//  This is being done in BP.
//	UFUNCTION()
//	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|Items|Components")
	UBoxComponent* ItemCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|Items|Components")
	UStaticMeshComponent* ItemStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|Items|Components")
	USkeletalMeshComponent* ItemSkeletalMesh = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, Category = "MLS|Items")
	int32 Amount = 1;
};
