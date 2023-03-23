/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "BulletComponent.h"

#include "Components/TextBlock.h"


void UBulletComponent::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBulletComponent::UpdateCurrentBullets(const FText& Text)
{
	CurrentBullets->SetText(Text);
}

void UBulletComponent::UpdateMaxBullets(const FText& Text)
{
	MaxBullets->SetText(Text);
}
