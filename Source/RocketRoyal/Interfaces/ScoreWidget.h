/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBarComponent.h"
#include "RocketRoyal/Actors/SpawnPointActor.h"
#include "ScoreWidget.generated.h"

UCLASS()
class ROCKETROYAL_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	int MaxValue  = 30;
	int BlueValue = 0;
	int RedValue  = 0;
	
public:
	UPROPERTY(EditAnywhere, Category = "Player UI", meta = (BindWidget)) class UUserWidget* BlueBarComponent;
	UPROPERTY(EditAnywhere, Category = "Player UI", meta = (BindWidget)) class UUserWidget* RedBarComponent;

protected:
	virtual bool Initialize()	   override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;

public:
	UProgressBarComponent* GetBlueBar() const;
	UProgressBarComponent* GetRedBar()  const;

	void UpdateBlueBar(const float& Amount) const;
	void UpdateRedBar (const float& Amount) const;

	float GetBlueValue() const;
	float GetRedValue()  const;

	float UpdateValue(const ESpawnTeam& Team, const float& Value);
};