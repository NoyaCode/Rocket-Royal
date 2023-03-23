/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "SpawnPointActor.h"

#include "Components/BoxComponent.h"
#include "RocketRoyal/Player/MilitaryCharacter.h"
#include "RocketRoyal/Utils/Utils.h"

#pragma region Setup
ASpawnPointActor::ASpawnPointActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SpawnRootComponent = CreateDefaultSubobject<USceneComponent>("SpawnRoot");
	RootComponent  = SpawnRootComponent;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SpawnMesh");
	MeshComponent->SetupAttachment(RootComponent);

	ShapeComponent = CreateDefaultSubobject<UBoxComponent>("SpawnBox");
	ShapeComponent->SetupAttachment(RootComponent);

	const FString AssignedTeam = UEnum::GetDisplayValueAsText(GetTeamAssigned()).ToString();
	Tags.Add(FName(AssignedTeam + " Spawn"));
}

void ASpawnPointActor::BeginPlay()
{
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	
	ShapeComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	ShapeComponent->SetGenerateOverlapEvents(true);

	MeshComponent->SetMaterial(0, Material);
	
	Super::BeginPlay();
}

void ASpawnPointActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASpawnPointActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if(!OtherActor->Tags.Contains("Player")) return;	
	PlayersInZone.Add(Cast<AMilitaryCharacter>(OtherActor));

	//DebugWarning("SpawnPoint: BeginOverlap!");
}

void ASpawnPointActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
	if(!OtherActor->Tags.Contains("Player")) return;
	PlayersInZone.Remove(Cast<AMilitaryCharacter>(OtherActor));

	//DebugWarning("SpawnPoint: EndOverlap!");
}

void ASpawnPointActor::Tick(float DeltaSeconds)
{
	if(PlayersInZone.Num() == 0) return;
	for(int i = 0; i < PlayersInZone.Num(); i++)
	{
		if(CheckActorSpawnPoint(PlayersInZone[i])) PlayersInZone[i]->TakeHealEvent(HealingGainRate * 0.1f);
		else PlayersInZone[i]->TakeUltimateEvent(UltimateGainRate * 0.1f);
	}
	
	Super::Tick(DeltaSeconds);
}
#pragma endregion

#pragma region Getter & Setter
ESpawnTeam ASpawnPointActor::GetTeamAssigned() const
{
	return AssignedTeamSpawn;
}

bool ASpawnPointActor::CheckActorSpawnPoint(const AMilitaryCharacter* Character) const
{
	return Character->GetTeam() == GetTeamAssigned();
}

UStaticMeshComponent* ASpawnPointActor::GetMesh() const
{
	return IsValid(MeshComponent) ? MeshComponent : nullptr;
}

#pragma endregion
