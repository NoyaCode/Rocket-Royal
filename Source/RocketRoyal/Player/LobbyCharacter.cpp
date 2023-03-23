// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyCharacter.h"

#include "RocketRoyal/Utils/Utils.h"

// Sets default values
ALobbyCharacter::ALobbyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALobbyCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentTeam = E_BLUE;
}

void ALobbyCharacter::SetTeam(const ESpawnTeam& Team)
{
	CurrentTeam = Team;

	//DebugData("Player Team %hs", Team == E_RED ? "Red" : "Blue");
}

ESpawnTeam ALobbyCharacter::GetTeam() const
{
	return CurrentTeam;
}



