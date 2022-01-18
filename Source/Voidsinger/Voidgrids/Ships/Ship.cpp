// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"

/* \/ ==== \/ *\
|  \/ Ship \/  |
\* \/ ==== \/ */

/* ---------------------------------- *\
\* \/ Playable Voidsong Management \/ */

/**
 * Adds Voidsongs to the list of Voidsongs that this component can play.
 *
 * @param VoidsongsAdded - The new playable Voidsongs
 */
void AShip::AddNewVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> VoidsongsAdded)
{
	for (TSubclassOf<UBaseVoidsong> EachVoidsongAdded : VoidsongsAdded)
	{
		PlayableVoidsongs.Emplace(EachVoidsongAdded);
	}
}

/* /\ Playable Voidsong Management /\ *\
\* ---------------------------------- */

void AShip::PlayVoidsongs(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs)
{
}

/* ----------------------- *\
\* \/ Ship State Saving \/ */

/**
 * Saves this ships state.
 */
void AShip::SaveState()
{
	if (UShipSaveState* SaveGameInstance = Cast<UShipSaveState>(UGameplayStatics::CreateSaveGameObject(UShipSaveState::StaticClass())))
	{
		SaveGameInstance->VoidgridState = GetState();
		SaveGameInstance->Location = FVector2D(GetActorLocation());

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, GetSaveStateSlotName(), 0);
	}
}

/**
 * Loads this ships state.
 */
void AShip::LoadState(bool bLoadMold)
{
	if (UShipSaveState* SaveGameInstance = Cast<UShipSaveState>(UGameplayStatics::LoadGameFromSlot(GetSaveStateSlotName(), 0)))
	{
		if (bLoadMold)
		{
			SetState(SaveGameInstance->VoidgridState);
		}
		else
		{
			SetState(FVoidgridState(GetPixelMold(), SaveGameInstance->VoidgridState.State));
		}
	}
}

/**
 * Gets the save slot name used to save this ship
 */
FString AShip::GetSaveStateSlotNamePrefix()
{
	return "Ship_";
}

/**
 * Gets the save slot name used to save this ship
 */
FString AShip::GetSaveStateSlotID()
{
	return "DefaultShip";
}

/**
 * Gets the save slot name used when loading this ship.
 */
FString AShip::GetSaveStateSlotName()
{
	return GetSaveStateSlotNamePrefix() + GetSaveStateSlotID();
}

/* /\ ==== /\ *\
|  /\ Ship /\  |
\* /\ ==== /\ */
