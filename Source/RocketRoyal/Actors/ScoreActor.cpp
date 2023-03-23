// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreActor.h"

#include "RocketRoyal/Utils/Utils.h"

AScoreActor::AScoreActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AScoreActor::BeginPlay()
{
	Super::BeginPlay();

	ScoreWidget = CreateWidget<UScoreWidget>(GetWorld(), ScoreBP);
	if(IsValid(ScoreWidget))
	{
		DebugInfo("Created Score Widget");
		
		ScoreWidget->AddToViewport();
	}
}

void AScoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UScoreWidget* AScoreActor::GetScoreWidget() const
{
	return ScoreWidget;
}