/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "BulletActor.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class AMilitaryCharacter;
UENUM()
enum ERarityType
{
	E_COMMON    UMETA(DisplayName = "Common"),
	E_RARE	    UMETA(DisplayName = "Rare"),
	E_EPIC   	UMETA(DisplayName = "Epic"),
	E_LEGENDARY UMETA(DisplayName = "Legendary"),
};

UENUM()
enum EEquipmentType
{
	RocketLauncher UMETA(DisplayName = "RocketLauncher"),
};

UCLASS()
class ROCKETROYAL_API AWeaponActor : public AActor
{
	GENERATED_BODY()

private:
	AMilitaryCharacter* ParentCharacter = nullptr;

	int CachedLoadedBullets;
	int CachedReserveBullets; 
	
	ERarityType WeaponRarity;
	
	bool Reloading = false;
	bool Shooting  = true;
	
	FName SocketName = "WeaponSocket";
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties") TEnumAsByte<EBulletClass>  BulletClassUsage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties") TSubclassOf<ABulletActor>  BulletBP = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties") float ReloadingCooldown	= 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties") float ShootingCooldown  = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties") int	  LoaderCapacity	= 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties") int	  ReserveCapacity	= 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties") int	  LoadedBullets	    = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties") int	  ReserveBullets	= 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Render") UStaticMeshComponent* WeaponMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon VFX") UParticleSystemComponent* ParticleSystemComponent  = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon VFX") bool					   FloatingEffect			= true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon VFX") float					   FloatingRate				= 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds") USoundCue* SoundRocketShoot;

protected:
	virtual void BeginPlay()									   override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds)						   override;
public:	
	AWeaponActor();

	ERarityType GetRarity          () const;
	EBulletClass GetBulletTypeUsage() const;
	
	UStaticMeshComponent* GetMesh() const;
	
	float GetReloadTime   () const;
	float GetShootCooldown() const;
	
	int GetLoaderCapacity () const;
	int GetReserveCapacity() const;
	int GetLoadedBullets  () const;
	int GetReserveBullets () const;
	int AddReserveBullet  (const int& Amount);
	
	bool IsReserveFull () const;
	bool IsLoaderFull  () const;
	bool IsReserveEmpty() const;
	bool IsLoaderEmpty () const;
	bool IsReloading   () const;
	bool IsShooting    () const;
	bool IsPickable    () const;

	UFUNCTION() void SetShooting	 (const bool& Condition);
	UFUNCTION() void SetReloading	 (const bool& Condition);
	UFUNCTION() void SetAutoReloading(const bool& Condition);

	void ResetWeapon();
	void AddLoadedBullet(const float& Amount);
	void RemoveReserveBullet();
	void RemoveLoadedBullet();
	void ReloadingEvent();
	void ShootingEvent(AMilitaryCharacter* Character, const bool& IsUltimate);
	void UltimateEvent(AMilitaryCharacter* Character);
	void PickupEvent(AMilitaryCharacter* NewParentCharacter);
	void DropEvent(AMilitaryCharacter* CurrentParentCharacter);
	
	void BindWeaponTimer(const FName& Function, const float& Cooldown, const bool& Arg);
};
