// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "RocketRoyal/Player/LobbyCharacter.h"

void ULobbyWidget::StartFunctionAfterTime(float time , ALobbyCharacter* firstChar)
{
	FTimerHandle TimerHandle	 = {};
	FTimerDelegate TimerDelegate = {};
	TimerDelegate.BindUFunction(this, FName("StartGame"), true);

	if(firstChar)
	{
		firstChar->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false, time);
	}

}
