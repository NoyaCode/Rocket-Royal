/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathZoneActor.generated.h"

class AMilitaryCharacter;
UENUM()
enum EZoneState
{
	E_First	 = 2 UMETA(DisplayName = "1"),
	E_Second = 4 UMETA(DisplayName = "2"),
	E_Third	 = 6 UMETA(DisplayName = "3"),
	E_Fourth = 8 UMETA(DisplayName = "4")
};

UENUM()
enum EZoneDirection
{
	E_North UMETA(DisplayName = "North"),
	E_South UMETA(DisplayName = "South"),
	E_West  UMETA(DisplayName = "West"),
	E_East  UMETA(DisplayName = "East"),
};

UCLASS()
class ROCKETROYAL_API ADeathZoneActor : public AActor
{
	GENERATED_BODY()

private:
	float CurrentAppliedDamage = 0.f;

	TArray<AMilitaryCharacter*> PlayersInZone;
	
	EZoneState       ZoneState		   = E_First;
	USceneComponent* ZoneRootComponent = nullptr;
	FVector 		 InitialPosition   = FVector(0);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Properties") TEnumAsByte<EZoneDirection> ZoneDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Properties") float ReducingRate		 = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Properties") float InitialAppliedDamage = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Properties") float DamageRate			 = 0.0025f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Properties") bool  ShouldMove		     = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Physics") UShapeComponent* ZoneShapeComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Render") UStaticMeshComponent* ZoneMeshComponent = nullptr;
	
protected:
	virtual void BeginPlay              ()										   override;
	virtual void EndPlay                (const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick                   (float DeltaSeconds)					   override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)					   override;
	virtual void NotifyActorEndOverlap  (AActor* OtherActor)					   override;
	
public:
	ADeathZoneActor();

	bool ShouldZoneReduce() const;
	
	void CalcZoneState  ();
	void CalcZoneDamages(const float& DeltaSeconds);
	void ReduceZoneEvent(const float& DeltaSeconds);

	EZoneState GetZoneState    () const;
	EZoneState GetNextZoneState() const;
	
	void SetZoneState     (const EZoneState& State);
	void GoToNextZoneState();
	
	EZoneDirection GetZoneDirection() const;
};
