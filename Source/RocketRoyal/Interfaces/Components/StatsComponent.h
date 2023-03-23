/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatsComponent.generated.h"

UENUM()
enum ESTAT_TYPE
{
	E_KILLS,
	E_DEATHS,
	E_ASSISTS,
};

UCLASS()
class ROCKETROYAL_API UStatsComponent : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Stats UI", meta = (BindWidget)) class UTextBlock* CurrentKills;
    UPROPERTY(EditAnywhere, Category = "Stats UI", meta = (BindWidget)) class UTextBlock* CurrentDeaths;
    UPROPERTY(EditAnywhere, Category = "Stats UI", meta = (BindWidget)) class UTextBlock* CurrentAssists;
	
protected:
	virtual bool Initialize()	   override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;

public:
	UFUNCTION(BlueprintCallable) void SetupStatsComponent(const AActor* Actor);
	
	UFUNCTION(BlueprintCallable) void UpdateText(const ESTAT_TYPE& Type, const FText& Amount);
};
