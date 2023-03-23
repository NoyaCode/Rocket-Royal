/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReloadingWidget.generated.h"

UCLASS()
class ROCKETROYAL_API UReloadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Reloading UI", meta = (BindWidget)) class UTextBlock*  TextReloading;
	
protected:
	virtual bool Initialize()	   override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;
	
};
