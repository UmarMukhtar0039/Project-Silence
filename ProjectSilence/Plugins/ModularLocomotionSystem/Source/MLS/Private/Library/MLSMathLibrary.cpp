



#include "Library/MLSMathLibrary.h"


#include "Library/MLSCharacterStructLibrary.h"
#include "Components/ALSDebugComponent.h"

#include "Components/CapsuleComponent.h"

FVector UMLSMathLibrary::GetCapsuleBaseLocation(const float ZOffset, UCapsuleComponent* Capsule)
{
	return Capsule->GetComponentLocation() -
		Capsule->GetUpVector() * (Capsule->GetScaledCapsuleHalfHeight() + ZOffset);
}

FVector UMLSMathLibrary::GetCapsuleLocationFromBase(FVector BaseLocation, const float ZOffset,
                                                    UCapsuleComponent* Capsule)
{
	BaseLocation.Z += Capsule->GetScaledCapsuleHalfHeight() + ZOffset;
	return BaseLocation;
}

bool UMLSMathLibrary::CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset,
                                          float RadiusOffset, EDrawDebugTrace::Type DebugType, bool DrawDebugTrace)
{
	// Perform a trace to see if the capsule has room to be at the target location.
	const float ZTarget = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere() - RadiusOffset + HeightOffset;
	FVector TraceStart = TargetLocation;
	TraceStart.Z += ZTarget;
	FVector TraceEnd = TargetLocation;
	TraceEnd.Z -= ZTarget;
	const float Radius = Capsule->GetUnscaledCapsuleRadius() + RadiusOffset;

	const UWorld* World = Capsule->GetWorld();
	check(World);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Capsule->GetOwner());

	FHitResult HitResult;
	const FCollisionShape SphereCollisionShape = FCollisionShape::MakeSphere(Radius);
	const bool bHit = World->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity,
	                                              ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params);

	if (DrawDebugTrace)
	{
		UALSDebugComponent::DrawDebugSphereTraceSingle(World,
		                                               TraceStart,
		                                               TraceEnd,
		                                               SphereCollisionShape,
		                                               DebugType,
		                                               bHit,
		                                               HitResult,
		                                               FLinearColor(0.130706f, 0.896269f, 0.144582f, 1.0f),  // light green
		                                               FLinearColor(0.932733f, 0.29136f, 1.0f, 1.0f),        // light purple
		                                               1.0f);
	}

	return !(HitResult.bBlockingHit || HitResult.bStartPenetrating);
}

bool UMLSMathLibrary::AngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool IncreaseBuffer)
{
	if (IncreaseBuffer)
	{
		return Angle >= MinAngle - Buffer && Angle <= MaxAngle + Buffer;
	}
	return Angle >= MinAngle + Buffer && Angle <= MaxAngle - Buffer;
}

ECharacterMovementDirection UMLSMathLibrary::CalculateQuadrant(ECharacterMovementDirection Current, float FRThreshold,
                                                         float FLThreshold,
                                                         float BRThreshold, float BLThreshold, float Buffer,
                                                         float Angle)
{
	// Take the input angle and determine its quadrant (direction). Use the current Movement Direction to increase or
	// decrease the buffers on the angle ranges for each quadrant.
	if (AngleInRange(Angle, FLThreshold, FRThreshold, Buffer,
	                 Current != ECharacterMovementDirection::Forward && Current != ECharacterMovementDirection::Backward))
	{
		return ECharacterMovementDirection::Forward;
	}

	if (AngleInRange(Angle, FRThreshold, BRThreshold, Buffer,
	                 Current != ECharacterMovementDirection::Right && Current != ECharacterMovementDirection::Left))
	{
		return ECharacterMovementDirection::Right;
	}

	if (AngleInRange(Angle, BLThreshold, FLThreshold, Buffer,
	                 Current != ECharacterMovementDirection::Right && Current != ECharacterMovementDirection::Left))
	{
		return ECharacterMovementDirection::Left;
	}

	return ECharacterMovementDirection::Backward;
}
