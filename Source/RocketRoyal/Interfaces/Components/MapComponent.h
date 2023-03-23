/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapComponent.generated.h"

UCLASS()
class ROCKETROYAL_API UMapComponent : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Map UI", meta = (BindWidget)) class UUserWidget* MapPlayButton;
	UPROPERTY(EditAnywhere, Category = "Map UI", meta = (BindWidget)) class UImage*	     MapPreview;

	UPROPERTY(EditAnywhere, Category = "Map UI") class UTexture2D* MapTexture;
protected:
	virtual bool Initialize()      override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;
};
