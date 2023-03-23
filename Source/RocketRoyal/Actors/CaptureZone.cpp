// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureZone.h"

#include "Components/WidgetComponent.h"
#include "../Player/MilitaryCharacter.h"
#include "../Utils/Utils.h"
#include "Components/ProgressBar.h"

#pragma region  Setup
ACaptureZone::ACaptureZone()
{
	PrimaryActorTick.bCanEverTick = true;
	CaptureZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Capture Zone"));
	
	CaptureProgress = CreateDefaultSubobject<UWidgetComponent>(TEXT("Progress Widget"));
	CaptureProgress->AttachToComponent(CaptureZone, FAttachmentTransformRules::KeepRelativeTransform);
}

void ACaptureZone::BeginPlay()
{
	Super::BeginPlay();
	CaptureProgress->SetWidgetClass(CaptureWidget);

	CaptureProgress->SetRelativeLocation(FVector(0,0,-104));
	CaptureProgress->SetRelativeRotation(FRotator(90,0,0));
	
	CaptureZone->OnComponentBeginOverlap.AddDynamic(this, &ACaptureZone::OnBeginInZone);
	CaptureZone->OnComponentEndOverlap.AddDynamic(this, &ACaptureZone::OnEndInZone);
	Widget = Cast<UProgressBarComponent>(CaptureProgress->GetUserWidgetObject());
	FLag = GetWorld()->SpawnActor<AFlagActor>(FlagActorBP, GetActorLocation() + FVector(0, 0, 250), FRotator(0));
	
	CaptureZone->SetHiddenInGame(false);

	CurrentSpeed = CaptureSpeed;

	GameInstance = Cast<UPersistentGameInstance>(GetGameInstance());
}
#pragma endregion 

#pragma region Tick
void ACaptureZone::Tick(float DeltaTime)
{
	if (!actionDone)
	{
		Super::Tick(DeltaTime);
		
		switch (CurrentZoneState)
		{
		case INCREASING:
			CurrentPercent += DeltaTime/3 * CurrentSpeed;
			Widget->GetProgressBar()->SetPercent(CurrentPercent);
			if (CurrentPercent >= 1)
			{
				UpdateUlti();
				CaptureManager->BindZoneTimer();
				FLag->Destroy();
				Destroy();
			}
			break;
		case DECREASING:
			CurrentPercent -= DeltaTime/3;
			Widget->GetProgressBar()->SetPercent(CurrentPercent);
			if (CurrentPercent <= 0 )
			{
				CurrentZoneState = INCREASING;
				CurrentTeam = ListPlayersInZone[0]->GetTeam();
				SetWidgetColor(CurrentTeam);
			}
			break;
		default:
			break;
		}
	}
}

void ACaptureZone::OnBeginInZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	 AMilitaryCharacter* Player = Cast<AMilitaryCharacter>(OtherActor);
	 if (Player != nullptr)
	 {
	 	ListPlayersInZone.Add(Player);
		 switch (CurrentZoneState)
		 {
		 case NOSTATE:
			 if (CurrentTeam == E_UNDEFINED)
			 {
		 		CurrentTeam = Player->GetTeam();
		 		SetWidgetColor(CurrentTeam);
		 		SetZoneState(INCREASING);
			 	return;
			 }
			 if (CurrentTeam == Player->GetTeam())
				SetZoneState(INCREASING);
			 else
				SetZoneState(DECREASING);
		 	break;
		 case INCREASING:
			 if (CurrentTeam != Player->GetTeam())
				 SetZoneState(CONTESTED);
			 else
				 CurrentSpeed = DoubleCaptureSpeed;
		 	break;
		 case DECREASING:
		    if (ListPlayersInZone[0]->GetTeam() != Player->GetTeam())
			    SetZoneState(CONTESTED);
		 default:
		 	break;
		 }
	 }
}

void ACaptureZone::OnEndInZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMilitaryCharacter* Player = Cast<AMilitaryCharacter>(OtherActor);
    if (Player != nullptr)
    {
    	ListPlayersInZone.Remove(Player);
    	switch (CurrentZoneState)
    	{
    	case INCREASING:
			if (ListPlayersInZone.Num() == 0)
			{
				CurrentZoneState = NOSTATE;
				LastTeam = CurrentTeam;
			}
			else
				CurrentSpeed = CaptureSpeed;
    		break;
    	case DECREASING:
    		if (ListPlayersInZone.Num() == 0)
    		{
    			CurrentZoneState = NOSTATE;
    		}
    		break;
    	case CONTESTED:
	        if (!IsZoneStillContested(Player))
	        {
				if (ListPlayersInZone[0]->GetTeam() == CurrentTeam)
				{
					CurrentZoneState = INCREASING;
					if(ListPlayersInZone.Num() > 1)
						CurrentSpeed = DoubleCaptureSpeed;
				}
				else
				{
					CurrentZoneState = DECREASING;
				}
	        }
    		break;
    	default:
			DebugError("Error current zone state null");
    		break;
    	}
    }
}
#pragma endregion 

#pragma region Event
bool ACaptureZone::IsZoneStillContested(AMilitaryCharacter* OutPlayer)
{
	for (AMilitaryCharacter* player : ListPlayersInZone)
	{
		if (OutPlayer->GetTeam() == player->GetTeam())
			return true;			
	}
	return false;
}

void ACaptureZone::UpdateUlti()
{
	for (AMilitaryCharacter* p : GameInstance->MilitaryCharacters)
	{
		if(p->GetTeam() == CurrentTeam)
			p->TakeUltimateEvent(UltReward);
	}	
}
#pragma endregion 

#pragma region Getter & Setter
EZoneStatus ACaptureZone::GetZoneState() const
{
	return CurrentZoneState;
}

void ACaptureZone::SetZoneState(const EZoneStatus NewState)
{
	CurrentZoneState = NewState;
}
void ACaptureZone::SetWidgetColor(const ESpawnTeam NewColor)
{
	if(!IsValid(Widget)) return;

	
	switch (NewColor)
	{
	case E_RED:
		FLag->UpdateColorEvent(FColor::Red);
		Widget->GetProgressBar()->SetFillColorAndOpacity(FLinearColor(255,0,0,0.7));
		break;
	case E_BLUE:
		FLag->UpdateColorEvent(FColor::Blue);
		Widget->GetProgressBar()->SetFillColorAndOpacity(FLinearColor(0,0,255,0.7));
		break;
	default:
		DebugInfo("Failed to set color");
		break;
	}
}

void ACaptureZone::SetCaptureManager(ACaptureZoneManager* Manager)
{
	CaptureManager = Manager;
}
#pragma endregion