// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BumperActor.generated.h"

UCLASS()
class ROCKETROYAL_API ABumperActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABumperActor();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere) UBoxComponent* JumpZone;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* JumpPad;
	UPROPERTY(EditAnywhere) float BumpForce = 500;
	UFUNCTION() void BumpOnCollide(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaSeconds) override;

};
