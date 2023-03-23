/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AirdropActor.generated.h"

UCLASS()
class ROCKETROYAL_API AAirdropActor : public AActor
{
	GENERATED_BODY()

private:
	FVector FinalLocation;

	bool InDestroyState = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airdrop Properties") float DescendingRate  = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airdrop Properties") float DespawnDelay    = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airdrop Properties") TArray<TSubclassOf<AActor>> DropsBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airdrop Physics") UShapeComponent* AirdropShapeComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airdrop Render") UStaticMeshComponent* DropMeshComponent	  = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airdrop Render") UStaticMeshComponent* ParachuteMeshComponent = nullptr;

private:
	void BindAirdropDestroyTimer();
	void BindAirdropDescendingTimer();
	
protected:
	virtual void BeginPlay              ()									       override;
	virtual void EndPlay                (const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick                   (float DeltaSeconds)					   override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)					   override;

public:	
	AAirdropActor();

	bool    IsOnFloor		() const;
	FVector GetFinalLocation() const;
	int		PickRandomDrop  () const;
	
	void SetFinalLocation(const FVector& Location);
};
