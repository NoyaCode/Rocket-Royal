// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketRoyal/Actors/FlagActor.h"
#include "CaptureZoneManager.generated.h"

class ACaptureZone;
UCLASS()
class ROCKETROYAL_API ACaptureZoneManager : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)	TArray<FVector>		      SpawnPositions;
	UPROPERTY(EditAnywhere) TSubclassOf<ACaptureZone> CaptureZoneBP;
	
	UPROPERTY(EditAnywhere) float TimerSpawn = 3;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:	
	ACaptureZoneManager();

	FVector PickSpawnData();
	
	void BindZoneTimer();
	UFUNCTION() void CreateZoneCapture();
};
