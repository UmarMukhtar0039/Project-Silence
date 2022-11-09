// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Library/MLSCharacterEnumLibrary.h"

#include "AnimationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MLS_API IAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MLS|Interface|Weapon")
	void ShootingAddRecoil(EWeaponType WeaponType, float RecoilStrength, float RecoilStartDelay, float RecoilEndDelay);
};
