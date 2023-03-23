/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ButtonComponent.generated.h"

UCLASS()
class ROCKETROYAL_API UButtonComponent : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Bouton UI", meta = (BindWidget)) class UTextBlock* DisplayedText;
	UPROPERTY(EditAnywhere, Category = "Bouton UI", meta = (BindWidget)) class UButton*    Button;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	FORCEINLINE void SetText(const FText& Value) const { if (IsValid(DisplayedText)) DisplayedText->SetText(Value); }
	FORCEINLINE UButton* GetButton() const { return IsValid(Button) ? Button : nullptr; }
};
