#include "Character/MLSFixedCamera.h"
#include "Character/MLSPlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/BillBoardComponent.h"


bool AMLSFixedCamera::bAllCameraInactive = true;

// Sets default values
AMLSFixedCamera::AMLSFixedCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SetActorTickEnabled(false);
	
	FixedCameraState = EMLSFixedCameraState::InActive;

	bIsStaticCamera = false;

	CameraLocationDebug = CreateDefaultSubobject<UBillboardComponent>(TEXT("CameraLocationDebug"));

	// Make the billboard component a root to debug the camera position.
	SetRootComponent(CameraLocationDebug);

	bUseBoxToActivateOrDeactivate = false;
}

void AMLSFixedCamera::PostActorCreated()
{
	Super::PostActorCreated();

	//ActivateCameraBox->SetWorldLocation(GetActorLocation());
	//DeactivateCameraBox->SetWorldLocation(GetActorLocation());
	//ForceThirdPersonBox->SetWorldLocation(GetActorLocation());
}

// Called when the game starts or when spawned
void AMLSFixedCamera::BeginPlay()
{
	Super::BeginPlay();

	// Get all child components
	TArray<USceneComponent*> AllChildren;
	RootComponent->GetChildrenComponents(false, AllChildren);
	
	// Iterate over all child components and detach them from parents.
	// This is being done because we don't have the FixedCam actor's transform to affect the Collision boxes
	for (auto& Child : AllChildren)
	{
		// Save the world transform because when we detach the child's transform will be relative to the world's root.
		//const FTransform CurrentWorldTransform = Child->GetComponentTransform();
		Child->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//Child->SetWorldTransform(CurrentWorldTransform);
	}

	// TODO: Probably do this with an interface?
	AMLSPlayerCameraManager* PlayerCameraManager = Cast<AMLSPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	
	if (PlayerCameraManager)
	{
		// Adds this camera to player cam manager to keep track of all the cameras we have in this level.
		PlayerCameraManager->AddFixedCamera(this);
	}
}

void AMLSFixedCamera::OnActivateBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMLSPlayerCameraManager* PlayerCameraManager = Cast<AMLSPlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	
	AMLSFixedCamera* CurrentlyActive = PlayerCameraManager->GetCurrentFixedCamera();

	if (CurrentlyActive)
	{
		if (CurrentlyActive == this)
		{
			return;
		}

		CurrentlyActive->Deactivate();
	}

	Activate(OtherActor);
}

void AMLSFixedCamera::OnDeactivateBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Deactivate();
}


// Called every frame
void AMLSFixedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LookTarget)
	{
		if (!bIsStaticCamera)
		{
			FVector FixedCamLoc = GetActorLocation();
			FVector LookTargetLoc = LookTarget->GetActorLocation();
		
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(FixedCamLoc, LookTargetLoc);

			// Needn't use smoothed rotation as this is not the real camera we rotate
			SetActorRotation(NewRotation);
		}
		

		// Check if target is in view
		FHitResult HitResult;
		TraceToLookTarget(HitResult, LookTarget);

		AActor* HitActor = HitResult.GetActor();
		FString Name = UKismetSystemLibrary::GetDisplayName(HitActor);

		if (HitActor != LookTarget && !bUseBoxToActivateOrDeactivate)
		{
			Deactivate();
		}
	}
}

void AMLSFixedCamera::SetState(EMLSFixedCameraState NewState)
{
	FixedCameraState = NewState;
}

void AMLSFixedCamera::Activate(AActor* NewLookTarget)
{
	if(NewLookTarget)
	LookTarget = NewLookTarget;

	bIsActive = true;

	SetActorTickEnabled(true);

	SetState(EMLSFixedCameraState::Active);

	bAllCameraInactive = false;

	UE_LOG(LogTemp, Warning, TEXT("Camera Active"));
}

bool AMLSFixedCamera::TraceToLookTarget(FHitResult& HitResultOut, const AActor* LookTargetIn) const
{
	TArray<AActor*> ActorsToIgnore;

	FVector FixedCamLoc = GetActorLocation();

	FVector LookTargetLoc = LookTargetIn->GetActorLocation();

	// Trace from Camera to Player to see if there isn't any object in between.
	// TraceTypeQuery2 corresponds to ECC_Camera trace channel. Visibility is ignored.
	return UKismetSystemLibrary::LineTraceSingle(this, FixedCamLoc, LookTargetLoc, ETraceTypeQuery::TraceTypeQuery3, false, ActorsToIgnore, EDrawDebugTrace::None, HitResultOut, true);
}

void AMLSFixedCamera::Deactivate()
{
	LookTarget = nullptr;

	bIsActive = false;

	SetActorTickEnabled(false);

	SetState(EMLSFixedCameraState::InActive);

	bAllCameraInactive = true;

}
