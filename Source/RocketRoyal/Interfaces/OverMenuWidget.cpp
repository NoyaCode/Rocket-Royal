/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

// TODO : Eventually refactor this for general UI template
#include "OverMenuWidget.h"

#pragma region Setup
bool UOverMenuWidget::Initialize()
{
	return Super::Initialize();
}

void UOverMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetQuitButton()->SetText(FText::FromString("Quit Game"));
	GetMenuButton()->SetText(FText::FromString("Game Menu"));
}

void UOverMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
#pragma endregion

#pragma region Misc
void UOverMenuWidget::PlayFadeIn()
{
	if (IsValid(FadeInAnimation)) PlayAnimation(FadeInAnimation, 0, 1);
}

void UOverMenuWidget::PlayFadeOut()
{
	if (IsValid(FadeOutAnimation)) PlayAnimation(FadeOutAnimation, 0, 1);
}
#pragma endregion

#pragma region Getter & Setter
UButtonComponent* UOverMenuWidget::GetQuitButton() const
{
	return IsValid(this->QuitButtonWidget) ? CastChecked<UButtonComponent>(this->QuitButtonWidget) : nullptr;
}

UButtonComponent* UOverMenuWidget::GetMenuButton() const
{
	return IsValid(this->MenuButtonWidget) ? CastChecked<UButtonComponent>(this->MenuButtonWidget) : nullptr;
}

UWidgetAnimation* UOverMenuWidget::GetFadeAnimation(const FString& Name) const
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
