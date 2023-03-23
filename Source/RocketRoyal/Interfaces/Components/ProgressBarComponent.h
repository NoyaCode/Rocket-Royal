/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProgressBarComponent.generated.h"

UCLASS()
class ROCKETROYAL_API UProgressBarComponent : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Bar UI", meta = (BindWidget)) class UProgressBar* ProgressBar;
	UPROPERTY(EditAnywhere, Category = "Bar UI", meta = (BindWidget)) class UTextBlock* CurrentText;
	UPROPERTY(EditAnywhere, Category = "Bar UI", meta = (BindWidget)) class UTextBlock* TotalText;
    
protected:
	virtual bool Initialize()	   override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;

public:
	UFUNCTION(BlueprintCallable) void SetupComponent(const float& CurrentValue, const float& MaxValue, const FColor& Color);

	UProgressBar* GetProgressBar() const;
	UTextBlock*   GetCurrentText() const;
	UTextBlock*	  GetTotalText()   const;

	void UpdateProgressBar(const float& CurrentValue, const float& MaxValue);
	void SetCurrentText	  (const FText& Text);
	void SetTotalText     (const FText& Text);
};
