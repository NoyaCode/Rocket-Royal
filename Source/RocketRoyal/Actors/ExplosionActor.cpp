// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionActor.h"

#include <string>

#include "DrawDebugHelpers.h"

#include "RocketRoyal/Player/MilitaryCharacter.h"
#include "RocketRoyal/Utils/Utils.h"

// Sets default values
AExplosionActor::AExplosionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ExplosionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
}

// Called when the game starts or when spawned
void AExplosionActor::BeginPlay()
{
	Super::BeginPlay();
	ExplosionCollider->OnComponentBeginOverlap.AddDynamic(this, &AExplosionActor::InExplosion);
	ExplosionCollider->SetHiddenInGame(false);
	FTimerHandle _;
	GetWorldTimerManager().SetTimer(_, this, &AExplosionActor::DeleteActor, ExplosionDuration, false);
}

// Called every frame
void AExplosionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosionActor::DeleteActor()
{
	Destroy();
}

void AExplosionActor::SetExplosionData(const ESpawnTeam Team, float Damage, AMilitaryCharacter* Sender)
{
	MyTeam = Team;
	ExplosionDamage = Damage;
	SenderPlayer = Sender;
}

void AExplosionActor::InExplosion(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMilitaryCharacter* Player = Cast<AMilitaryCharacter>(OtherActor);
	if(Player) 
	{
		DebugWarning("InBeginOverlap")
		FVector ImpulseVector = Player->GetActorLocation() - GetActorLocation();
		ImpulseVector.Normalize();
		FHitResult Hit;
		if(GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), Player->GetActorLocation(), ECC_Visibility)) return;
        if (Player->GetActorLocation().Z < ImpulseVector.Z)
         	ImpulseVector.Z = 0;
        Player->LaunchCharacter(ImpulseVector * 2000, true , true);
        if ( Player->GetTeam() != MyTeam)
        {
	        DebugInfo("Explosion damage %f", ExplosionDamage);
        	Player->TakeDamageEvent(SenderPlayer, ExplosionDamage);
        }
	}
}

