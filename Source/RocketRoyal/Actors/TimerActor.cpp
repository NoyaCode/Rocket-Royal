/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */


#include "TimerActor.h"

#include "RocketRoyal/Gamemode/PersistentGameInstance.h"
#include "RocketRoyal/Interfaces/Components/TimerComponent.h"
#include "RocketRoyal/Utils/Utils.h"

ATimerActor::ATimerActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

UTimerComponent* ATimerActor::GetTimerComponent() const
{
	return TimerComponent;
}

void ATimerActor::BindTimer()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATimerActor::Countdown, 1.f, true, 0.0f);
}

void ATimerActor::BeginPlay()
{
	Super::BeginPlay();

	DecSec = Seconds;
	DecMin = Minutes;

	TimerComponent = CreateWidget<UTimerComponent>(GetWorld(), TimerBP);
	if(IsValid(TimerComponent))
	{
		DebugInfo("Created Timer Widget");
		TimerComponent->UpdateMinutes(FText::AsNumber(Minutes));
		TimerComponent->UpdateSeconds(FText::AsNumber(Seconds));
		
		TimerComponent->AddToViewport();
	}
	
	BindTimer();
}

void ATimerActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ATimerActor::Countdown()
{
	if(DecSec != 0)
	{
		DecSec -= 1;
	}
	else if (DecMin == 0)
	{
		UPersistentGameInstance* PersistentGameInst = Cast<UPersistentGameInstance>(GetWorld()->GetGameInstance());
		PersistentGameInst->SetGameFinished(true, PersistentGameInst->GetWinTeam());
		return;
	}
	else if(DecMin != 0) {
		DecMin -= 1;
		DecSec = 59;
	}
	
	const FString MinuteVal = FString::FromInt(DecMin);
	FString FinalMinuteVal;
	
	if(DecMin < 10) FinalMinuteVal = FString::FromInt(0) + MinuteVal;
	else FinalMinuteVal = MinuteVal;
	
	TimerComponent->UpdateMinutes(FText::FromString(FinalMinuteVal));

	const FString SecondVal = FString::FromInt(DecSec);
	FString FinalSecondVal;
	
	if(DecSec < 10) FinalSecondVal = FString::FromInt(0) + SecondVal;
	else FinalSecondVal = SecondVal;
	
	TimerComponent->UpdateSeconds(FText::FromString(FinalSecondVal));
}
