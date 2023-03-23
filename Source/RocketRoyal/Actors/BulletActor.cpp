/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "BulletActor.h"

#include "DrawDebugHelpers.h"
#include "WeaponActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "RocketRoyal/Player/MilitaryCharacter.h"
#include "RocketRoyal/Utils/Utils.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#pragma region Setup
ABulletActor::ABulletActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* BulletRootComponent = CreateDefaultSubobject<USceneComponent>("BulletRoot");
	RootComponent  = BulletRootComponent;
	
	BulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	BulletMeshComponent->SetupAttachment(RootComponent);
	
	BulletShapeComponent = CreateDefaultSubobject<UBoxComponent>("BulletBox");
	BulletShapeComponent->SetupAttachment(RootComponent);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");

	Tags.Add("Bullet");
}

void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	BulletShapeComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BulletShapeComponent->SetGenerateOverlapEvents(true);
	
	InitAsInteractable();

	ActorLocation = GetActorLocation();
}

void ABulletActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

ABulletActor* ABulletActor::InitAsProjectile(const FRotator& Rotation, AMilitaryCharacter* Sender, const FVector& DestroyAtLocation)
{
	if(!IsValid(this)) return nullptr;

	/*for(UStaticMeshComponent* Mesh : BulletMeshComponents)
		Mesh->SetCollisionResponseToAllChannels(ECR_Block);*/

	SenderActor = Sender;
	BulletType = E_PROJECTILE;
	DebugWarning("Sender actor team %d",SenderActor->GetTeam() );
	BulletTeam = SenderActor->GetTeam();
	DestroyLocation = DestroyAtLocation;
	
	FloatingEffect = false;
	SetActorRotation(Rotation);
	BindBulletTimer();
	
	CurrentTrail = NSTrail;

	return this;
}

ABulletActor* ABulletActor::InitAsInteractable()
{
	/*for(UStaticMeshComponent* Mesh : BulletMeshComponents)
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);*/
	
	BulletType = E_INTERACTABLE;
	FloatingEffect = true;
	
	return this;
}

#pragma endregion

#pragma region Update
void ABulletActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (FloatingEffect)
	{
		SetActorLocation(ActorLocation + FVector (0,0, sin(UGameplayStatics::GetRealTimeSeconds(GetWorld())*2)/6));
		SetActorRotation(GetActorRotation() + FRotator(0, FloatingRate, 0));
	}
	else
	{
		if(CurrentTrail && SoundRocket)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CurrentTrail, GetActorLocation());

			DelaySoundRocket += DeltaSeconds;
			if(DelaySoundRocket >= 0.2)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundRocket, GetActorLocation());
				DelaySoundRocket = 0;
			}
		}
		else
		{
			Debug("no Niagara System or No Sound for Rocket"); 
		}
	}
	
}

void ABulletActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(ShouldDestroy())					      return;
	if( OtherActor->Tags.Contains("Bullet"))  return;
	if( OtherActor->Tags.Contains("Weapon"))  return;
	if( OtherActor->Tags.Contains("Airdrop")) return;
	if(!OtherActor->Tags.Contains("Player" )) return DestroyEvent();
	
	AMilitaryCharacter* MilitaryCharacter = Cast<AMilitaryCharacter>(OtherActor);
	if(!IsValid(MilitaryCharacter)) return;
	
	if(IsProjectile())
	{
		//DebugInfo("Bullet: IsProjectile");

		if (!IsValid(SenderActor)) return;
		if (IsSenderBullet(MilitaryCharacter)) return;

		if(GetBulletTeam() != E_UNDEFINED && !IsTeamBullet(MilitaryCharacter) && !IsFriendlyFire())
			MilitaryCharacter->TakeDamageEvent(SenderActor, AppliedDamage);
		
		DestroyEvent();
	}
	else
	{
		//DebugInfo("NotProjectile");
		if(!MilitaryCharacter->IsHoldingWeapon()) return;
	
		AWeaponActor* LauncherWeapon = Cast<AWeaponActor>(MilitaryCharacter->GetWeapon());
		if(!IsValid(LauncherWeapon))							return;
		if(LauncherWeapon->GetBulletTypeUsage() != BulletClass) return;
		if(MilitaryCharacter->GetWeapon()->IsReserveFull())		return;
		
		// TODO: Store in player inventory
		const int UnitsLeft = LauncherWeapon->AddReserveBullet(GetDroppedUnits());
			
		if(UnitsLeft == 0) { Destroy(); return; }

		SetActorLocation(OtherActor->GetActorLocation());
		SetDroppedUnits(UnitsLeft);
	}
}
#pragma endregion 

#pragma region Events
void ABulletActor::HomingBehaviorEvent()
{
	if(IsValid(ParticleSystemComponent)) ParticleSystemComponent->Activate(true);
	if(!HasTarget()) return;
	
	//ProjectileComponent->HomingTargetComponent = TargetActor;
	ProjectileComponent->bIsHomingProjectile = true;
	ProjectileComponent->HomingAccelerationMagnitude = GetShootingSpeed();
}

void ABulletActor::ExplosiveBehaviorEvent(const FVector& Direction)
{
	if(IsValid(ParticleSystemComponent)) ParticleSystemComponent->Activate(true);
	
	ProjectileComponent->InitialSpeed = GetShootingSpeed();
	ProjectileComponent->MaxSpeed = GetShootingSpeed();
	ProjectileComponent->Velocity = Direction * GetShootingSpeed();
}
#pragma endregion 

#pragma region Getter & Setter
EBulletClass ABulletActor::GetBulletClass() const
{
	return BulletClass;
}

EBulletType ABulletActor::GetBulletType() const
{
	return BulletType;
}

ESpawnTeam ABulletActor::GetBulletTeam() const
{
	return BulletTeam;
}

void ABulletActor::SetupAsUltimate()
{
	IsUltimateBullet = true;
	CurrentTrail = NSTrailUltimate;
}

UProjectileMovementComponent* ABulletActor::GetProjectileComponent() const
{
	return IsValid(ProjectileComponent) ? ProjectileComponent : nullptr;
}

int ABulletActor::GetDamage() const
{
	return AppliedDamage;
}

bool ABulletActor::IsProjectile() const
{
	return BulletType == E_PROJECTILE;
}

bool ABulletActor::HasTarget() const
{
	return TargetActor != nullptr;
}

bool ABulletActor::IsFriendlyFire() const
{
	return FriendlyFire;
}

bool ABulletActor::IsSenderBullet(const AMilitaryCharacter* Character) const
{
	return SenderActor == Character;
}

bool ABulletActor::IsTeamBullet(const AMilitaryCharacter* Character) const
{
	return Character->GetTeam() == BulletTeam;
}

bool ABulletActor::ShouldDestroy() const
{
	return ShouldDestroyState;
}

float ABulletActor::GetShootingSpeed() const
{
	return ShootingSpeed;
}

int ABulletActor::GetDroppedUnits() const
{
	return DroppedUnits;
}

void ABulletActor::SetDroppedUnits(const float& Amount)
{
	DroppedUnits = Amount;
}

void ABulletActor::SetTarget(AActor* Target)
{
	TargetActor = Target;
}

void ABulletActor::BindBulletTimer()
{
	//DebugInfo("Started BulletTimer");
	FTimerHandle TimerHandle = {};
	FTimerDelegate TimerDelegate = {};
	TimerDelegate.BindLambda([this]{ DestroyEvent(); } );
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, DespawnDelay, false, DespawnDelay);
}

void ABulletActor::DestroyEvent()
{
	if(ShouldDestroy()) return;
	
	DebugWarning("Destroyed Bullet");
	ShouldDestroyState = true;
	AExplosionActor* ExplosionActor = GetWorld()->SpawnActorDeferred<AExplosionActor>(BPExplosion, GetTransform());
	//ExplosionActor = Cast<AExplosionActor>(UGameplayStatics::FinishSpawningActor(ExplosionActor, GetTransform()));
	
	float DamageToApply = GetDamage() / 2;

	if (IsValid(ExplosionActor))
	{
		DebugInfo("ExplosionActor");
		if(IsUltimateBullet)
		{
			DamageToApply *= 2;
			ExplosionActor->SetActorRelativeScale3D(ExplosionActor->GetActorRelativeScale3D() * 2);
		}
		if (SenderActor)
			ExplosionActor->SetExplosionData(BulletTeam, DamageToApply, SenderActor);
		else
		{
			Debug("ALLLLLLLLLLLLLEEEEEEEEEEEEDDDDDDDDDD");
		}
		ExplosionActor->FinishSpawning(GetTransform());
	}


	if(IsUltimateBullet)
	{
		if(NSExplosionUltimate)
			UNiagaraComponent* ExplosionComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSExplosionUltimate, GetActorLocation());
	}
	else
	{
		if(NSExplosion)
			UNiagaraComponent* ExplosionComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NSExplosion, GetActorLocation());
	}

	if(SoundExplosion)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundExplosion, GetActorLocation());

	// TODO: Play Explode SOUND + Explode VFX
	//ParticleSystemComponent->Activate(true);
	//UGameplayStatics::PlaySoundAtLocation(this, RocketExplodeSound, GetActorLocation());
	Destroy();
}
#pragma endregion 
