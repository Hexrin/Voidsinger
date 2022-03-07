// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Motifs/BaseMotif.h"
#include "Motifs/Factions/BaseFactionMotif.h"
#include "Motifs/Nouns/BaseNounMotif.h"
#include "Motifs/Verbs/BaseVerbMotif.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "VoidsongData.generated.h"

/**
 * Stores Motifs that are played in a Voidsong
 */
USTRUCT(BlueprintType)
struct VOIDSINGER_API FVoidsongData
{

	GENERATED_BODY()

	/* ------------ *\
	\* \/ Motifs \/ */

private:

	//All Motifs played
	TArray<UBaseMotif*> Motifs = TArray<UBaseMotif*>();

	//Faction Motifs played
	TSet<UBaseFactionMotif*> FactionMotifs = TSet<UBaseFactionMotif*>();

	//Noun Motifs played
	TSet<UBaseNounMotif*> NounMotifs = TSet<UBaseNounMotif*>();

	//Verb Motifs played
	TArray<UBaseVerbMotif*> VerbMotifs = TArray<UBaseVerbMotif*>();

	/* /\ Motifs /\ *\
	\* ------------ */

	/* ----------- *\
	\* \/ Enums \/ */

private:

	//Factions played (enum)
	TSet<EFaction> Factions = TSet<EFaction>();

	//Nouns played (enum)
	TSet<ENoun> Nouns = TSet<ENoun>();

	/* /\ Enums /\ *\
	\* ----------- */

	/* -------------- *\
	\* \/ Duration \/ */

private:

	float Duration = 0;

	/* /\ Duration /\ *\
	\* -------------- */

	/* ------------------ *\
	\* \/ Constructors \/ */

public:

	/**
	 * Default constructor
	 */
	FVoidsongData()
	{
		FVoidsongData(TSet<UBaseFactionMotif*>(), TSet<UBaseNounMotif*>(), TArray<UBaseVerbMotif*>());
	}

	/**
	 * Constructor taking in Faction Motifs, Noun Motifs, and Verb Motifs
	 * 
	 * @param FactionMotifsInVoidsong - The Faction Motifs played in this Voidsong
	 * @param NounMotifsInVoisdong - The Noun Motifs played in this Voidsong
	 * @param VerbMotifsInVoidsong - The Verb Motifs played in this Voidsong
	 */
	FVoidsongData(TSet<UBaseFactionMotif*> FactionMotifsInVoidsong, TSet<UBaseNounMotif*> NounMotifsInVoidsong, TArray<UBaseVerbMotif*> VerbMotifsInVoidsong)
	{
		EmplaceFactionMotifs(FactionMotifsInVoidsong);
		EmplaceNounMotifs(NounMotifsInVoidsong);
		EmplaceVerbMotifs(VerbMotifsInVoidsong);
	}

	/* /\ Constructors /\ *\
	\* ------------------ */

	/* ---------------------------- *\
	\* \/ Emplace singular Motif \/ */

	/**
	 * Adds a Motif of unknown type to the Voidsong
	 * 
	 * @param AddedMotif - The Motif added
	 */
	void EmplaceMotif(UBaseMotif* MotifAdded)
	{
		if (IsValid(Cast<UBaseFactionMotif>(MotifAdded)))
		{
			EmplaceFactionMotif(Cast<UBaseFactionMotif>(MotifAdded));
		}
		else if (IsValid(Cast<UBaseNounMotif>(MotifAdded)))
		{
			EmplaceNounMotif(Cast<UBaseNounMotif>(MotifAdded));
		}
		else if (IsValid(Cast<UBaseVerbMotif>(MotifAdded)))
		{
			EmplaceVerbMotif(Cast<UBaseVerbMotif>(MotifAdded));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Added Motif of unknown type"));
		}
	}

	/**
	 * Adds a Faction Motif to the Voidsong
	 * 
	 * @param AddedFactionMotif - The Faction Motif added
	 */
	void EmplaceFactionMotif(UBaseFactionMotif* AddedFactionMotif)
	{
		Motifs.Emplace(AddedFactionMotif);
		FactionMotifs.Emplace(AddedFactionMotif);
		Factions.Emplace(AddedFactionMotif->Faction);
		Duration += AddedFactionMotif->Duration;
	}

	/**
	 * Adds a Noun Motif to the Voidsong
	 *
	 * @param AddedNounMotif - The Noun Motif added
	 */
	void EmplaceNounMotif(UBaseNounMotif* AddedNounMotif)
	{
		Motifs.Emplace(AddedNounMotif);
		NounMotifs.Emplace(AddedNounMotif);
		Nouns.Emplace(AddedNounMotif->Noun);
		Duration += AddedNounMotif->Duration;
	}

	/**
	 * Adds a Verb Motif to the Voidsong
	 *
	 * @param AddedVerbMotif - The Verb Motif added
	 */
	void EmplaceVerbMotif(UBaseVerbMotif* AddedVerbMotif)
	{
		Motifs.Emplace(AddedVerbMotif);
		VerbMotifs.Emplace(AddedVerbMotif);
		Duration += AddedVerbMotif->Duration;
	}

	/* /\ Emplace singular Motif /\ *\
	\* ---------------------------- */

	/* ----------------------------- *\
	\* \/ Emplace multiple Motifs \/ */

	/**
	 * Adds multiple Motifs of unknown type to the Voidsong
	 * 
	 * @param AddedMotifs - The Motifs added
	 */
	void EmplaceMotifs(TArray<UBaseMotif*> AddedMotifs)
	{
		for (UBaseMotif* EachAddedMotif : AddedMotifs)
		{
			EmplaceMotif(EachAddedMotif);
		}
	}

	/**
	 * Adds multiple Faction Motifs to the Voidsong
	 *
	 * @param AddedFactionMotifs - The Faction Motifs added
	 */
	void EmplaceFactionMotifs(TSet<UBaseFactionMotif*> AddedFactionMotifs)
	{
		for (UBaseFactionMotif* EachAddedFactionMotif : AddedFactionMotifs)
		{
			EmplaceFactionMotif(EachAddedFactionMotif);
		}
	}

	/**
	 * Adds multiple Noun Motifs to the Voidsong
	 *
	 * @param AddedNounMotifs - The Noun Motifs added
	 */
	void EmplaceNounMotifs(TSet<UBaseNounMotif*> AddedNounMotifs)
	{
		for (UBaseNounMotif* EachAddedNounMotif : AddedNounMotifs)
		{
			EmplaceNounMotif(EachAddedNounMotif);
		}
	}

	/**
	 * Adds multiple Verb Motifs to the Voidsong
	 *
	 * @param AddedVerbMotifs - The Verb Motifs added
	 */
	void EmplaceVerbMotifs(TArray<UBaseVerbMotif*> AddedVerbMotifs)
	{
		for (UBaseVerbMotif* EachAddedVerbMotif : AddedVerbMotifs)
		{
			EmplaceVerbMotif(EachAddedVerbMotif);
		}
	}

	/* /\ Emplace multiple Motifs /\ *\
	\* ----------------------------- */

	/* ------------- *\
	\* \/ Getters \/ */

	/**
	 * Gets a copy of all the Motifs in the Voidsong
	 * 
	 * @return - Copy of Motifs
	 */
	TArray<UBaseMotif*> GetMotifs() const
	{
		return TArray<UBaseMotif*>(Motifs);
	}

	/**
	 * Gets a copy of the Faction Motifs in the Voidsong
	 *
	 * @return - Copy of FactionMotifs
	 */
	TSet<UBaseFactionMotif*> GetFactionMotifs() const
	{
		return TSet<UBaseFactionMotif*>(FactionMotifs);
	}

	/**
	 * Gets a copy of the Noun Motifs in the Voidsong
	 *
	 * @return - Copy of NounMotifs
	 */
	TSet<UBaseNounMotif*> GetNounMotifs() const
	{
		return TSet<UBaseNounMotif*>(NounMotifs);
	}

	/**
	 * Gets a copy of all the Verb Motifs in the Voidsong
	 *
	 * @return - Copy of VerbMotifs
	 */
	TArray<UBaseVerbMotif*> GetVerbMotifs() const
	{
		return TArray<UBaseVerbMotif*>(VerbMotifs);
	}

	/**
	 * Gets a copy of the Factions (enum) in the Voidsong
	 *
	 * @return - Copy of Factions
	 */
	TSet<EFaction> GetFactions() const
	{
		return TSet<EFaction>(Factions);
	}

	/**
	 * Gets a copy of the Nouns (enum) in the Voidsong
	 *
	 * @return - Copy of Nouns
	 */
	TSet<ENoun> GetNouns() const
	{
		return TSet<ENoun>(Nouns);
	}

	/*
	 * Gets the duration of this Voidsong
	 * 
	 * @return - The duration of the Voidsong
	 */
	float GetDuration() const
	{
		return Duration;
	}

	/* /\ Getters /\ *\
	\* ------------- */
};
