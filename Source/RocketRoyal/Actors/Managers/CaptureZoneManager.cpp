// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureZoneManager.h"

#include "DrawDebugHelpers.h"
#include "../CaptureZone.h"
#include "RocketRoyal/Utils/Utils.h"

ACaptureZoneManager::ACaptureZoneManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACaptureZoneManager::BeginPlay()
{
	Super::BeginPlay();
	
	BindZoneTimer();

	/*for(FVector Point : AvailableSpawnPositions)
		DrawDebugBox(GetWorld(), Point, FVector(50), FColor::Orange, true, 1, 0, 20);*/
}

FVector ACaptureZoneManager::PickSpawnData()
{
	//DebugInfo("NB FVector : %d", AvailableSpawnPositions.Num());
	if(SpawnPositions.Num() <= 0) return FVector();
	
	const int Index = FMath::RandRange(0, SpawnPositions.Num()-1);
	const FVector Data = SpawnPositions[Index];
	//AvailableSpawnPositions.RemoveAt(Index);
	return Data;
}

void ACaptureZoneManager::CreateZoneCapture()
{
	const FVector Data = PickSpawnData();
	//DebugInfo("Zone Pos x: %f Y: %f Z: %f", Data.X, Data.Y, Data.Z);
	ACaptureZone* ZoneCreated = GetWorld()->SpawnActor<ACaptureZone>(CaptureZoneBP, FVector(Data.X, Data.Y, Data.Z+210), FRotator(0));
	if (IsValid(ZoneCreated))
		ZoneCreated->SetCaptureManager(this);
}

void ACaptureZoneManager::BindZoneTimer()
{
	//DebugInfo("Started CaptureZoneSpawnTimer");
	FTimerHandle _;
	GetWorldTimerManager().SetTimer(_, this, &ACaptureZoneManager::CreateZoneCapture, TimerSpawn, false);
}

void ACaptureZoneManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

