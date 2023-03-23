/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "DeathZoneActor.h"

#include "Components/BoxComponent.h"
#include "RocketRoyal/Player/MilitaryCharacter.h"
#include "RocketRoyal/Utils/Utils.h"

#pragma region Setup
ADeathZoneActor::ADeathZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ZoneRootComponent = CreateDefaultSubobject<USceneComponent>("WeaponRoot");
	RootComponent = ZoneRootComponent;
	
	ZoneMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ZoneMesh");
	ZoneMeshComponent->SetupAttachment(RootComponent);
	
	ZoneShapeComponent = CreateDefaultSubobject<UBoxComponent>("ZoneBox");
	ZoneShapeComponent->SetupAttachment(RootComponent);

	Tags.Add("Zone");
}

void ADeathZoneActor::BeginPlay()
{
	Super::BeginPlay();

	CurrentAppliedDamage = InitialAppliedDamage;
	
	ZoneMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	ZoneShapeComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	ZoneShapeComponent->SetGenerateOverlapEvents(true);
	
	InitialPosition = ZoneRootComponent->GetRelativeLocation();
}

void ADeathZoneActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
#pragma endregion

#pragma region Update
void ADeathZoneActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(!ShouldZoneReduce()) return;
	if(ShouldMove) ReduceZoneEvent(DeltaSeconds);
	
	for(int i = 0; i < PlayersInZone.Num(); i++)
	{
		if(!IsValid(PlayersInZone[i])) { PlayersInZone.Remove(PlayersInZone[i]); return; }
		PlayersInZone[i]->TakeDamageEvent(nullptr, CurrentAppliedDamage * 0.1f);
	}
	
}

void ADeathZoneActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(!OtherActor->Tags.Contains("Player")) return;
	PlayersInZone.Add(Cast<AMilitaryCharacter>(OtherActor));

	Super::NotifyActorBeginOverlap(OtherActor);
}

void ADeathZoneActor::NotifyActorEndOverlap(AActor* OtherActor)
{
	if(!OtherActor->Tags.Contains("Player")) return;
	PlayersInZone.Remove(Cast<AMilitaryCharacter>(OtherActor));
	
	Super::NotifyActorEndOverlap(OtherActor);
}
#pragma endregion

#pragma region Events
void ADeathZoneActor::ReduceZoneEvent(const float& DeltaSeconds)
{
	const FVector CurrentLocation = ZoneRootComponent->GetRelativeLocation();
	FVector NewLocation = CurrentLocation;
	
	const FVector CurrentScale = ZoneRootComponent->GetRelativeScale3D();
	FVector NewScale = CurrentScale;
	
	switch (GetZoneDirection())
	{
		case E_East:  NewLocation.X += DeltaSeconds * ReducingRate; break;
		case E_West:  NewLocation.X -= DeltaSeconds * ReducingRate; break;
		case E_North: NewLocation.Y -= DeltaSeconds * ReducingRate; break;
		case E_South: NewLocation.Y += DeltaSeconds * ReducingRate; break;
		default:													break;
	}

	NewScale.X -= DeltaSeconds * ReducingRate * 0.02f;
	
	ZoneRootComponent->SetRelativeLocation(NewLocation);
	ZoneRootComponent->SetRelativeScale3D(NewScale);
	
	CalcZoneState();
	CalcZoneDamages(DeltaSeconds);
}
#pragma endregion

#pragma region Miscs
void ADeathZoneActor::CalcZoneDamages(const float& DeltaSeconds)
{
	switch (GetZoneState())
	{
		case E_First:
			CurrentAppliedDamage += DamageRate * DeltaSeconds * 0.8; break;
		case E_Second:
			CurrentAppliedDamage += DamageRate * DeltaSeconds * 1.2; break;
		case E_Third:
			CurrentAppliedDamage += DamageRate * DeltaSeconds * 1.4; break;
		case E_Fourth:
			CurrentAppliedDamage += DamageRate * DeltaSeconds * 1.6; break;
		default: break;
	}
}

void ADeathZoneActor::CalcZoneState()
{
	const FVector CurrentPosition = GetActorLocation();
	
	switch (GetZoneDirection())
	{
		case E_East:  if(CurrentPosition.X >= InitialPosition.X/GetZoneState() && CurrentPosition.X <= InitialPosition.X/GetNextZoneState()) { GoToNextZoneState(); } break;
		case E_West:  if(CurrentPosition.X <= InitialPosition.X/GetZoneState() && CurrentPosition.X >= InitialPosition.X/GetNextZoneState()) { GoToNextZoneState(); } break;
		case E_North: if(CurrentPosition.Y <= InitialPosition.Y/GetZoneState() && CurrentPosition.Y >= InitialPosition.Y/GetNextZoneState()) { GoToNextZoneState(); } break;
		case E_South: if(CurrentPosition.Y >= InitialPosition.Y/GetZoneState() && CurrentPosition.Y <= InitialPosition.Y/GetNextZoneState()) { GoToNextZoneState(); } break;
		default: break;
	}
}
#pragma endregion

#pragma region Getter & Setter
bool ADeathZoneActor::ShouldZoneReduce() const
{
	return ZoneState != E_Fourth;
}

EZoneState ADeathZoneActor::GetZoneState() const
{
	return ZoneState;
}

EZoneState ADeathZoneActor::GetNextZoneState() const
{
	return static_cast<EZoneState>(ZoneState + 2);
}

void ADeathZoneActor::SetZoneState(const EZoneState& State)
{
	ZoneState = State;
}

void ADeathZoneActor::GoToNextZoneState()
{
	if(GetZoneState() == E_Fourth) return;
	SetZoneState(static_cast<EZoneState>(ZoneState + 2));
}

EZoneDirection ADeathZoneActor::GetZoneDirection() const
{
	return ZoneDirection;
}
#pragma endregion
