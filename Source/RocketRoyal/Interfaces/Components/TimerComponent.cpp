/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "TimerComponent.h"

#include "Components/TextBlock.h"
#include "RocketRoyal/Gamemode/PersistentGameInstance.h"

void UTimerComponent::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTimerComponent::UpdateMinutes(const FText& Text)
{
	Minutes->SetText(Text);
}

void UTimerComponent::UpdateSeconds(const FText& Text)
{
	Seconds->SetText(Text);
}

