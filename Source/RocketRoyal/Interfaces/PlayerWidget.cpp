/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "PlayerWidget.h"

#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "RocketRoyal/Gamemode/PersistentGameInstance.h"
#include "RocketRoyal/Utils/Utils.h"

#pragma region Setup
bool UPlayerWidget::Initialize()
{
	return Super::Initialize();
}

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPlayerWidget::SetupScale()
{
	ScaleBoxUI->SetStretch(EStretch::UserSpecified);
	const int RatioDivision = (float)Cast<UPersistentGameInstance>(GetWorld()->GetGameInstance())->nbPlayers;
	float NewScale;
	switch (RatioDivision)
	{
	case 1:
		NewScale = 1.f / RatioDivision;
		break;
	case 2:
		NewScale = 1.3f / RatioDivision;
		ScaleBoxUI->SetRenderTranslation(FVector2D(0,60.f));
		break;
	case 3:
		NewScale = 1.6f / RatioDivision;
		ScaleBoxUI->SetRenderTranslation(FVector2D(0,70.f));
		break;
	case 4:
		NewScale = 2.f / RatioDivision;
		ScaleBoxUI->SetRenderTranslation(FVector2D(0,80.f));
		break;
	default:
		NewScale = 1.f / RatioDivision;
		break;
	}

	ScaleBoxUI->SetUserSpecifiedScale(NewScale);
	//SetPositionInViewport(NewXY);
}
#pragma endregion

#pragma region Events
void UPlayerWidget::UpdateUltimateBarEvent(const float& Amount)
{
	GetUltimateBar()->GetCurrentText()->SetText(FText::AsNumber(Amount));
}
void UPlayerWidget::UpdateHealthBarEvent(const float& Amount)
{
	GetHealthBar()->GetCurrentText()->SetText(FText::AsNumber(Amount));
}
void UPlayerWidget::UpdateKillsEvent(const int& Amount)
{
	GetStats()->UpdateText(E_KILLS, FText::AsNumber(Amount));
}
void UPlayerWidget::UpdateDeathsEvent(const int& Amount)
{
	GetStats()->UpdateText(E_DEATHS, FText::AsNumber(Amount));
}
void UPlayerWidget::UpdateAssistsEvent(const int& Amount)
{
	GetStats()->UpdateText(E_ASSISTS, FText::AsNumber(Amount));
}

void UPlayerWidget::UpdateWeaponEvent(const int& Amount)
{
	GetBulletComponent()->UpdateCurrentBullets(FText::AsNumber(Amount));
}

void UPlayerWidget::UpdateMaxWeaponEvent(const int& Amount)
{
	GetBulletComponent()->UpdateMaxBullets(FText::AsNumber(Amount));
}
#pragma endregion

#pragma region Getter & Setter
UProgressBarComponent* UPlayerWidget::GetHealthBar() const
{
	return IsValid(HealthBarComponent) ? CastChecked<UProgressBarComponent>(HealthBarComponent) : nullptr;
}

UProgressBarComponent* UPlayerWidget::GetUltimateBar() const
{
	return IsValid(UltimateBarComponent) ? CastChecked<UProgressBarComponent>(UltimateBarComponent) : nullptr;
}

UStatsComponent* UPlayerWidget::GetStats() const
{
	return IsValid(StatsComponent) ? CastChecked<UStatsComponent>(StatsComponent) : nullptr;
}

UReloadingWidget* UPlayerWidget::GetReloading() const
{
	return IsValid(ReloadWidget) ? CastChecked<UReloadingWidget>(ReloadWidget) : nullptr;
}

UBulletComponent* UPlayerWidget::GetBulletComponent() const
{
	return IsValid(BulletComponent) ? CastChecked<UBulletComponent>(BulletComponent) : nullptr;
}

#pragma endregion
