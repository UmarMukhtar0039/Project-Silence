#pragma once

#include "CoreMinimal.h"
#include "MLSCharacterEnumLibrary.generated.h"

/* Returns the enumeration index. */
template <typename Enumeration>
static FORCEINLINE int32 GetEnumerationIndex(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetIndexByValue(static_cast<int64>(InValue));
}

/* Returns the enumeration value as string. */
template <typename Enumeration>
static FORCEINLINE FString GetEnumerationToString(const Enumeration InValue)
{
	return StaticEnum<Enumeration>()->GetNameStringByValue(static_cast<int64>(InValue));
}

/**
 * Character gait state. Note: Also edit related struct in MLSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class ECharacterGait : uint8
{
	Walking,
	Running
};

/**
 * Character movement state. Note: Also edit related struct in MLSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	None,
	Grounded
};

/**
 * Character overlay state. Note: Also edit related struct in MLSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class ECharacterOverlayState : uint8
{
	Default,
	Masculine,
	Feminine,
	Rifle,
	Pistol,
};

/**
 * Character rotation mode. Note: Also edit related struct in MLSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class ECharacterRotationMode : uint8
{
	VelocityDirection,
	LookingDirection,
	CharacterDirection,
	Aiming
};

/**
 * Character stance. Note: Also edit related struct in MLSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class ECharacterStance : uint8
{
	Standing,
	Crouching
};

/**
 * Character view mode. Note: Also edit related struct in MLSStructEnumLibrary if you add new enums
 */
UENUM(BlueprintType)
enum class ECharacterViewMode : uint8
{
	ThirdPerson,
	FixedCamera,
	Default,
};

UENUM(BlueprintType)
enum class ECharacterAnimFeatureExample : uint8
{
	StrideBlending,
	AdditiveBlending
};

UENUM(BlueprintType)
enum class ECharacterFootstepType : uint8
{
	Step,
	WalkRun
};

UENUM(BlueprintType)
enum class ECharacterHipsDirection : uint8
{
	F,
	B,
	RF,
	RB,
	LF,
	LB
};

UENUM(BlueprintType)
enum class ECharacterMovementDirection : uint8
{
	Forward,
	Right,
	Left,
	Backward
};

UENUM(BlueprintType)
enum class ECharacterSpawnType : uint8
{
	Location,
	Attached
};

UENUM(BlueprintType)
enum EPistolModel
{
	Pistol_M9				UMETA(DisplayName = "M9"),
	Pistol_DesertEagle		UMETA(DisplayName = "DesertEagle")
};

UENUM(BlueprintType)
enum class ECharacterCombatState : uint8
{
	NONE,

	Ranged,
	Melee,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol2H,
	Rifle,
};