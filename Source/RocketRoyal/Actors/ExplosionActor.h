// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPointActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Tests/AutomationTestSettings.h"
#include "ExplosionActor.generated.h"

UCLASS()
class ROCKETROYAL_API AExplosionActor : public AActor
{
	GENERATED_BODY()
	
private:
	ESpawnTeam MyTeam = E_UNDEFINED;
	AMilitaryCharacter* SenderPlayer = nullptr;
	// Sets default values for this actor's properties
protected:
	UPROPERTY(VisibleAnywhere, Category = "Stats") float ExplosionDamage;
	UPROPERTY(EditAnywhere, Category = "Stats") float ExplosionDuration = 0.1;
	UPROPERTY(EditAnywhere) USphereComponent* ExplosionCollider;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void InExplosion(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void DeleteActor();
public:
	void SetExplosionData(const ESpawnTeam Team, float Damage, AMilitaryCharacter* Sender);
	AExplosionActor();

};
