/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BulletComponent.generated.h"

UCLASS()
class ROCKETROYAL_API UBulletComponent : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Weapon UI", meta = (BindWidget)) class UTextBlock* CurrentBullets;
	UPROPERTY(EditAnywhere, Category = "Weapon UI", meta = (BindWidget)) class UTextBlock* MaxBullets;

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateCurrentBullets(const FText& Text);
	void UpdateMaxBullets    (const FText& Text);
};
