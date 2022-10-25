


#pragma once

#include "CoreMinimal.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "ALSDebugComponent.generated.h"

class AMLSBaseCharacter;
class USkeletalMesh;

UCLASS(Blueprintable, BlueprintType)
class MLS_API UALSDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UALSDebugComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "MLS|Debug")
	void OnPlayerControllerInitialized(APlayerController* Controller);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	/** Implemented on BP to update layering colors */
	UFUNCTION(BlueprintImplementableEvent, Category = "MLS|Debug")
	void UpdateColoringSystem();

	/** Implement on BP to draw debug spheres */
	UFUNCTION(BlueprintImplementableEvent, Category = "MLS|Debug")
	void DrawDebugSpheres();

	/** Implemented on BP to set/reset layering colors */
	UFUNCTION(BlueprintImplementableEvent, Category = "MLS|Debug")
	void SetResetColors();

	/** Implemented on BP to set dynamic color materials for debugging */
	UFUNCTION(BlueprintImplementableEvent, Category = "MLS|Debug")
	void SetDynamicMaterials();

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleGlobalTimeDilationLocal(float TimeDilation);

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleSlomo();

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleHud() { bShowHud = !bShowHud; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleDebugView();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Debug")
	void OpenOverlayMenu(bool bValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MLS|Debug")
	void OverlayMenuCycle(bool bValue);

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleDebugMesh();

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleTraces() { bShowTraces = !bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleDebugShapes() { bShowDebugShapes = !bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleLayerColors() { bShowLayerColors = !bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void ToggleCharacterInfo() { bShowCharacterInfo = !bShowCharacterInfo; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	bool GetDebugView() { return bDebugView; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	bool GetShowTraces() { return bShowTraces; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	bool GetShowDebugShapes() { return bShowDebugShapes; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	bool GetShowLayerColors() { return bShowLayerColors; }

	UFUNCTION(BlueprintCallable, Category = "MLS|Debug")
	void FocusedDebugCharacterCycle(bool bValue);

	// utility functions to draw trace debug shapes,
	// which are derived from Engine/Private/KismetTraceUtils.h.
	// Sadly the functions are private, which was the reason
	// why there reimplemented here.
	static void DrawDebugLineTraceSingle(const UWorld* World,
	                                     const FVector& Start,
	                                     const FVector& End,
	                                     EDrawDebugTrace::Type DrawDebugType,
	                                     bool bHit,
	                                     const FHitResult& OutHit,
	                                     FLinearColor TraceColor,
	                                     FLinearColor TraceHitColor,
	                                     float DrawTime);

	static void DrawDebugCapsuleTraceSingle(const UWorld* World,
	                                        const FVector& Start,
	                                        const FVector& End,
	                                        const FCollisionShape& CollisionShape,
	                                        EDrawDebugTrace::Type DrawDebugType,
	                                        bool bHit,
	                                        const FHitResult& OutHit,
	                                        FLinearColor TraceColor,
	                                        FLinearColor TraceHitColor,
	                                        float DrawTime);

	static void DrawDebugSphereTraceSingle(const UWorld* World,
	                                       const FVector& Start,
	                                       const FVector& End,
	                                       const FCollisionShape& CollisionShape,
	                                       EDrawDebugTrace::Type DrawDebugType,
	                                       bool bHit,
	                                       const FHitResult& OutHit,
	                                       FLinearColor TraceColor,
	                                       FLinearColor TraceHitColor,
	                                       float DrawTime);

protected:
	void DetectDebuggableCharactersInWorld();

public:
	UPROPERTY(BlueprintReadOnly, Category = "MLS|Debug")
	TObjectPtr<AMLSBaseCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|Debug")
	bool bSlomo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|Debug")
	bool bShowHud = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|Debug")
	bool bShowCharacterInfo = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MLS|Debug")
	TObjectPtr<USkeletalMesh> DebugSkeletalMesh = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Debug")
	TArray<TObjectPtr<AMLSBaseCharacter>> AvailableDebugCharacters;

	UPROPERTY(BlueprintReadOnly, Category = "MLS|Debug")
	TObjectPtr<AMLSBaseCharacter> DebugFocusCharacter = nullptr;
private:
	static bool bDebugView;

	static bool bShowTraces;

	static bool bShowDebugShapes;

	static bool bShowLayerColors;

	bool bNeedsColorReset = false;

	bool bDebugMeshVisible = false;

	UPROPERTY()
	TObjectPtr<USkeletalMesh> DefaultSkeletalMesh = nullptr;

	/// Stores the index, which is used to select the next focused debug MLSBaseCharacter.
	/// If no characters where found during BeginPlay the value should be set to INDEX_NONE.
	int32 FocusedDebugCharacterIndex = INDEX_NONE;
};
