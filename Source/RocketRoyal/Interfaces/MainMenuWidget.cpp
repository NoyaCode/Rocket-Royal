/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

// TODO : Eventually refactor this for general UI template
#include "MainMenuWidget.h"

#pragma region Setup
bool UMainMenuWidget::Initialize()
{
	return Super::Initialize();
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetPlayButton()->SetText(FText::FromString("Start Game"));
	GetQuitButton()->SetText(FText::FromString("Quit Game"));
}

void UMainMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
#pragma endregion

#pragma region Misc
void UMainMenuWidget::PlayFadeIn()
{
	if (IsValid(FadeInAnimation)) PlayAnimation(FadeInAnimation, 0, 1);
}

void UMainMenuWidget::PlayFadeOut()
{
	if (IsValid(FadeOutAnimation)) PlayAnimation(FadeOutAnimation, 0, 1);
}
#pragma endregion

#pragma region Getter & Setter
UButtonComponent* UMainMenuWidget::GetPlayButton() const
{
	return IsValid(PlayButtonWidget) ? CastChecked<UButtonComponent>(PlayButtonWidget) : nullptr;
}

UButtonComponent* UMainMenuWidget::GetQuitButton() const
{
	return IsValid(QuitButtonWidget) ? CastChecked<UButtonComponent>(QuitButtonWidget) : nullptr;
}

UWidgetAnimation* UMainMenuWidget::GetFadeAnimation(const FString& Name) const
{
	if (Name != "OUT" || Name != "IN") return nullptr;
	const int Index = Name == "IN" ? 0 : 1;

	switch (Index)
	{
		case 0: return IsValid(FadeInAnimation) ? FadeInAnimation : nullptr;
		case 1: return IsValid(FadeInAnimation) ? FadeInAnimation : nullptr;
		default: return nullptr;
	}
}
#pragma endregion
