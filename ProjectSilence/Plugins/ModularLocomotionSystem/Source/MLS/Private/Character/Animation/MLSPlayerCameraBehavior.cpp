#include "Character/Animation/MLSPlayerCameraBehavior.h"


#include "Character/MLSBaseCharacter.h"

void UMLSPlayerCameraBehavior::SetRotationMode(ECharacterRotationMode RotationMode)
{
	bVelocityDirection = RotationMode == ECharacterRotationMode::VelocityDirection;
	bLookingDirection = RotationMode == ECharacterRotationMode::LookingDirection;
	bAiming = RotationMode == ECharacterRotationMode::Aiming;
}

void UMLSPlayerCameraBehavior::SetViewMode(ECharacterViewMode ViewModeInput)
{
	ViewMode = ViewModeInput;
	bFixedCamera = ViewMode == ViewModeInput;

	//// Disable collision on camera when it is in fixed camera mode, enable otherwise.
	//if (ViewMode == ECharacterViewMode::FixedCamera)
	//{
	//	GetSkelMeshComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}
	//else
	//{
	//	GetSkelMeshComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//}
}