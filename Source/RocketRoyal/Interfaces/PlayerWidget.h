/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "ReloadingWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBarComponent.h"
#include "Components/StatsComponent.h"
#include "Components/BulletComponent.h"
#include "PlayerWidget.generated.h"

UCLASS()
class ROCKETROYAL_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Player UI", meta = (BindWidget)) class UUserWidget* HealthBarComponent;
	UPROPERTY(EditAnywhere, Category = "Player UI", meta = (BindWidget)) class UUserWidget* UltimateBarComponent;
	UPROPERTY(EditAnywhere, Category = "Player UI", meta = (BindWidget)) class UUserWidget* StatsComponent;
	UPROPERTY(EditAnywhere, Category = "Player UI", meta = (BindWidget)) class UUserWidget* ReloadWidget;
	UPROPERTY(EditAnywhere, Category = "Player UI", meta = (BindWidget)) class UUserWidget* BulletComponent;
	UPROPERTY(EditAnywhere, Category = "Player UI", meta = (BindWidget)) class UScaleBox*   ScaleBoxUI;
	
protected:
	virtual bool Initialize()	   override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;

public:
	void SetupScale();
	
	UProgressBarComponent* GetHealthBar()		const;
	UProgressBarComponent* GetUltimateBar()		const;
	UStatsComponent*	   GetStats()			const;
	UReloadingWidget*	   GetReloading()		const;
	UBulletComponent*	   GetBulletComponent() const;
	
	void UpdateUltimateBarEvent(const float& Amount);
	void UpdateHealthBarEvent  (const float& Amount);
	void UpdateKillsEvent      (const int& Amount);
	void UpdateDeathsEvent     (const int& Amount);
	void UpdateAssistsEvent    (const int& Amount);
	void UpdateWeaponEvent	   (const int& Amount);
	void UpdateMaxWeaponEvent  (const int& Amount);
};
