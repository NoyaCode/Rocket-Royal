// Fill out your copyright notice in the Description page of Project Settings.


#include "BumperActor.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "RocketRoyal/Player/MilitaryCharacter.h"

ABumperActor::ABumperActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* BumperRootComponent = CreateDefaultSubobject<USceneComponent>("BumperRoot");
	RootComponent  = BumperRootComponent;
	
	JumpPad = CreateDefaultSubobject<UStaticMeshComponent>("SpawnMesh");
	JumpPad->SetupAttachment(BumperRootComponent);
	
	JumpZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Capture Zone"));
	JumpZone->SetupAttachment(BumperRootComponent);
}

void ABumperActor::BeginPlay()
{
	Super::BeginPlay();
	
	JumpPad->SetCollisionResponseToChannels(ECR_Ignore);
	JumpZone->OnComponentBeginOverlap.AddDynamic(this, &ABumperActor::BumpOnCollide);
}

void ABumperActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100, FColor::Magenta, false, -1, 0, 10);
}

void ABumperActor::BumpOnCollide(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMilitaryCharacter* Character = Cast<AMilitaryCharacter>(OtherActor);
	
	if (IsValid(Character))
	{
		const FVector LaunchDirection = GetActorForwardVector() * BumpForce;
		Character->LaunchCharacter(LaunchDirection, true, true);
	}
}


