/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "MapComponent.h"

#include "Components/Image.h"

#pragma region Setup
bool UMapComponent::Initialize()
{
	return Super::Initialize();
}

void UMapComponent::NativeConstruct()
{
	Super::NativeConstruct();

	MapPlayButton->SetCursor(EMouseCursor::Hand);

	if(!IsValid(MapTexture)) return;
	MapPreview->SetBrushFromTexture(MapTexture);
} 

void UMapComponent::NativeDestruct()
{
	Super::NativeDestruct();
}
#pragma endregion
