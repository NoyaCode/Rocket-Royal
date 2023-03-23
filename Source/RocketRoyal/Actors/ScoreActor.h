// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketRoyal/Interfaces/ScoreWidget.h"
#include "ScoreActor.generated.h"

UCLASS()
class ROCKETROYAL_API AScoreActor : public AActor
{
	GENERATED_BODY()

private:
	UScoreWidget* ScoreWidget = nullptr;
	
protected:
	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> ScoreBP;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	AScoreActor();

	UScoreWidget* GetScoreWidget() const;
};
