/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "ProgressBarComponent.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "RocketRoyal/Gamemode/PersistentGameInstance.h"
#include "RocketRoyal/Player/MilitaryCharacter.h"

#pragma region Setup
bool UProgressBarComponent::Initialize()
{
	return Super::Initialize();
}

void UProgressBarComponent::NativeConstruct()
{
	Super::NativeConstruct();
}

void UProgressBarComponent::NativeDestruct()
{
	Super::NativeDestruct();
}

void UProgressBarComponent::SetupComponent(const float& CurrentValue, const float& MaxValue, const FColor& Color)
{
	if(IsValid(CurrentText)) CurrentText->SetText(FText::AsNumber(CurrentValue));
	if(IsValid(TotalText))   TotalText  ->SetText(FText::AsNumber(MaxValue));
	if(IsValid(ProgressBar))
	{
		ProgressBar->SetPercent(CurrentValue/MaxValue);
		ProgressBar->SetFillColorAndOpacity(Color);
	}
}
#pragma endregion

#pragma region Getter & Setter
UProgressBar* UProgressBarComponent::GetProgressBar() const
{
	return IsValid(ProgressBar) ? ProgressBar : nullptr;
}

UTextBlock* UProgressBarComponent::GetCurrentText() const
{
	return IsValid(CurrentText) ? CurrentText : nullptr;
}

UTextBlock* UProgressBarComponent::GetTotalText() const
{
	return IsValid(TotalText) ? TotalText : nullptr;
}

void UProgressBarComponent::UpdateProgressBar(const float& CurrentValue, const float& MaxValue)
{
	ProgressBar->SetPercent(CurrentValue/MaxValue);
	SetCurrentText(FText::AsNumber((int)CurrentValue));
}

void UProgressBarComponent::SetCurrentText(const FText& Text)
{
	CurrentText->SetText(Text);
}

void UProgressBarComponent::SetTotalText(const FText& Text)
{
	TotalText->SetText(Text);
}
#pragma endregion
