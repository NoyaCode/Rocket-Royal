// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RocketRoyalGameMode.generated.h"

class USoundCue;
UCLASS()
class ROCKETROYAL_API ARocketRoyalGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private :
	
protected:
	UPROPERTY(EditDefaultsOnly,  BlueprintReadWrite, Category = "Sounds") USoundCue* LevelMusic;

	virtual void BeginPlay() override;
	
public :
	UFUNCTION(BlueprintCallable) void StartSplitScreen(bool ShouldStart);

};
