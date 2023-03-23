/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "NiagaraComponent.h"
#include "CoreMinimal.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Actor.h"
#include "FlagActor.generated.h"

class AMilitaryCharacter;
UCLASS()
class ROCKETROYAL_API AFlagActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "VFX") UPointLightComponent* LightEffect		 = nullptr;

protected:
	virtual void BeginPlay() override;

public:	
	AFlagActor();

	void UpdateColorEvent(const FColor& Color) const;
};
