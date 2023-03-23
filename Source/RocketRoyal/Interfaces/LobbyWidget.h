// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
class ALobbyCharacter;

UCLASS()
class ROCKETROYAL_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	
protected:
	UFUNCTION(BlueprintCallable) void StartFunctionAfterTime(float time , ALobbyCharacter* firstChar);
	UFUNCTION(BlueprintImplementableEvent) void StartGame();
	
public:

};
