// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Voidsinger/StarSystemGameMode.h"
#include "Voidsinger/Voidsongs/Voidsong.h"
#include "Kismet/GameplayStatics.h"

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
void AShip::AddNewMotifs(TArray<UBaseMotif*> MotifsAdded)
{
	for (UBaseMotif* EachMotifAdded : MotifsAdded)
	{
		PlayableMotifs.Emplace(EachMotifAdded);
	}
}

/* /\ Playable Voidsong Management /\ *\
\* ---------------------------------- */

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
 * Plays a Voidsong with the given Motifs
 *
 * @param Factions - The Faction Motifs played
 * @param Nouns - The Noun Motifs played
 * @param Verbs - The Verb Motifs played
 */
UVoidsong* AShip::PlayVoidsong(const TArray<UBaseFactionMotif*>& Factions, const TArray<UBaseNounMotif*>& Nouns, const TArray<UBaseVerbMotif*>& Verbs)
{
	if (bCanPlayVoidsongs)
	{

		// \/ Check if Factions is empty, if so play every playable Faction \/ /

		TArray<UBaseFactionMotif*> FactionsToPlay;

		if (!Factions.IsEmpty())
		{
			FactionsToPlay = Factions;
		}
		else
		{
			for (UBaseMotif* EachPlayableMotif : PlayableMotifs)
			{
				if (IsValid(Cast<UBaseFactionMotif>(EachPlayableMotif)))
				{
					FactionsToPlay.Emplace(Cast<UBaseFactionMotif>(EachPlayableMotif));
				}
			}
		}

		// /\ Check if Factions is empty, if so play every playable Faction /\ /

		// \/ Check if Nouns is empty, if so play every playable Noun \/ /

		TArray<UBaseNounMotif*> NounsToPlay;

		if (!Nouns.IsEmpty())
		{
			NounsToPlay = Nouns;
		}
		else
		{
			for (UBaseMotif* EachPlayableMotif : PlayableMotifs)
			{
				if (IsValid(Cast<UBaseNounMotif>(EachPlayableMotif)))
				{
					NounsToPlay.Emplace(Cast<UBaseNounMotif>(EachPlayableMotif));
				}
			}
		}

		// /\ Check if Nouns is empty, if so play every playable Noun /\ /

		//Initialize VoidsongData
		FVoidsongData VoidsongData = FVoidsongData(FactionsToPlay, NounsToPlay, Verbs);

		//Call the globally available "PlayVoidsong" function on the Game Mode
		if (IsValid(Cast<AStarSystemGameMode>(UGameplayStatics::GetGameMode(GetWorld()))))
		{
			return Cast<AStarSystemGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->VoidsongManager->PlayVoidsong(VoidsongData);
		}
		else
		{
			//Print an error message if unable to play due to invalid game mode or world
			UE_LOG(LogTemp, Warning, TEXT("Failed to play Voidsong. Either the world or the game mode is null on ship %s"), *GetFName().ToString());
		}
		// \/ Make this ship unable to play Voidsongs until the duration is up \/ /

		DeactivatePlayingVoidsongs();
		FTimerHandle DurationTimer;
		GetWorld()->GetTimerManager().SetTimer(DurationTimer, this, &AShip::ActivatePlayingVoidsongs, VoidsongData.GetDuration());

		// /\ Make this ship unable to play Voidsongs until the duration is up /\ /

	}

	//Return nullptr if no Voidsong is played
	return nullptr;

}

/*
 * Deactivates this ship's ability to play Voidsongs
 */
void AShip::DeactivatePlayingVoidsongs()
{
	bCanPlayVoidsongs = false;
}

/**
 * Activates this ship's ability to play Voidsongs
 */
void AShip::ActivatePlayingVoidsongs()
{
	bCanPlayVoidsongs = true;
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */


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