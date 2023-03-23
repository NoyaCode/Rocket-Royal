// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MilitaryCharacter.h"
#include "GameFramework/Character.h"
#include "LobbyCharacter.generated.h"

UCLASS()
class ROCKETROYAL_API ALobbyCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	ESpawnTeam CurrentTeam;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
private:

protected:

public:
	UFUNCTION(BlueprintCallable) void SetTeam(const ESpawnTeam& Team);
	UFUNCTION(BlueprintCallable) ESpawnTeam GetTeam() const;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) void SetMeshPlayer();

	// Sets default values for this character's properties
	ALobbyCharacter();
	
	
};
