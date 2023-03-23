/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "FlagActor.h"

#include "RocketRoyal/Player/MilitaryCharacter.h"

AFlagActor::AFlagActor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* EffectRootComponent = CreateDefaultSubobject<USceneComponent>("EffectRoot");
	RootComponent  = EffectRootComponent;
	
	LightEffect = CreateDefaultSubobject<UPointLightComponent>("LightEffect");
	LightEffect->SetupAttachment(RootComponent);
}

void AFlagActor::BeginPlay()
{
	Super::BeginPlay();

	LightEffect->SetLightColor(FColor::White);
}

void AFlagActor::UpdateColorEvent(const FColor& Color) const
{
	LightEffect->SetLightColor(Color);
}
		