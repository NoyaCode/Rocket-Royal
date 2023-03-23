// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlagActor.h"
#include "SpawnPointActor.h"
#include "GameFramework/Actor.h"
#include "RocketRoyal/Interfaces/Components/ProgressBarComponent.h" 
#include "Components/BoxComponent.h"
#include "Managers/CaptureZoneManager.h"
#include "RocketRoyal/Gamemode/PersistentGameInstance.h"
#include "CaptureZone.generated.h"

enum EZoneStatus
{
	NOSTATE,
	INCREASING,
	DECREASING,
	CONTESTED,
};

UCLASS()
class ROCKETROYAL_API ACaptureZone : public AActor
{
	GENERATED_BODY()
	
private:
	bool actionDone = false;
	bool InZone = false;
	float CurrentPercent = 0;
	float CurrentSpeed = 0;
	ESpawnTeam CurrentTeam	     = E_UNDEFINED;
	ESpawnTeam LastTeam			 = E_UNDEFINED;
	EZoneStatus CurrentZoneState = NOSTATE;
	ACaptureZoneManager* CaptureManager;
	UPersistentGameInstance* GameInstance;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Properties") int UltReward = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Properties") float CaptureSpeed = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zone Properties") float DoubleCaptureSpeed = 1;

	UPROPERTY(EditAnywhere) TSubclassOf<AFlagActor>	  FlagActorBP;
	UPROPERTY() UProgressBarComponent* Widget;
	UPROPERTY() AFlagActor* FLag;
	UPROPERTY(VisibleAnywhere) TArray<AMilitaryCharacter*> ListPlayersInZone;
	UPROPERTY(VisibleAnywhere) class UWidgetComponent* CaptureProgress;
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUserWidget> CaptureWidget;
	UPROPERTY(EditAnywhere, Category = "Collider") UBoxComponent* CaptureZone;
	UPROPERTY(EditAnywhere) AFlagActor* FlagActor;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION() void OnBeginInZone(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnEndInZone(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	bool IsZoneStillContested(AMilitaryCharacter* RemovePlayer);
	void UpdateUlti();
	
public:
	ACaptureZone();

	EZoneStatus GetZoneState() const;

	void SetZoneState(const EZoneStatus NewState);
	void SetWidgetColor(const ESpawnTeam NewColor);
	void SetCaptureManager(ACaptureZoneManager* Manager);
};
