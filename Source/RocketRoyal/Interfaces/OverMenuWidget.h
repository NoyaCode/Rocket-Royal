/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ButtonComponent.h"
#include "OverMenuWidget.generated.h"

UCLASS()
class ROCKETROYAL_API UOverMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Over UI", meta = (BindWidget)) class UUserWidget* QuitButtonWidget;
	UPROPERTY(EditAnywhere, Category = "Over UI", meta = (BindWidget)) class UUserWidget* MenuButtonWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeInAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeOutAnimation;
	
protected:
	virtual bool Initialize()	   override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;

public:
	UButtonComponent* GetQuitButton() const;
	UButtonComponent* GetMenuButton() const;

	UWidgetAnimation* GetFadeAnimation(const FString& Name) const;
	
	void PlayFadeIn();
	void PlayFadeOut();
};

