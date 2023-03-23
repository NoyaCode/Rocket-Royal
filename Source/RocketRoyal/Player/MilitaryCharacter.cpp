/*
 * Copyright @NoIdeaIndustry. All Rights Reserved.
 */

#include "MilitaryCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RocketRoyal/Actors/WeaponActor.h"
#include "RocketRoyal/Gamemode/PersistentGameInstance.h"
#include "RocketRoyal/Interfaces/PlayerWidget.h"
#include "RocketRoyal/Utils/Utils.h"
#include "../Interfaces/PauseMenu.h"

#pragma region Setup
AMilitaryCharacter::AMilitaryCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	// Setup camera spring arm.
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = CameraLag;
	SpringArmComponent->TargetArmLength = CameraDistance;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 35.0f, 35.0f));
	SpringArmComponent->SocketOffset = FVector(0.0f, 15.0f, 0.0f);
	SpringArmComponent->ProbeSize = 2.0f;

	// Setup camera.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));

	// Setup character movement.
	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bUseControllerDesiredRotation = false;
	CharacterMovementComponent->bOrientRotationToMovement = false;
	
	Tags.Add("Player");

	Mesh	= GetMesh();
	Capsule = GetCapsuleComponent();
}

void AMilitaryCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);

	NewInputComponent->BindAxis("MoveForward", this, &AMilitaryCharacter::MoveForward);
	NewInputComponent->BindAxis("MoveRight", this, &AMilitaryCharacter::MoveRight);

	NewInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	NewInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	NewInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	NewInputComponent->BindAxis("TurnRate", this, &AMilitaryCharacter::Turn);

	NewInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	NewInputComponent->BindAxis("LookUpRate", this, &AMilitaryCharacter::LookUp);
	
	NewInputComponent->BindAction("Shoot" , IE_Pressed, this , &AMilitaryCharacter::ShootEvent);
	NewInputComponent->BindAction("Ultimate" , IE_Pressed, this , &AMilitaryCharacter::ShootUltimateEvent);

	NewInputComponent->BindAction("Pause", IE_Pressed, this, &AMilitaryCharacter::Pause).bExecuteWhenPaused = true;
	//NewInputComponent->BindAction("Drop" , IE_Pressed, this , &AMilitaryCharacter::DropWeaponEvent);
}

void AMilitaryCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if(!IsValid(NewController)){return;}
	APlayerController* PlayerController = Cast<APlayerController>(NewController);

	if (!IsValid(PlayerWidgetBP)) return;
	PlayerInterfaceWidget = Cast<UPlayerWidget>(CreateWidget(PlayerController, PlayerWidgetBP));
	if (!IsValid(PlayerInterfaceWidget)) return;
	
	PlayerInterfaceWidget->SetVisibility(ESlateVisibility::Visible);

	PlayerInterfaceWidget->GetHealthBar()->SetupComponent(GetHealth(), GetMaxHealth(), FColor::Red);
	PlayerInterfaceWidget->GetUltimateBar()->SetupComponent(GetUltimate(), GetMaxUltimate(), FColor::Green);
	PlayerInterfaceWidget->GetStats()->SetupStatsComponent(this);

	PlayerInterfaceWidget->GetBulletComponent()->UpdateCurrentBullets(FText::AsNumber(GetWeapon()->GetLoadedBullets()));
	PlayerInterfaceWidget->GetBulletComponent()->UpdateMaxBullets(FText::AsNumber(GetWeapon()->GetLoaderCapacity()));

	PlayerInterfaceWidget->SetupScale();
	PlayerInterfaceWidget->AddToPlayerScreen();
}

void AMilitaryCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameScoreInstance = Cast<UPersistentGameInstance>(GetWorld()->GetGameInstance())->GetScoreActor();
	MeshLoacation = GetMesh()->GetRelativeLocation();
	
	SpringArmComponent->CameraLagSpeed  = CameraLag;
	SpringArmComponent->TargetArmLength = CameraDistance;
	
	GiveDefaultWeapon();
}

void AMilitaryCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
#pragma endregion

#pragma region Update
void AMilitaryCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
#pragma endregion

#pragma region Miscs
void AMilitaryCharacter::BindRespawnTimer(const bool& NaturalCause)
{
	DebugInfo("Started RespawnTimer");
	IsRespawning = true;
	FTimerHandle TimerHandle = {};
	FTimerDelegate TimerDelegate = {};
	TimerDelegate.BindUFunction(this, FName("RespawnEvent"), NaturalCause);
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false, RespawnCooldown);
}

void AMilitaryCharacter::SpawnAtAssignedTeamPoint()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPointActor::StaticClass(), SpawnPoints);
	if(SpawnPoints.Num() != 0)
	{
		//DebugData("Found spawnPoints");
		for(AActor* Actor : SpawnPoints)
		{
			ASpawnPointActor* SpawnPoint = Cast<ASpawnPointActor>(Actor);
			if(!IsValid(SpawnPoint)) return;
			if(SpawnPoint->GetTeamAssigned() != GetTeam())
			{
				//DebugInfo("Team is different")
				continue;
			}

			AvailableTeamSpawn.Add(SpawnPoint);
			//DebugData("Added spawnPoint");
		}
	}

	if(AvailableTeamSpawn.Num() != 0)
	{
		const int RandIndex = FMath::RandRange(0, AvailableTeamSpawn.Num()-1);

		const FVector SpawnLocation = AvailableTeamSpawn[RandIndex]->GetActorLocation() + FVector(0, 0, 5);
		const FRotator SpawnRotation = AvailableTeamSpawn[RandIndex]->GetActorRotation();
		
		SetActorLocation(SpawnLocation);
		SetActorRotation(SpawnRotation);
		
		//DebugData("Setting location with spawn point");
	}
}

void AMilitaryCharacter::GiveDefaultWeapon()
{
	if(!IsValid(BaseWeaponBP)) return;
	AWeaponActor* WeaponActor = Cast<AWeaponActor>(GetWorld()->SpawnActor(BaseWeaponBP));
	WeaponActor->PickupEvent(this);
}
#pragma endregion

#pragma region Movement
void AMilitaryCharacter::Turn(const float Amount)
{
	if(!IsAlive()) return;
	AddControllerYawInput(Amount * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMilitaryCharacter::LookUp(const float Amount)
{
	if(!IsAlive()) return;
	AddControllerPitchInput(Amount * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMilitaryCharacter::Pause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), !GetWorld()->IsPaused());

	Debug("pause Called");
	if(GetWorld()->IsPaused())
	{
		pauseMenu = Cast<UPauseMenu>(CreateWidget<UUserWidget>(GetWorld(), pauseMenuClass));
		if(pauseMenu)
		{
			const int ControllerID = UGameplayStatics::GetPlayerControllerID(Cast<APlayerController>(Controller));
			pauseMenu->InitializePauseMenu(ControllerID);
			pauseMenu->AddToViewport();
			Debug("added");
		}
	}
	else
	{
		if(pauseMenu)
		{
			pauseMenu->RemoveFromViewport();
			Debug("added");
		}
	}
}

void AMilitaryCharacter::Jump()
{
	if(!IsAlive()) return;
	Super::Jump();
}

void AMilitaryCharacter::MoveForward(const float Amount)
{
	if(!IsAlive()) return;
	if (Controller == nullptr && Amount == 0.0f) return;

	const FRotator CurrentRotation = Controller->GetControlRotation();
	const FRotator NewRotation(0, CurrentRotation.Yaw, 0);

	AddMovementInput(FRotationMatrix(NewRotation).GetUnitAxis(EAxis::X), Amount);
}

void AMilitaryCharacter::MoveRight(const float Amount)
{
	if(!IsAlive()) return;
	if (Controller == nullptr && Amount == 0.0f) return;

	const FRotator CurrentRotation = Controller->GetControlRotation();
	const FRotator NewRotation(0, CurrentRotation.Yaw, 0);

	AddMovementInput(FRotationMatrix(NewRotation).GetUnitAxis(EAxis::Y), Amount);
}

#pragma endregion

#pragma region Events
void AMilitaryCharacter::TakeDamageEvent(AMilitaryCharacter* Sender, const float& Value)
{
	if(IsInvincible())
	{
		DebugWarning("Player is invincible");
		return;
	}
	
	SetHealthGauge(CurrentHealth -= Value);
	if(!IsAlive()) return DeathEvent(Sender);
	
	//DebugData("Health Lose %f", CurrentHealth)
}

void AMilitaryCharacter::TakeHealEvent(const float& Value)
{
	if(!IsAlive()) return;
	if(CurrentHealth >= MaxHealth) return;
	SetHealthGauge(CurrentHealth += Value);
	
	//DebugData("Health Gain %f", CurrentHealth)
}

void AMilitaryCharacter::TakeUltimateEvent(const float& Value)
{
	if(!IsAlive()) return;
	if(CurrentUltimate >= MaxUltimate) return;
	SetUltimateGauge(CurrentUltimate + Value);
	
	//DebugData("Ultimate Gain %f", CurrentUltimate)
}

void AMilitaryCharacter::ShootEvent()
{
	if(IsValid(CurrentWeapon)) CurrentWeapon->ShootingEvent(this, false);
}

void AMilitaryCharacter::ShootUltimateEvent()
{
	if(!IsUltimateAvailable()) return;
	
	if(IsValid(CurrentWeapon)) CurrentWeapon->UltimateEvent(this);
}

void AMilitaryCharacter::DropWeaponEvent()
{
	if(IsValid(CurrentWeapon)) CurrentWeapon->DropEvent(this);
}

void AMilitaryCharacter::RespawnEvent(const bool& NaturalCause)
{
	SetInvincibility(true);
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this] { SetInvincibility(false); });
		GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false, InvincibilityCooldown);
	}
	
	CharacterMovementComponent->SetMovementMode(MOVE_Walking);
	
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionObjectType(ECC_Pawn);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->AttachToComponent(Capsule, FAttachmentTransformRules::KeepRelativeTransform);
	Mesh->SetRelativeLocation(MeshLoacation);
	Mesh->SetRelativeRotation(FRotator(0, -90, 0));
	
	CurrentHealth = MaxHealth;

	SetHealthGauge(CurrentHealth);
	if(!NaturalCause) SetUltimateGauge(CurrentUltimate + 30);
	
	SpawnAtAssignedTeamPoint();
	IsRespawning = false;
	
	if(!IsValid(GetWeapon())) return;
	GetWeapon()->ResetWeapon();
}

void AMilitaryCharacter::DeathEvent(AMilitaryCharacter* Sender)
{
	CharacterMovementComponent->SetMovementMode(MOVE_None);
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionObjectType(ECC_PhysicsBody);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);

	AddDeaths(1);

	const bool SenderDefined = IsValid(Sender);
	if(!SenderDefined) return;
	
	if(!IsRespawning) BindRespawnTimer(SenderDefined);
	Sender->AddKills(1);
	Sender->SetUltimateGauge(Sender->GetUltimate() + 10);
	
	switch (GetTeam())
	{
		case E_BLUE: GameScoreInstance->GetScoreWidget()->UpdateRedBar(GameScoreInstance->GetScoreWidget()->UpdateValue(E_RED, 1));  break;
		case E_RED:  GameScoreInstance->GetScoreWidget()->UpdateBlueBar(GameScoreInstance->GetScoreWidget()->UpdateValue(E_BLUE, 1)); break;
		default:														   break;
	}

}
#pragma endregion

#pragma region Getter & Setter
float AMilitaryCharacter::GetHealth() const
{
	return CurrentHealth;
}

bool AMilitaryCharacter::IsAlive() const
{
	return !(CurrentHealth <= 0);
}

float AMilitaryCharacter::GetUltimate() const
{
	return CurrentUltimate;
}

float AMilitaryCharacter::GetMaxUltimate() const
{
	return MaxUltimate;
}

int AMilitaryCharacter::GetDeaths() const
{
	return CurrentDeaths;
}

int AMilitaryCharacter::GetKills() const
{
	return CurrentKills;
}

int AMilitaryCharacter::GetAssists() const
{
	return CurrentAssists;
}

void AMilitaryCharacter::AddDeaths(const int& Amount)
{
	CurrentDeaths += Amount;
	PlayerInterfaceWidget->GetStats()->UpdateText(E_DEATHS, FText::AsNumber(CurrentDeaths));
}

void AMilitaryCharacter::AddAssists(const int& Amount)
{
	CurrentAssists += Amount;
	PlayerInterfaceWidget->GetStats()->UpdateText(E_ASSISTS, FText::AsNumber(CurrentAssists));
}

void AMilitaryCharacter::SetInvincibility(const bool& Condition)
{
	Invincibility = Condition;
}

void AMilitaryCharacter::AddKills(const int& Amount)
{
	CurrentKills += Amount;
	PlayerInterfaceWidget->GetStats()->UpdateText(E_KILLS, FText::AsNumber(CurrentKills));
}

void AMilitaryCharacter::SetUltimateGauge(const float& Value)
{
	CurrentUltimate = Value;
	if(CurrentUltimate > MaxUltimate) CurrentUltimate = MaxUltimate;
	
	PlayerInterfaceWidget->GetUltimateBar()->UpdateProgressBar(CurrentUltimate, MaxUltimate);
}

bool AMilitaryCharacter::IsUltimateAvailable() const
{
	return CurrentUltimate >= MaxUltimate;
}

bool AMilitaryCharacter::IsHoldingWeapon() const
{
	return CurrentWeapon != nullptr;
}

bool AMilitaryCharacter::IsInvincible() const
{
	return Invincibility;
}

AWeaponActor* AMilitaryCharacter::GetWeapon() const
{
	return CurrentWeapon;
}

void AMilitaryCharacter::SetWeapon(AWeaponActor* Weapon)
{
	CurrentWeapon = Weapon;
}

UCameraComponent* AMilitaryCharacter::GetCamera() const
{
	return CameraComponent;
}

void AMilitaryCharacter::SetTeam(const ESpawnTeam& Team)
{
	CurrentTeam = Team;
	SpawnAtAssignedTeamPoint();
}

void AMilitaryCharacter::SetHealthGauge(const float& Value)
{
	CurrentHealth = Value;
	if(CurrentHealth > MaxHealth) CurrentHealth = MaxHealth;
	
	PlayerInterfaceWidget->GetHealthBar()->UpdateProgressBar(CurrentHealth, MaxHealth);
}

ESpawnTeam AMilitaryCharacter::GetTeam() const
{
	return CurrentTeam;
}

UPlayerWidget* AMilitaryCharacter::GetPlayerWidget() const
{
	return IsValid(PlayerInterfaceWidget) ? PlayerInterfaceWidget : nullptr;
}

AScoreActor* AMilitaryCharacter::GetGameScore()
{
	return IsValid(GameScoreInstance) ? GameScoreInstance : nullptr;
}

float AMilitaryCharacter::GetMaxHealth() const
{
	return MaxHealth;
}
#pragma endregion 