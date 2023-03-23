/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapComponent.generated.h"

UCLASS()
class ROCKETROYAL_API UMinimapComponent : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Minimap UI", meta = (BindWidget)) class UImage* MapImage;

protected:
	virtual bool Initialize()	   override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;

public:
	UFUNCTION(BlueprintCallable) void SetupMinimapComponent(const AActor* Actor);
};
