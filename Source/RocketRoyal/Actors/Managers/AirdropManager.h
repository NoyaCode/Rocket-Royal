// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AirdropManager.generated.h"

UCLASS()
class ROCKETROYAL_API AAirdropManager : public AActor
{
	GENERATED_BODY()

private:
	TArray<FVector> AvailableSpawnPoints;

	bool CanSpawn = true;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirdropManager Properties") TArray<FVector>     SpawnPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirdropManager Properties") TSubclassOf<AActor> AirdropBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirdropManager Properties") bool				ActivateFeature = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirdropManager Properties") float SpawnDelay = 50.0f;
protected:
	virtual void BeginPlay              ()									       override;
	virtual void EndPlay                (const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick                   (float DeltaSeconds)					   override;

private:
	int GetRandomizedSpawnLocation() const;
	void BindAirdropSpawnTimer();

public:	
	AAirdropManager();

	void SpawnAirdropEvent();

	bool CanSpawnAirdrops() const;
};
