/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelectorWidget.generated.h"

UCLASS()
class ROCKETROYAL_API UMapSelectorWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Selector UI", meta = (BindWidget)) class UUserWidget* MapButton;

private:
	void LoadLevel(const FName& Name);
	
protected:
	virtual bool Initialize		() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct () override;
};
