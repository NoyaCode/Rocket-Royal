// Copyright Epic Games, Inc. All Rights Reserved.

#include "RocketRoyalGameMode.h"

#include "PersistentGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "RocketRoyal/Utils/Utils.h"

void ARocketRoyalGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if(LevelMusic)
		UGameplayStatics::PlaySound2D(GetWorld(), LevelMusic);

	UPersistentGameInstance* Instance =  Cast<UPersistentGameInstance>(GetWorld()->GetGameInstance());

	if(IsValid(Instance))
	{
		Instance->SetupScoreUI();
		Instance->SetupTimerUI();
	}
}

void ARocketRoyalGameModeBase::StartSplitScreen(bool shouldStart)
{
	if(GetWorld()->GetGameViewport())
		GetWorld()->GetGameViewport()->SetForceDisableSplitscreen(!shouldStart);
	else
		DebugError("No Game ViewPort");
}
