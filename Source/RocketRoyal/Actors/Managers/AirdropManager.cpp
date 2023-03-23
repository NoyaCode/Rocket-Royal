// Fill out your copyright notice in the Description page of Project Settings.


#include "AirdropManager.h"

#include "DrawDebugHelpers.h"
#include "RocketRoyal/Actors/AirdropActor.h"
#include "RocketRoyal/Utils/Utils.h"

#pragma region Setup
AAirdropManager::AAirdropManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAirdropManager::BeginPlay()
{
	Super::BeginPlay();
	
	AvailableSpawnPoints = SpawnPoints;
	if(ActivateFeature) BindAirdropSpawnTimer();;

	/*for(FVector Point : SpawnPoints)
		DrawDebugBox(GetWorld(), Point, FVector(50), FColor::Orange, true, 1, 0, 20);*/
}

void AAirdropManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
#pragma endregion

#pragma region Update
void AAirdropManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
#pragma endregion 

#pragma region Events
void AAirdropManager::SpawnAirdropEvent()
{
	if(!CanSpawnAirdrops()) return;
	if(!IsValid(AirdropBP))	return;
	
	if(AvailableSpawnPoints.Num() == 0)
	{
		CanSpawn = false;
		return;
	}
	
	const FVector FinalLocation = AvailableSpawnPoints[GetRandomizedSpawnLocation()];
	AvailableSpawnPoints.Remove(FinalLocation);
	
	const FVector RandomizeLocation = FinalLocation + FVector(0, 0, FMath::RandRange(800, 1400));
	AAirdropActor* AirdropActor = Cast<AAirdropActor>(GetWorld()->SpawnActor(AirdropBP, &RandomizeLocation));
	AirdropActor->SetFinalLocation(FinalLocation);
}

bool AAirdropManager::CanSpawnAirdrops() const
{
	return CanSpawn;
}
#pragma endregion 

#pragma region Getters & Setters
int AAirdropManager::GetRandomizedSpawnLocation() const
{
	return FMath::RandRange(0, AvailableSpawnPoints.Num()-1);
}
#pragma endregion

#pragma region Miscs
void AAirdropManager::BindAirdropSpawnTimer()
{
	if(!IsValid(this)) return;
	
	//DebugInfo("Started AirdropManagerTimer");
	FTimerHandle TimerHandle = {};
	FTimerDelegate TimerDelegate = {};
	TimerDelegate.BindLambda([this]{ SpawnAirdropEvent(); });
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, SpawnDelay, true, SpawnDelay);
}
#pragma endregion 