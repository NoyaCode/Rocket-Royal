/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

// TODO : Eventually refactor this for general UI template
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ButtonComponent.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class ROCKETROYAL_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
	UPROPERTY(EditAnywhere, Category = "MainMenu UI", meta = (BindWidget)) class UUserWidget* PlayButtonWidget;
	UPROPERTY(EditAnywhere, Category = "MainMenu UI", meta = (BindWidget)) class UUserWidget* QuitButtonWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeInAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeOutAnimation;
	
protected:
	virtual bool Initialize()	   override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct()  override;

public:
	UButtonComponent* GetPlayButton() const;
	UButtonComponent* GetQuitButton() const;

	UWidgetAnimation* GetFadeAnimation(const FString& Name) const;
	
	void PlayFadeIn();
	void PlayFadeOut();
};

