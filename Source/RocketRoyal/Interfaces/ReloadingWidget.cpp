/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "ReloadingWidget.h"

bool UReloadingWidget::Initialize()
{
	return Super::Initialize();
}

void UReloadingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetVisibility(ESlateVisibility::Hidden);
}

void UReloadingWidget::NativeDestruct()
{
	Super::NativeDestruct();
}