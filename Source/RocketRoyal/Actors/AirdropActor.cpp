/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "AirdropActor.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "RocketRoyal/Utils/Utils.h"

#pragma region Setup
AAirdropActor::AAirdropActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* AirdropRootComponent = CreateDefaultSubobject<USceneComponent>("AirdropRoot");
	RootComponent  = AirdropRootComponent;

	DropMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("DropMesh");
	DropMeshComponent->SetupAttachment(RootComponent);
	
	ParachuteMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ParachuteMesh");
	ParachuteMeshComponent->SetupAttachment(RootComponent);
	
	AirdropShapeComponent = CreateDefaultSubobject<UBoxComponent>("AirdropBox");
	AirdropShapeComponent->SetupAttachment(RootComponent);

	Tags.Add("Airdrop");
}

void AAirdropActor::BeginPlay()
{
	Super::BeginPlay();

	FinalLocation = GetActorLocation();

	ParachuteMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	DropMeshComponent	  ->SetCollisionResponseToAllChannels(ECR_Block);
	
	AirdropShapeComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	AirdropShapeComponent->SetGenerateOverlapEvents(true);

	SetActorScale3D(FVector(2.f));
}

void AAirdropActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
#pragma endregion

#pragma region Update
void AAirdropActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!IsOnFloor())SetActorLocation(GetActorLocation() - FVector(0, 0, DescendingRate) * DeltaSeconds);
	else if(IsValid(ParachuteMeshComponent))
		ParachuteMeshComponent->DestroyComponent();
}

void AAirdropActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if(!IsOnFloor()) return;
	if(!OtherActor->Tags.Contains("Player")) return;
	
	if(DropsBP.Num() == 0) return;

	const FVector  Location = GetActorLocation();
	const FRotator Rotation = GetActorRotation();
	
	GetWorld()->SpawnActor(DropsBP[PickRandomDrop()], &Location, &Rotation);
	
	Destroy();
	// TODO: VFX + BOOM Sound
}
#pragma endregion

#pragma region Miscs
void AAirdropActor::BindAirdropDestroyTimer()
{
	DebugInfo("Started AirdropDestroyTimer");
	InDestroyState = true;
	
	/*FTimerHandle TimerHandle = {};
	FTimerDelegate TimerDelegate = {};
	TimerDelegate.BindLambda([this]{ Destroy(); });
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false, DespawnDelay);*/
}

#pragma endregion

#pragma region Getters & Setters
bool AAirdropActor::IsOnFloor() const
{
	return GetActorLocation().Z <= GetFinalLocation().Z;
}

FVector AAirdropActor::GetFinalLocation() const
{
	return FinalLocation;
}

int AAirdropActor::PickRandomDrop() const
{
	return FMath::RandRange(0, DropsBP.Num()-1);
}

void AAirdropActor::SetFinalLocation(const FVector& Location)
{
	FinalLocation = Location;
}
#pragma endregion 
