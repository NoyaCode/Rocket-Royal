// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeLobby.h"

void AGameModeLobby::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameViewport()->SetForceDisableSplitscreen(true);
}
