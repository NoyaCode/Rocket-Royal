/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "RocketRoyal/Actors/ScoreActor.h"
#include "RocketRoyal/Actors/SpawnPointActor.h"
#include "RocketRoyal/Actors/WeaponActor.h"
#include "RocketRoyal/Interfaces/PlayerWidget.h"
#include "MilitaryCharacter.generated.h"

class UPauseMenu;

UCLASS()
class ROCKETROYAL_API AMilitaryCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	AScoreActor* GameScoreInstance = nullptr;
	
	float BaseTurnRate   = 120.f;
	float BaseLookUpRate = 120.f;

	bool IsRespawning = false;
	
	int CurrentKills   = 0;
	int CurrentAssists = 0;
	int CurrentDeaths  = 0;

	UPauseMenu* pauseMenu;
	
	FVector                 MeshLoacation;
	USkeletalMeshComponent* Mesh;
	UCapsuleComponent *     Capsule;
	
	USpringArmComponent*         SpringArmComponent         = nullptr;
	UCameraComponent*            CameraComponent            = nullptr;
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	AWeaponActor*				 CurrentWeapon				= nullptr;
	UPlayerWidget*				 PlayerInterfaceWidget		= nullptr;
	TArray<ASpawnPointActor*>    AvailableTeamSpawn;

protected:
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraLag      = 15.f;
	UPROPERTY(EditAnywhere, Category = "Camera") float CameraDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "Player Properties") float InvincibilityCooldown = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float MaxHealth				= 100.f;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float CurrentHealth			= 100.f;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float MaxUltimate			= 100.0f;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float CurrentUltimate		= 0.0f;
	UPROPERTY(EditAnywhere, Category = "Player Properties") float RespawnCooldown		= 100.0f;
	UPROPERTY(VisibleAnywhere, Category = "Player Properties") TEnumAsByte<ESpawnTeam> CurrentTeam = E_UNDEFINED;
	UPROPERTY(EditAnywhere, Category = "Player Properties") bool Invincibility			= false;
	
	UPROPERTY(EditAnywhere, Category = "Player Properties") TSubclassOf<AWeaponActor>  BaseWeaponBP   = nullptr;
	UPROPERTY(EditAnywhere, Category = "Player Properties") TSubclassOf<UPlayerWidget> PlayerWidgetBP = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UUserWidget> pauseMenuClass;


private:
	void MoveForward (const float Amount);
	void MoveRight   (const float Amount);
	
	void Turn		 (const float Amount);
	void LookUp		 (const float Amount);

	virtual void Jump() override;

protected:
	virtual void BeginPlay()												   override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)			   override;
	virtual void Tick(float DeltaSeconds)									   override;
	virtual void SetupPlayerInputComponent(UInputComponent* NewInputComponent) override;
	virtual void PossessedBy(AController* NewController)					   override;
	
public:
	AMilitaryCharacter();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) void SetMeshPlayer();
	UFUNCTION(BlueprintCallable) void Pause ();

#pragma region Events
	void TakeDamageEvent  (AMilitaryCharacter* Sender, const float& Value);
	void TakeHealEvent    (const float& Value);
	void TakeUltimateEvent(const float& Value);
	void ShootEvent();
	void ShootUltimateEvent();
	void DropWeaponEvent();
	UFUNCTION(BlueprintCallable) void RespawnEvent(const bool& NaturalCause);
	void DeathEvent(AMilitaryCharacter* Sender);
#pragma endregion 

#pragma region Getters & Setters
	float GetMaxHealth()   const;
	float GetHealth()	   const;
	float GetUltimate()	   const;
	float GetMaxUltimate() const;
	
	int GetDeaths()  const;
	int GetKills()   const;
	int GetAssists() const;

	void AddDeaths (const int& Amount);
	void AddKills  (const int& Amount);
	void AddAssists(const int& Amount);
	void SetInvincibility(const bool& Condition);

	UFUNCTION(BlueprintCallable) void SetTeam(const ESpawnTeam& Team);
	void SetHealthGauge                      (const float& Value);
	void SetUltimateGauge                    (const float& Value);
	void SetWeapon                           (AWeaponActor* Weapon);
	
	bool IsAlive()		       const;
	bool IsUltimateAvailable() const;
	bool IsHoldingWeapon()     const;
	bool IsInvincible()		   const;
	
	AWeaponActor*     GetWeapon()                     const;
	UCameraComponent* GetCamera()                     const;
	UFUNCTION(BlueprintCallable) ESpawnTeam GetTeam() const;

	UPlayerWidget* GetPlayerWidget() const;

	AScoreActor* GetGameScore();
#pragma endregion

#pragma region Miscs
	void BindRespawnTimer(const bool& NaturalCause);
	void SpawnAtAssignedTeamPoint();
	void GiveDefaultWeapon();
#pragma endregion 
};
