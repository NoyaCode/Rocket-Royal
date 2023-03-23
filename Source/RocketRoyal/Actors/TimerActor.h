/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerActor.generated.h"

class UTimerComponent;
UCLASS()
class ROCKETROYAL_API ATimerActor : public AActor
{
	GENERATED_BODY()

private:
	int DecMin;
	int DecSec;

	UTimerComponent* TimerComponent = nullptr;
	
protected:
	UPROPERTY(EditAnywhere) int Minutes = 2;
	UPROPERTY(EditAnywhere) int Seconds = 0;

	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> TimerBP;

private:
	void BindTimer();
	void Countdown();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:	
	ATimerActor();

	UTimerComponent* GetTimerComponent() const;
};
