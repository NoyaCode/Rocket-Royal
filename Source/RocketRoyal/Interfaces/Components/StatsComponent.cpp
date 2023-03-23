/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "StatsComponent.h"

#include "Components/TextBlock.h"
#include "RocketRoyal/Player/MilitaryCharacter.h"

#pragma region Setup
bool UStatsComponent::Initialize()
{
	return Super::Initialize();
}

void UStatsComponent::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStatsComponent::NativeDestruct()
{
	Super::NativeDestruct();
}

void UStatsComponent::SetupStatsComponent(const AActor* Actor)
{
	if (IsValid(Actor)) {
		const AMilitaryCharacter* Character = Cast<AMilitaryCharacter>(Actor);
		
		UpdateText(E_KILLS, FText::AsNumber(Character->GetKills()));
		UpdateText(E_DEATHS ,FText::AsNumber(Character->GetDeaths()));
		UpdateText(E_ASSISTS , FText::AsNumber(Character->GetAssists()));
	}
}
#pragma endregion

#pragma region Events
void UStatsComponent::UpdateText(const ESTAT_TYPE& Type, const FText& Amount)
{
	switch (Type)
	{
		case E_KILLS:	if (IsValid(CurrentKills))   CurrentKills->SetText(Amount);   break;
		case E_DEATHS:  if (IsValid(CurrentDeaths))  CurrentDeaths->SetText(Amount);	break;
		case E_ASSISTS: if (IsValid(CurrentAssists)) CurrentAssists->SetText(Amount); break;
		default:																		break;
	}
}
#pragma endregion
