// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMotif.h"
#include "Factions/BaseFactionMotif.h"
#include "Nouns/BaseNounMotif.h"
#include "Verbs/BaseVerbMotif.h"
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
	TArray<UBaseFactionMotif*> FactionMotifs = TArray<UBaseFactionMotif*>();

	//Noun Motifs played
	TArray<UBaseNounMotif*> NounMotifs = TArray<UBaseNounMotif*>();

	//Verb Motifs played
	TArray<UBaseVerbMotif*> VerbMotifs = TArray<UBaseVerbMotif*>();

	/* /\ Motifs /\ *\
	\* ------------ */

	/* ----------- *\
	\* \/ Enums \/ */

private:

	//Factions played (enum)
	TArray<EFaction> Factions = TArray<EFaction>();

	//Nouns played (enum)
	TArray<ENoun> Nouns = TArray<ENoun>();

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
		FVoidsongData(TArray<UBaseFactionMotif*>(), TArray<UBaseNounMotif*>(), TArray<UBaseVerbMotif*>());
	}

	/**
	 * Constructor taking in Faction Motifs, Noun Motifs, and Verb Motifs
	 * 
	 * @param FactionMotifsInVoidsong - The Faction Motifs played in this Voidsong
	 * @param NounMotifsInVoisdong - The Noun Motifs played in this Voidsong
	 * @param VerbMotifsInVoidsong - The Verb Motifs played in this Voidsong
	 */
	FVoidsongData(TArray<UBaseFactionMotif*> FactionMotifsInVoidsong, TArray<UBaseNounMotif*> NounMotifsInVoidsong, TArray<UBaseVerbMotif*> VerbMotifsInVoidsong)
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
	}

	/* /\ Emplace singular Motif /\ *\
	\* ---------------------------- */

	/* ----------------------------- *\
	\* \/ Emplace multiple Motifs \/ */

	/**
	 * Adds multiple Faction Motifs to the Voidsong
	 *
	 * @param AddedFactionMotifs - The Faction Motifs added
	 */
	void EmplaceFactionMotifs(TArray<UBaseFactionMotif*> AddedFactionMotifs)
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
	void EmplaceNounMotifs(TArray<UBaseNounMotif*> AddedNounMotifs)
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
	TArray<UBaseMotif*> GetMotifs()
	{
		return TArray<UBaseMotif*>(Motifs);
	}

	/**
	 * Gets a copy of the Faction Motifs in the Voidsong
	 *
	 * @return - Copy of FactionMotifs
	 */
	TArray<UBaseFactionMotif*> GetFactionMotifs()
	{
		return TArray<UBaseFactionMotif*>(FactionMotifs);
	}

	/**
	 * Gets a copy of the Noun Motifs in the Voidsong
	 *
	 * @return - Copy of NounMotifs
	 */
	TArray<UBaseNounMotif*> GetNounMotifs()
	{
		return TArray<UBaseNounMotif*>(NounMotifs);
	}

	/**
	 * Gets a copy of all the Verb Motifs in the Voidsong
	 *
	 * @return - Copy of VerbMotifs
	 */
	TArray<UBaseVerbMotif*> GetVerbMotifs()
	{
		return TArray<UBaseVerbMotif*>(VerbMotifs);
	}

	/**
	 * Gets a copy of the Factions (enum) in the Voidsong
	 *
	 * @return - Copy of Factions
	 */
	TArray<EFaction> GetFactions()
	{
		return TArray<EFaction>(Factions);
	}

	/**
	 * Gets a copy of the Nouns (enum) in the Voidsong
	 *
	 * @return - Copy of Nouns
	 */
	TArray<ENoun> GetNouns()
	{
		return TArray<ENoun>(Nouns);
	}

	/*
	 * Gets the duration of this Voidsong
	 * 
	 * @return - The duration of the Voidsong
	 */
	float GetDuration()
	{
		return Duration;
	}

	/* /\ Getters /\ *\
	\* ------------- */
};
