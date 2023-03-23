/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPointActor.generated.h"

class AMilitaryCharacter;
UENUM()
enum ESpawnTeam
{
	E_BLUE      UMETA(DisplayName = "Blue"),
	E_RED       UMETA(DisplayName = "Red"),
	E_UNDEFINED UMETA(DisplayName = "Undefined"),
};

UCLASS()
class ROCKETROYAL_API ASpawnPointActor : public AActor
{
	GENERATED_BODY()

private:
	TArray<AMilitaryCharacter*> PlayersInZone;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Physics") UShapeComponent* ShapeComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Render") UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Render") UMaterial* Material = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Properties") TEnumAsByte<ESpawnTeam> AssignedTeamSpawn = E_UNDEFINED;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Properties") float HealingGainRate  = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Properties") float UltimateGainRate = 0.02f;
	
protected:
	virtual void BeginPlay()									   override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds)						   override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)	   override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor)		   override;
	
public:	
	ASpawnPointActor();
	
	ESpawnTeam GetTeamAssigned()	const;
	UStaticMeshComponent* GetMesh() const;
	
	bool CheckActorSpawnPoint(const AMilitaryCharacter* Character) const;
};
