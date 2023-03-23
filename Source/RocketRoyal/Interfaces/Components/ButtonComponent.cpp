/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "ButtonComponent.h"

#include "Components/Button.h"

#pragma region Setup
bool UButtonComponent::Initialize()
{
	return Super::Initialize();
}

void UButtonComponent::NativeConstruct()
{
	Super::NativeConstruct();

	Button->SetCursor(EMouseCursor::Hand);
}

void UButtonComponent::NativeDestruct()
{
	Super::NativeDestruct();
}
#pragma endregion



