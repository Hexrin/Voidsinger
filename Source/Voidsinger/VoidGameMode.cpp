// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipPlayerState.h"
#include "Ships/PlayerShip.h"
#include "VoidGameMode.h"

void IActivateInterface::Activate()
{
}

AVoidGameMode::AVoidGameMode()
{
	//LoadConfig();
	PlayerStateClass = AShipPlayerState::StaticClass();
	DefaultPawnClass = APlayerShip::StaticClass();
}

void AVoidGameMode::ActivateWithEffects(AActor* ActorHit, AActor* ActorThatActivated, TArray<UBasePart*> PartsHit, FVector LocationCalledFrom, FVector WorldLocation, float Effectiveness)
{
	//UE_LOG(LogTemp, Warning, TEXT("Verbiness is called"));
	if (!VerbsActive.IsEmpty())
	{
		for (auto& i : VerbsActive)
		{
			i->PreActivate(ActorHit, ActorThatActivated, PartsHit, LocationCalledFrom, WorldLocation, Effectiveness);
		}
	}
}

void AVoidGameMode::Broadcast(TArray<TEnumAsByte<EFactions>> Factions, TArray<TSubclassOf<UObject>> NounClasses, TArray<UBaseVerbVoidsong*> Verbs)
{
	VerbsActive = Verbs;
	OnVoidsongDelegate.Broadcast(Factions, NounClasses);
}

void AVoidGameMode::UnsetVerbs()
{
	for (auto& i : VerbsActive)
	{
		i->Deactivate();
	}

	VerbsActive.Empty();

	OnDeactivateVoidsongDelegate.Broadcast();

}

FVoidsongDelegate AVoidGameMode::GetVoidsongDelegate()
{
	return OnVoidsongDelegate;
}
