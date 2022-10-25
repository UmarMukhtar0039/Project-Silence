


#pragma once

#include "MLSCharacterEnumLibrary.h"

#include "Kismet/KismetSystemLibrary.h"
#include "CoreMinimal.h"
#include "Library/MLSCharacterStructLibrary.h"
#include "MLSMathLibrary.generated.h"

class UCapsuleComponent;

/**
 * Math library functions for ALS
 */
UCLASS()
class MLS_API UMLSMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MLS|Math Utils")
	static FTransform TransformSub(const FTransform& T1, const FTransform& T2)
	{
		return FTransform(T1.GetRotation().Rotator() - T2.GetRotation().Rotator(),
		                  T1.GetLocation() - T2.GetLocation(), T1.GetScale3D() - T2.GetScale3D());
	}

	UFUNCTION(BlueprintCallable, Category = "MLS|Math Utils")
	static FTransform TransformAdd(const FTransform& T1, const FTransform& T2)
	{
		return FTransform(T1.GetRotation().Rotator() + T2.GetRotation().Rotator(),
		                  T1.GetLocation() + T2.GetLocation(), T1.GetScale3D() + T2.GetScale3D());
	}

	UFUNCTION(BlueprintCallable, Category = "MLS|Math Utils")
	static FVector GetCapsuleBaseLocation(float ZOffset, UCapsuleComponent* Capsule);

	UFUNCTION(BlueprintCallable, Category = "MLS|Math Utils")
	static FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset, UCapsuleComponent* Capsule);

	UFUNCTION(BlueprintCallable, Category = "MLS|Math Utils")
	static bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset,
	                                float RadiusOffset, EDrawDebugTrace::Type DebugType = EDrawDebugTrace::Type::None, bool DrawDebugTrace = false);

	UFUNCTION(BlueprintCallable, Category = "MLS|Math Utils")
	static bool AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer);

	UFUNCTION(BlueprintCallable, Category = "MLS|Math Utils")
	static ECharacterMovementDirection CalculateQuadrant(ECharacterMovementDirection Current, float FRThreshold, float FLThreshold,
	                                               float BRThreshold,
	                                               float BLThreshold, float Buffer, float Angle);
};
