/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "WeaponActor.h"

#include "CollisionDebugDrawingPublic.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "RocketRoyal/Player/MilitaryCharacter.h"
#include "RocketRoyal/Utils/Utils.h"

#pragma region Setup
AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* WeaponRootComponent = CreateDefaultSubobject<USceneComponent>("WeaponRoot");
	RootComponent  = WeaponRootComponent;

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMeshComponent->SetupAttachment(RootComponent);

	Tags.Add("Weapon");

	WeaponRarity = static_cast<ERarityType>(rand() % E_LEGENDARY);
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	switch (WeaponRarity)
	{
		case E_COMMON:	  Tags.Add("Common");	 break;
		case E_RARE:	  Tags.Add("Rare");		 break;
		case E_EPIC:	  Tags.Add("Epic");		 break;
		case E_LEGENDARY: Tags.Add("Legendary"); break;
		default:								 break;
	}

	CachedLoadedBullets = LoadedBullets;
	CachedReserveBullets = ReserveBullets;
}

void AWeaponActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
#pragma endregion 

#pragma region Update
void AWeaponActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * 100, FColor::Magenta, false, -1, 0, 10);
	
	if (FloatingEffect) {
		SetActorLocation(GetActorLocation() + FVector (0,0, sin(UGameplayStatics::GetRealTimeSeconds(GetWorld())*2)/6));
		SetActorRotation(GetActorRotation() + FRotator(0, 1*FloatingRate, 0));
	}
}
#pragma endregion 

#pragma region Events
void AWeaponActor::PickupEvent(AMilitaryCharacter* NewParentCharacter)
{
	if(!NewParentCharacter->IsAlive()) return;
	
	if(NewParentCharacter->IsHoldingWeapon()) return;
	
	FloatingEffect = false;
	AttachToComponent(Cast<USceneComponent>(NewParentCharacter->GetMesh()),
					  FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					  SocketName);
	
	SetActorRelativeLocation({0, 0, 0});
	SetActorRelativeRotation({0, 0, 0});

	FloatingEffect = false;
	NewParentCharacter->SetWeapon(this);
	ParentCharacter = NewParentCharacter;
}

void AWeaponActor::DropEvent(AMilitaryCharacter* CurrentParentCharacter)
{
	if(!CurrentParentCharacter->IsAlive()) return;
	
	if(!CurrentParentCharacter->IsHoldingWeapon()) return;
	
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	SetActorRelativeLocation(CurrentParentCharacter->GetActorLocation());
	SetActorRelativeRotation({0, 0, 0});
	SetActorScale3D({1,1,1});

	FloatingEffect = true;
	CurrentParentCharacter->SetWeapon(nullptr);
	ParentCharacter = nullptr;
}

void AWeaponActor::ShootingEvent(AMilitaryCharacter* Character, const bool& IsUltimate)
{
	if(!Character->IsAlive()) return;
	
	if(!IsUltimate)
	{
		if(!IsShooting()) return;
		if(IsReloading()) return;

		DebugAction("Weapon: Shooting");
		SetShooting(false);
	}
	else
	{
		DebugAction("Weapon: Ultimate");
	}
	
	FVector  Location = GetActorLocation();
	//const FRotator Rotation = Character->GetCamera()->GetComponentRotation();
	
	const FVector CamForward  = Character->GetCamera()->GetForwardVector();
	const FVector CamPosition = Character->GetCamera()->GetComponentLocation();
	const float   CamOffset   = FVector::DotProduct((Location - CamPosition), CamForward);
	
	const FVector RaycastStart = CamPosition  + CamForward * CamOffset;
	const FVector RaycastEnd   = RaycastStart + CamForward * 10000;

	FVector HitLocation = RaycastEnd;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, RaycastStart, RaycastEnd, ECC_Visibility))
		HitLocation = Hit.Location;

	FVector BulletDirection = HitLocation - Location;
	BulletDirection.Normalize();

	if (FVector::DotProduct(BulletDirection, GetActorRightVector()) < 0.98f)
	{
		BulletDirection = CamForward;
		Location = Character->GetActorLocation();
		Location.Z = GetActorLocation().Z;
	}
	
	if(!IsUltimate)
	{
		RemoveLoadedBullet();
		if(IsLoaderEmpty())
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([this]{ ReloadingEvent(); });
			GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false, ReloadingCooldown);
		}
	}
	
	BindWeaponTimer(FName("SetShooting"), ShootingCooldown, true);
	
	const FVector BulletSpawnLocation = Location;
	const FTransform BulletTransform = FTransform(BulletDirection.Rotation(), BulletSpawnLocation, FVector(1));
	
	ABulletActor* BulletActor = GetWorld()->SpawnActorDeferred<ABulletActor>(BulletBP, BulletTransform, Character, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	BulletActor = Cast<ABulletActor>(UGameplayStatics::FinishSpawningActor(BulletActor, BulletTransform));
	if(!IsValid(BulletActor)) return;
	
	BulletActor->InitAsProjectile(BulletDirection.Rotation(), Character, HitLocation);
	//BulletActor->FinishSpawning(BulletTransform);
	if(IsUltimate) BulletActor->SetupAsUltimate();
	
	//DrawDebugLine(GetWorld(), BulletSpawnLocation, HitLocation, FColor::Cyan, true, -1, 0, 10);
	
	switch (BulletActor->GetBulletClass())
	{
		case E_EXPLOSIVE: BulletActor->ExplosiveBehaviorEvent(BulletDirection); break;
		case E_HOMING:	  BulletActor->HomingBehaviorEvent();					break;
		default:																break;
	}

	if(SoundRocketShoot)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundRocketShoot, GetActorLocation());
}

void AWeaponActor::ReloadingEvent()
{
	if(IsReserveEmpty()) return;
	
	//DebugAction("Reloading");
	RemoveReserveBullet();
	AddLoadedBullet(1);

	ParentCharacter->GetPlayerWidget()->UpdateWeaponEvent(GetLoadedBullets());

	SetReloading(true);
	BindWeaponTimer(FName("SetAutoReloading"), ReloadingCooldown, false);
}

void AWeaponActor::UltimateEvent(AMilitaryCharacter* Character)
{
	ShootingEvent(Character, true);
	Character->SetUltimateGauge(0);
}
#pragma endregion

#pragma region Miscs
void AWeaponActor::BindWeaponTimer(const FName& Function, const float& Cooldown, const bool& Arg)
{
	//DebugInfo("Started WeaponTimer");
	FTimerHandle TimerHandle	 = {};
	FTimerDelegate TimerDelegate = {};
	TimerDelegate.BindUFunction(this, Function, Arg);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false, Cooldown);

}
#pragma endregion

#pragma region Getter & Setter
int AWeaponActor::AddReserveBullet(const int& Amount)
{
	//DebugData("Before, Reserve Bullet : %d", ReserveBullets);
	
	const int Missing = GetReserveCapacity() - GetReserveBullets();
	//DebugData("%d", Missing);
	if(Amount < Missing)
	{
		ReserveBullets += Amount;
		//DebugData("Added, Reserve Bullet : %d", ReserveBullets);
		return 0;
	}
	
	ReserveBullets += Missing;
	//DebugData("Missing Added, Reserve Bullet : %d", ReserveBullets);
	return Amount - Missing;
}

void AWeaponActor::AddLoadedBullet(const float& Amount)
{
	LoadedBullets += Amount;
	//DebugData("Added, Loaded Bullet : %d", LoadedBullets);
}

void AWeaponActor::RemoveReserveBullet()
{
	ReserveBullets -= 1;
	//DebugData("Removed, Reserve Bullet : %d", ReserveBullets);
}

void AWeaponActor::RemoveLoadedBullet()
{
	LoadedBullets -= 1;
	ParentCharacter->GetPlayerWidget()->UpdateWeaponEvent(GetLoadedBullets());
	//DebugData("Removed, Loded Bullet : %d", LoadedBullets);
}

UStaticMeshComponent* AWeaponActor::GetMesh() const
{
	return WeaponMeshComponent;
}

ERarityType AWeaponActor::GetRarity() const
{
	return WeaponRarity;
}

EBulletClass AWeaponActor::GetBulletTypeUsage() const
{
	return BulletClassUsage;
}

float AWeaponActor::GetReloadTime() const
{
	return ReloadingCooldown;
}

float AWeaponActor::GetShootCooldown() const
{
	return ShootingCooldown;
}

int AWeaponActor::GetLoaderCapacity() const
{
	return LoaderCapacity;
}

int AWeaponActor::GetReserveCapacity() const
{
	return ReserveCapacity;
}

int AWeaponActor::GetLoadedBullets() const
{
	return LoadedBullets;
}

int AWeaponActor::GetReserveBullets() const
{
	return ReserveBullets;
}

bool AWeaponActor::IsReserveFull() const
{
	//DebugInfo("Reserve Capacity = %hs", ReserveBullets == ReserveCapacity ? "Full" : "Available");
	return ReserveBullets == ReserveCapacity;
}

bool AWeaponActor::IsLoaderFull() const
{
	//DebugInfo("Loader Capacity = %hs", LoadedBullets == LoaderCapacity ? "Full" : "Available");
	return LoadedBullets == LoaderCapacity;
}

bool AWeaponActor::IsLoaderEmpty() const
{
	//DebugInfo("Loader Capacity = %hs", LoadedBullets <= 0 ? "Empty" : "Available");
	return LoadedBullets <= 0;
}

bool AWeaponActor::IsReserveEmpty() const
{
	//DebugInfo("Reserve Capacity = %hs", ReserveBullets <= 0 ? "Empty" : "Available");
	return ReserveBullets <= 0;
}

bool AWeaponActor::IsReloading() const
{
	//DebugInfo("IsReloading = %hs", Reloading ? "True" : "False");
	return Reloading;
}

bool AWeaponActor::IsShooting() const
{
	//DebugInfo("IsShooting = %hs", Shooting ? "True" : "False");
	return Shooting;
}

bool AWeaponActor::IsPickable() const
{
	return ParentCharacter == nullptr;
}

void AWeaponActor::SetShooting(const bool& Condition)
{
	//DebugInfo("SetShooting to = %hs", Condition ? "True" : "False");
	Shooting = Condition;
}

void AWeaponActor::SetReloading(const bool& Condition)
{
	//DebugInfo("SetReloading to = %hs", Condition ? "True" : "False");
	Reloading = Condition;

	if (Condition)
		ParentCharacter->GetPlayerWidget()->GetReloading()->SetVisibility(ESlateVisibility::Visible);
	else
		ParentCharacter->GetPlayerWidget()->GetReloading()->SetVisibility(ESlateVisibility::Hidden);
}


void AWeaponActor::SetAutoReloading(const bool& Condition)
{
	if(!IsValid(ParentCharacter)) return SetReloading(false);
	
	if(IsLoaderFull() || IsReserveEmpty())
	{
		SetReloading(Condition);
		return;
	}
	
	ReloadingEvent();
}

void AWeaponActor::ResetWeapon()
{
	LoadedBullets  = CachedLoadedBullets;
	ReserveBullets = CachedReserveBullets;
}
#pragma endregion 
