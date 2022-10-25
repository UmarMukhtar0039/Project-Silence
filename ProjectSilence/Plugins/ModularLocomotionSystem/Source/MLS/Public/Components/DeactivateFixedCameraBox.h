// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CameraCollisionBox.h"
#include "DeactivateFixedCameraBox.generated.h"

/**
 * 
 */

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MLS_API UDeactivateFixedCameraBox : public UCameraCollisionBox
{
	GENERATED_BODY()
	
public:
	UDeactivateFixedCameraBox();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
