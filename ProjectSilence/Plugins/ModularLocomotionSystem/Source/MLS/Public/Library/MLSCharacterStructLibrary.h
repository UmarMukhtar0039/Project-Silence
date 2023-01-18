#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Materials/MaterialInterface.h"
#include "Library/MLSCharacterEnumLibrary.h"

#include "MLSCharacterStructLibrary.generated.h"

class UCurveVector;
class UAnimMontage;
class UAnimSequenceBase;
class UCurveFloat;
class UNiagaraSystem;
class UMaterialInterface;
class USoundBase;
class UPrimitiveComponent;
class USoundCue;
class UParticleSystem;

USTRUCT(BlueprintType)
struct FCharacterComponentAndTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Character Struct Library")
	FTransform Transform;

	UPROPERTY(EditAnywhere, Category = "Character Struct Library")
	TObjectPtr<UPrimitiveComponent> Component = nullptr;
};

USTRUCT(BlueprintType)
struct FMLSCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TargetArmLength = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector SocketOffset;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float LagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float RotationLagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bDoCollisionTest = true;
};

USTRUCT(BlueprintType)
struct FMLSCameraGaitSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	FMLSCameraSettings Walking;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FMLSCameraSettings Running;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FMLSCameraSettings Sprinting;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FMLSCameraSettings Crouching;
};

USTRUCT(BlueprintType)
struct FMLSCameraStateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Camera")
	FMLSCameraGaitSettings VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FMLSCameraGaitSettings LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FMLSCameraGaitSettings Aiming;
};

USTRUCT(BlueprintType)
struct FMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float WalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float RunSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	float SprintSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	TObjectPtr<UCurveVector> MovementCurve = nullptr;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	TObjectPtr<UCurveFloat> RotationRateCurve = nullptr;

	float GetSpeedForGait(const ECharacterGait Gait) const
	{
		switch (Gait)
		{
		case ECharacterGait::Running:
			return RunSpeed;
		case ECharacterGait::Walking:
			return WalkSpeed;
		default:
			return RunSpeed;
		}
	}
};

USTRUCT(BlueprintType)
struct FMLSMovementStanceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMovementSettings Standing;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMovementSettings Crouching;
};

USTRUCT(BlueprintType)
struct FMovementStateSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMLSMovementStanceSettings VelocityDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMLSMovementStanceSettings LookingDirection;

	UPROPERTY(EditAnywhere, Category = "Movement Settings")
	FMLSMovementStanceSettings Aiming;
};

USTRUCT(BlueprintType)
struct FMLSRotateInPlaceAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	TObjectPtr<UAnimSequenceBase> Animation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	FName SlotName;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float SlowTurnRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float FastTurnRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float SlowPlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation System")
	float FastPlayRate = 1.0f;
};

USTRUCT(BlueprintType)
struct FMLSHitFX : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Surface")
	TEnumAsByte<enum EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	ECharacterSpawnType SoundSpawnType;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = (EditCondition = "SoundSpawnType == ECharacterSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> SoundAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Sound")
	FVector SoundLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Sound")
	FRotator SoundRotationOffset;

	UPROPERTY(EditAnywhere, Category = "Decal")
	TSoftObjectPtr<UMaterialInterface> DecalMaterial;

	UPROPERTY(EditAnywhere, Category = "Decal")
	ECharacterSpawnType DecalSpawnType;

	UPROPERTY(EditAnywhere, Category = "Decal", meta = (EditCondition = "DecalSpawnType == ECharacterSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> DecalAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Decal")
	float DecalLifeSpan = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalSize;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FVector DecalLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Decal")
	FRotator DecalRotationOffset;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	TSoftObjectPtr<UParticleSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	ECharacterSpawnType NiagaraSpawnType;

	UPROPERTY(EditAnywhere, Category = "Niagara", meta = (EditCondition = "NiagaraSpawnType == ECharacterSpawnType::Attached"))
	TEnumAsByte<enum EAttachLocation::Type> NiagaraAttachmentType;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	FVector NiagaraLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	FRotator NiagaraRotationOffset;
};


USTRUCT(BlueprintType)
struct FPistolAssetSetting : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	TEnumAsByte<EPistolModel> PistolModel = EPistolModel::Pistol_M9;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	TObjectPtr<USkeletalMesh> SkeletaMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	TObjectPtr<UPhysicsAsset> PhysicsAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	TObjectPtr<UStaticMesh> MagazineMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	FName MagazineSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	TObjectPtr<USoundCue> ShootingSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	float DamageStrength;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	int32 MaxAmmoCount;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	float ShootingSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	int32 AmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	EAmmoType AmmoType;

	// This is required to reference the AnimInstace class in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pistol Settings")
	TObjectPtr<UAnimBlueprintGeneratedClass> AnimBlueprint = nullptr;
};