/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "ScoreWidget.h"

#include "RocketRoyal/Actors/SpawnPointActor.h"
#include "RocketRoyal/Gamemode/PersistentGameInstance.h"
#include "RocketRoyal/Utils/Utils.h"

#pragma region Setup
bool UScoreWidget::Initialize()
{
	return Super::Initialize();
}

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetBlueBar()->SetupComponent(0, MaxValue, FColor::Blue);
	GetRedBar()->SetupComponent(0,  MaxValue, FColor::Red);
}

void UScoreWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

#pragma endregion

#pragma region Events
void UScoreWidget::UpdateBlueBar(const float& Amount) const
{
	GetBlueBar()->UpdateProgressBar(Amount, MaxValue);
	
	if (Amount >= MaxValue)
	{
		DebugInfo("Blue team win!");
		Cast<UPersistentGameInstance>(GetWorld()->GetGameInstance())->FinishGame(E_BLUE);
	}
}

void UScoreWidget::UpdateRedBar(const float& Amount) const
{
	GetRedBar()->UpdateProgressBar(Amount, MaxValue);
	if (Amount >= MaxValue)
	{
		DebugInfo("Red team win!");
		Cast<UPersistentGameInstance>(GetWorld()->GetGameInstance())->FinishGame(E_RED);
	}
}

#pragma endregion

#pragma region Getters & Setters
float UScoreWidget::GetRedValue() const
{
	return RedValue;
}

float UScoreWidget::GetBlueValue() const
{
	return BlueValue;
}

UProgressBarComponent* UScoreWidget::GetBlueBar() const
{
	return IsValid(BlueBarComponent) ? CastChecked<UProgressBarComponent>(BlueBarComponent) : nullptr;
}

UProgressBarComponent* UScoreWidget::GetRedBar() const
{
	return IsValid(RedBarComponent) ? CastChecked<UProgressBarComponent>(RedBarComponent) : nullptr;
}

float UScoreWidget::UpdateValue(const ESpawnTeam& Team, const float& Value)
{
	switch (Team)
	{
		case E_RED:  return RedValue += Value;
		case E_BLUE: return BlueValue += Value;
		default:	 return -1;
	}
}

#pragma endregion
