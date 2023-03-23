// Fill out your copyright notice in the Description page of Project Settings.


#include "PersistentGameInstance.h"

#include "RocketRoyalGameMode.h"
#include "RocketRoyal/Utils/Utils.h"
#include "Kismet/GameplayStatics.h"
#include "RocketRoyal/Player/LobbyCharacter.h"
#include "Kismet/GameplayStatics.h"


void UPersistentGameInstance::UpdateSizeTeams(bool inLobby)
{
	nbPlayers = 0;

	if(inLobby)
	{
		TeamCharacters.clear();
		if(LobbyCharacters.Num() <= 0) return;
		
		if(!IsValid(LobbyCharacters[0]))
		{
			//DebugError("No lobby characters abort");
			return;
		}
			
		sizeTeamBlueLobby = 0;
		sizeTeamRedLobby = 0;
		
		for (ALobbyCharacter* lobbyCh : LobbyCharacters)
		{
			if(lobbyCh->GetTeam() == ESpawnTeam::E_BLUE)
				sizeTeamBlueLobby++;
			else if(lobbyCh->GetTeam() == ESpawnTeam::E_RED)
				sizeTeamRedLobby++;

			nbPlayers++;
			int ControllerID = UGameplayStatics::GetPlayerControllerID(Cast<APlayerController>(lobbyCh->GetController()));
			
			TeamCharacters.push_back(Tupple {ControllerID, lobbyCh->GetTeam()});
		}

		//DebugData("size Blue L: %d", sizeTeamBlueLobby);
		//DebugData("size Red  L: %d", sizeTeamRedLobby);
	}
	else
	{

		if(MilitaryCharacters.Num() == 0) return;
		
		if(MilitaryCharacters[0] == nullptr)
		{
			//DebugError("No characters abort");
			return;
		}
		
		sizeTeamBlue = 0;
		sizeTeamRed = 0;
		
		for (AMilitaryCharacter* playerCh : MilitaryCharacters)
		{
			if(playerCh->GetTeam() == ESpawnTeam::E_BLUE)
				sizeTeamBlue++;
			else if(playerCh->GetTeam() == ESpawnTeam::E_RED)
				sizeTeamRed++;

			nbPlayers++;
		}

		
		//Debug("size Blue : %d", sizeTeamBlue);
		//Debug("size Red  : %d", sizeTeamRed );
	}

	/*for (Tupple t : TeamCharacters)
	{
		//Debug("================");
		//Debug("%d  , %hs", t.ControllerId , t.PlayerTeam == E_RED ? "Red" : "Blue")
	}*/

}

void UPersistentGameInstance::SetTeamsForCharacters()
{
	for (AMilitaryCharacter* PlayerChar : MilitaryCharacters)
	{
		int ControllerID = UGameplayStatics::GetPlayerControllerID(Cast<APlayerController>(PlayerChar->GetController()));

		int i = 0;
		while(TeamCharacters[i].ControllerId != ControllerID)
		{
			i++;
			if(i > 4)
			{
				//DebugError("No Controller founded abort ");
				return;
			}
		}
		//Debug("Set player id : %d in team : %hs", ControllerID ,  TeamCharacters[i].PlayerTeam == E_RED ? "Red" : "Blue")
		PlayerChar->SetTeam(TeamCharacters[i].PlayerTeam);
		//TeamCharacters.erase(TeamCharacters.begin() + i);
	}
	
}

void UPersistentGameInstance::PrintTeamCharacters()
{

	/*Debug("Print");
	for (Tupple t : TeamCharacters)
	{
		Debug("================");
		Debug("%d  , %hs", t.ControllerId , t.PlayerTeam == E_RED ? "Red" : "Blue")
	}*/
}

void UPersistentGameInstance::ResetGameInstance()
{
	TeamCharacters.clear();
	ScoreActor = nullptr;
	TimerActor = nullptr;
	GameFinished = false;

	MilitaryCharacters.Empty();
	LobbyCharacters.Empty();
	sizeTeamBlue = 0;
	sizeTeamRed = 0;
	sizeTeamBlueLobby = 0;
	sizeTeamRedLobby = 0;
	
	GetWorld()->SetGameInstance(NewObject<UPersistentGameInstance>());
}

ESpawnTeam UPersistentGameInstance::GetWinTeam()
{
	int b = ScoreActor->GetScoreWidget()->GetBlueValue();
	int r = ScoreActor->GetScoreWidget()->GetRedValue();
	
	if(b > r)
		return ESpawnTeam::E_BLUE;
	else
		return ESpawnTeam::E_RED;
}

void UPersistentGameInstance::FinishGame(const ESpawnTeam& WinTeam)
{
	SetGameFinished(true, WinTeam);
}

void UPersistentGameInstance::SetupScoreUI()
{
	ScoreActor = GetWorld()->SpawnActor<AScoreActor>(ScoreActorBP);
}

AScoreActor* UPersistentGameInstance::GetScoreActor() const
{
	return ScoreActor;
}

void UPersistentGameInstance::SetupTimerUI()
{
	TimerActor = GetWorld()->SpawnActor<ATimerActor>(TimerActorBP);
}

ATimerActor* UPersistentGameInstance::GetTimerActor() const
{
	return TimerActor;
}

void UPersistentGameInstance::SetGameFinished(const bool& Condition, const ESpawnTeam& WinTeam)
{
	GameFinished = Condition;

	if(GameFinished)
	{
		WinningTeamBlue = WinTeam == E_BLUE ? true : false;
		StartGame();
	}
}