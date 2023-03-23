// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CaptureZoneWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROCKETROYAL_API UCaptureZoneWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget)) class UProgressBar* Bar;

public:
	void SetBarProgress(float Percent) const;
	float GetBarProgress() const;

};
