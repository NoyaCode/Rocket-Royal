/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "MapSelectorWidget.h"

#include "Kismet/GameplayStatics.h"

#pragma region Setup
bool UMapSelectorWidget::Initialize()
{
	return Super::Initialize();
}

void UMapSelectorWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapSelectorWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
#pragma endregion

#pragma region Miscs
void UMapSelectorWidget::LoadLevel(const FName& Name)
{
	UGameplayStatics::OpenLevel(this, Name);
}
#pragma endregion 
