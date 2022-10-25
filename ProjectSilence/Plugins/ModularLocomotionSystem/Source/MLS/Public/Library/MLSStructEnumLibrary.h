


#pragma once

#include "CoreMinimal.h"
#include "MLSCharacterEnumLibrary.h"

#include "MLSStructEnumLibrary.generated.h"


USTRUCT(BlueprintType)
struct FMLSMovementState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	ECharacterMovementState State = ECharacterMovementState::None;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool None_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool Grounded_ = false;

public:
	FMLSMovementState()
	{
	}

	FMLSMovementState(const ECharacterMovementState InitialState) { *this = InitialState; }

	const bool& None() const { return None_; }
	const bool& Grounded() const { return Grounded_; }

	operator ECharacterMovementState() const { return State; }

	void operator=(const ECharacterMovementState NewState)
	{
		State = NewState;
		None_ = State == ECharacterMovementState::None;
		Grounded_ = State == ECharacterMovementState::Grounded;
	}
};

USTRUCT(BlueprintType)
struct FMLSStance
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	ECharacterStance Stance = ECharacterStance::Standing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	bool Standing_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	bool Crouching_ = false;

public:
	FMLSStance()
	{
	}

	FMLSStance(const ECharacterStance InitialStance) { *this = InitialStance; }

	const bool& Standing() const { return Standing_; }
	const bool& Crouching() const { return Crouching_; }

	operator ECharacterStance() const { return Stance; }

	void operator=(const ECharacterStance NewStance)
	{
		Stance = NewStance;
		Standing_ = Stance == ECharacterStance::Standing;
		Crouching_ = Stance == ECharacterStance::Crouching;
	}
};

USTRUCT(BlueprintType)
struct FMLSRotationMode
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Rotation System")
	ECharacterRotationMode RotationMode = ECharacterRotationMode::VelocityDirection;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Rotation System")
	bool VelocityDirection_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Rotation System")
	bool LookingDirection_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Rotation System")
	bool Aiming_ = false;

public:
	FMLSRotationMode()
	{
	}

	FMLSRotationMode(const ECharacterRotationMode InitialRotationMode) { *this = InitialRotationMode; }

	const bool& VelocityDirection() const { return VelocityDirection_; }
	const bool& LookingDirection() const { return LookingDirection_; }
	const bool& Aiming() const { return Aiming_; }

	operator ECharacterRotationMode() const { return RotationMode; }

	void operator=(const ECharacterRotationMode NewRotationMode)
	{
		RotationMode = NewRotationMode;
		VelocityDirection_ = RotationMode == ECharacterRotationMode::VelocityDirection;
		LookingDirection_ = RotationMode == ECharacterRotationMode::LookingDirection;
		Aiming_ = RotationMode == ECharacterRotationMode::Aiming;
	}
};

USTRUCT(BlueprintType)
struct FMLSMovementDirection
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	ECharacterMovementDirection MovementDirection = ECharacterMovementDirection::Forward;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool Forward_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool Right_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool Left_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool Backward_ = false;

public:
	FMLSMovementDirection()
	{
	}

	FMLSMovementDirection(const ECharacterMovementDirection InitialMovementDirection)
	{
		*this = InitialMovementDirection;
	}

	const bool& Forward() const { return Forward_; }
	const bool& Right() const { return Right_; }
	const bool& Left() const { return Left_; }
	const bool& Backward() const { return Backward_; }

	operator ECharacterMovementDirection() const { return MovementDirection; }

	void operator=(const ECharacterMovementDirection NewMovementDirection)
	{
		MovementDirection = NewMovementDirection;
		Forward_ = MovementDirection == ECharacterMovementDirection::Forward;
		Right_ = MovementDirection == ECharacterMovementDirection::Right;
		Left_ = MovementDirection == ECharacterMovementDirection::Left;
		Backward_ = MovementDirection == ECharacterMovementDirection::Backward;
	}
};

USTRUCT(BlueprintType)
struct FMLSGait
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	ECharacterGait Gait = ECharacterGait::Walking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool Walking_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool Running_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Movement System")
	bool Sprinting_ = false;

public:
	FMLSGait()
	{
	}

	FMLSGait(const ECharacterGait InitialGait) { *this = InitialGait; }

	const bool& Walking() const { return Walking_; }
	const bool& Running() const { return Running_; }
	const bool& Sprinting() const { return Sprinting_; }

	operator ECharacterGait() const { return Gait; }

	void operator=(const ECharacterGait NewGait)
	{
		Gait = NewGait;
		Walking_ = Gait == ECharacterGait::Walking;
		Running_ = Gait == ECharacterGait::Running;
	}
};

USTRUCT(BlueprintType)
struct FMLSOverlayState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	ECharacterOverlayState State = ECharacterOverlayState::Default;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	bool Default_ = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	bool Masculine_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	bool Feminine_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	bool Rifle_ = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "MLS|Character States")
	bool Pistol_ = false;

public:
	FMLSOverlayState()
	{
	}

	FMLSOverlayState(const ECharacterOverlayState InitialState) { *this = InitialState; }

	const bool& Default() const { return Default_; }
	const bool& Masculine() const { return Masculine_; }
	const bool& Feminine() const { return Feminine_; }
	const bool& Rifle() const { return Rifle_; }
	const bool& Pistol() const { return Pistol_; }

	operator ECharacterOverlayState() const { return State; }

	void operator=(const ECharacterOverlayState NewAction)
	{
		State = NewAction;
		Default_ = State == ECharacterOverlayState::Default;
		Masculine_ = State == ECharacterOverlayState::Masculine;
		Feminine_ = State == ECharacterOverlayState::Feminine;
		Rifle_ = State == ECharacterOverlayState::Rifle;
		Pistol_ = State == ECharacterOverlayState::Pistol;
	}
};