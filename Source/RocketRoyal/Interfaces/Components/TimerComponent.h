/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerComponent.generated.h"

UCLASS()
class ROCKETROYAL_API UTimerComponent : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Timer UI", meta = (BindWidget)) class UTextBlock* Minutes;
	UPROPERTY(EditAnywhere, Category = "Timer UI", meta = (BindWidget)) class UTextBlock* Seconds;

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateMinutes(const FText& Text);
	void UpdateSeconds(const FText& Text);
};
