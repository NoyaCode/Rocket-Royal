// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class ROCKETROYAL_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

private :
	int ControllerID;
public :
	int GetControllerID() const;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) void InitializePauseMenu(int id );
}; 
