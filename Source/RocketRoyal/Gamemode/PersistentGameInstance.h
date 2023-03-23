// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <map>

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <vector>

#include "RocketRoyal/Actors/ScoreActor.h"
#include "RocketRoyal/Actors/SpawnPointActor.h"
#include "RocketRoyal/Actors/TimerActor.h"
#include "RocketRoyal/Interfaces/ScoreWidget.h"
#include "RocketRoyal/Interfaces/Components/TimerComponent.h"
#include "PersistentGameInstance.generated.h"

/* Forward decla */
class AMilitaryCharacter;
class ALobbyCharacter;

struct Tupple
{
	int ControllerId;
	ESpawnTeam PlayerTeam;
};

UCLASS()
class ROCKETROYAL_API UPersistentGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	std::vector<Tupple> TeamCharacters;
	AScoreActor* ScoreActor = nullptr;
	ATimerActor* TimerActor = nullptr;
	
	bool GameFinished = false;

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrays") TArray<AMilitaryCharacter*> MilitaryCharacters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrays") TArray<ALobbyCharacter*> LobbyCharacters;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") TSubclassOf<AScoreActor> ScoreActorBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") TSubclassOf<ATimerActor> TimerActorBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players") int sizeTeamRedLobby ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players") int sizeTeamBlueLobby ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players") int sizeTeamRed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players") int sizeTeamBlue ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Players") int nbPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameLoop") bool WinningTeamBlue;
	
public:
	UFUNCTION(BlueprintCallable)           void UpdateSizeTeams(bool inLobby = false);
	UFUNCTION(BlueprintCallable)           void SetTeamsForCharacters();
	UFUNCTION(BlueprintCallable)		   void PrintTeamCharacters();
	UFUNCTION(BlueprintCallable)		   void ResetGameInstance();
	UFUNCTION(BlueprintImplementableEvent) void StartGame(); 
	UFUNCTION() ESpawnTeam GetWinTeam();
	
	UFUNCTION(BlueprintCallable) void FinishGame(const ESpawnTeam& WinTeam);
	void SetupScoreUI();
	AScoreActor* GetScoreActor() const;

	void SetupTimerUI();
	ATimerActor* GetTimerActor() const;
	
	UFUNCTION(BlueprintCallable) void SetGameFinished(const bool& Condition, const ESpawnTeam& WinTeam);
};
