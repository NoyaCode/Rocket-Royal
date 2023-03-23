/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "SpawnPointActor.h"
#include "ExplosionActor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"
#include "BulletActor.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class AMilitaryCharacter;
UENUM()
enum EBulletClass
{
	E_EXPLOSIVE UMETA(DisplayName = "Explosive"),
	E_HOMING	UMETA(DisplayName = "Homing")
};

UENUM()
enum EBulletType
{
	E_PROJECTILE   UMETA(DisplayName = "Projectile"),
	E_INTERACTABLE UMETA(DisplayName = "Interactable")
};

UCLASS()
class ROCKETROYAL_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
private:
	AActor*		        TargetActor = nullptr;
	AMilitaryCharacter* SenderActor = nullptr;

	bool ShouldDestroyState = false;
	
	FVector DestroyLocation;
	FVector ActorLocation;
	
	ESpawnTeam  BulletTeam = E_UNDEFINED;
	EBulletType BulletType;

	UNiagaraSystem* CurrentTrail = nullptr;

	float DelaySoundRocket = 0;
	bool  IsUltimateBullet = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties") TEnumAsByte<EBulletClass> BulletClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties") TSubclassOf<AExplosionActor>  BPExplosion = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties") int	  AppliedDamage	= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties") int	  DroppedUnits	= 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties") float ShootingSpeed = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties") float DespawnDelay  = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Properties") bool  FriendlyFire  = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Physics") UShapeComponent*			   BulletShapeComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Physics") UProjectileMovementComponent* ProjectileComponent  = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Physics") bool  ApplyRadialForce  = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Physics") float ImpactRadius		 = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Physics") float RadialImpactForce = 2000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Render") UStaticMeshComponent*     BulletMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet Render") UParticleSystemComponent* ParticleSystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon VFX") bool  FloatingEffect = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon VFX") float FloatingRate   = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX") UNiagaraSystem* NSTrail;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX") UNiagaraSystem* NSTrailUltimate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX") UNiagaraSystem* NSExplosion;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX") UNiagaraSystem* NSExplosionUltimate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds") USoundCue* SoundExplosion;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds") USoundCue* SoundRocket;

private:
	void BindBulletTimer();
	
protected:
	virtual void BeginPlay              ()									       override;
	virtual void EndPlay                (const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick                   (float DeltaTime)						   override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor)					   override;

public:	
	ABulletActor();
	ABulletActor* InitAsProjectile  (const FRotator& Rotation, AMilitaryCharacter* Sender, const FVector& DestroyAtLocation);
	ABulletActor* InitAsInteractable();
	
	EBulletClass GetBulletClass() const;
	EBulletType  GetBulletType () const;
	ESpawnTeam   GetBulletTeam () const;

	void SetupAsUltimate();
	
	UProjectileMovementComponent* GetProjectileComponent() const;
	
	int  GetDamage()	   const;
	int  GetDroppedUnits() const;
	
	bool IsProjectile  ()									 const;
	bool HasTarget     ()									 const;
	bool IsFriendlyFire()									 const;
	bool IsSenderBullet(const AMilitaryCharacter* Character) const;
	bool IsTeamBullet  (const AMilitaryCharacter* Character) const;
	bool ShouldDestroy ()									 const;	
	
	float GetShootingSpeed() const;
	
	void SetDroppedUnits(const float& Amount);
	void SetTarget      (AActor* Target);

	void DestroyEvent          ();
	void HomingBehaviorEvent   ();
	void ExplosiveBehaviorEvent(const FVector& Direction);
};
