// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidsongBlueprintUtilityLibrary.h"

/* ---------------------------- *\
\* \/ Emplace singular Motif \/ */

/**
 * Adds a Motif of unknown type to the Voidsong
 *
 * @param VoidsongData - The struct to edit
 * @param AddedMotif - The Motif added
 */
void UVoidsongBlueprintUtilityLibrary::EmplaceMotif(FVoidsongData VoidsongData, UBaseMotif* MotifAdded)
{
	VoidsongData.EmplaceMotif(MotifAdded);
}

/**
 * Adds a Faction Motif to the Voidsong
 *
 * @param VoidsongData - The struct to edit
 * @param AddedFactionMotif - The Faction Motif added
 */
void UVoidsongBlueprintUtilityLibrary::EmplaceFactionMotif(FVoidsongData VoidsongData, UBaseFactionMotif* AddedFactionMotif)
{
	VoidsongData.EmplaceFactionMotif(AddedFactionMotif);
}

/**
 * Adds a Noun Motif to the Voidsong
 *
 * @param VoidsongData - The struct to edit
 * @param AddedNounMotif - The Noun Motif added
 */
void UVoidsongBlueprintUtilityLibrary::EmplaceNounMotif(FVoidsongData VoidsongData, UBaseNounMotif* AddedNounMotif)
{
	VoidsongData.EmplaceNounMotif(AddedNounMotif);
}

/**
 * Adds a Verb Motif to the Voidsong
 *
 * @param VoidsongData - The struct to edit
 * @param AddedVerbMotif - The Verb Motif added
 */
void UVoidsongBlueprintUtilityLibrary::EmplaceVerbMotif(FVoidsongData VoidsongData, UBaseVerbMotif* AddedVerbMotif)
{
	VoidsongData.EmplaceVerbMotif(AddedVerbMotif);
}

/* /\ Emplace singular Motif /\ *\
\* ---------------------------- */

/* ----------------------------- *\
\* \/ Emplace multiple Motifs \/ */

/**
 * Adds multiple Motifs of unknown type to the Voidsong
 *
 * @param VoidsongData - The struct to edit
 * @param AddedMotifs - The Motifs added
 */
void UVoidsongBlueprintUtilityLibrary::EmplaceMotifs(FVoidsongData VoidsongData, TArray<UBaseMotif*> AddedMotifs)
{
	VoidsongData.EmplaceMotifs(AddedMotifs);
}

/**
 * Adds multiple Faction Motifs to the Voidsong
 *
 * @param VoidsongData - The struct to edit
 * @param AddedFactionMotifs - The Faction Motifs added
 */
void UVoidsongBlueprintUtilityLibrary::EmplaceFactionMotifs(FVoidsongData VoidsongData, TSet<UBaseFactionMotif*> AddedFactionMotifs)
{
	VoidsongData.EmplaceFactionMotifs(AddedFactionMotifs);
}

/**
 * Adds multiple Noun Motifs to the Voidsong
 *
 * @param VoidsongData - The struct to edit
 * @param AddedNounMotifs - The Noun Motifs added
 */
void UVoidsongBlueprintUtilityLibrary::EmplaceNounMotifs(FVoidsongData VoidsongData, TSet<UBaseNounMotif*> AddedNounMotifs)
{
	VoidsongData.EmplaceNounMotifs(AddedNounMotifs);
}

/**
 * Adds multiple Verb Motifs to the Voidsong
 *
 * @param VoidsongData - The struct to edit
 * @param AddedVerbMotifs - The Verb Motifs added
 */
void UVoidsongBlueprintUtilityLibrary::EmplaceVerbMotifs(FVoidsongData VoidsongData, TArray<UBaseVerbMotif*> AddedVerbMotifs)
{
	VoidsongData.EmplaceVerbMotifs(AddedVerbMotifs);
}

/* /\ Emplace multiple Motifs /\ *\
\* ----------------------------- */

/* ------------- *\
\* \/ Getters \/ */

/**
 * Gets a copy of all the Motifs in the Voidsong
 *
 * @param VoidsongData - The struct to get the Motifs of
 *
 * @return - Copy of Motifs
 */
TArray<UBaseMotif*> UVoidsongBlueprintUtilityLibrary::GetMotifs(FVoidsongData VoidsongData)
{
	return VoidsongData.GetMotifs();
}

/**
 * Gets a copy of the Faction Motifs in the Voidsong
 *
 * @param VoidsongData - The struct to get the Faction Motifs of
 *
 * @return - Copy of FactionMotifs
 */
TSet<UBaseFactionMotif*> UVoidsongBlueprintUtilityLibrary::GetFactionMotifs(FVoidsongData VoidsongData)
{
	return VoidsongData.GetFactionMotifs();
}

/**
 * Gets a copy of the Noun Motifs in the Voidsong
 *
 * @param VoidsongData - The struct to get the Noun Motifs of
 *
 * @return - Copy of NounMotifs
 */
TSet<UBaseNounMotif*> UVoidsongBlueprintUtilityLibrary::GetNounMotifs(FVoidsongData VoidsongData)
{
	return VoidsongData.GetNounMotifs();
}

/**
 * Gets a copy of the Verb Motifs in the Voidsong
 *
 * @param VoidsongData - The struct to get the Verb Motifs of
 *
 * @return - Copy of VerbMotifs
 */
TArray<UBaseVerbMotif*> UVoidsongBlueprintUtilityLibrary::GetVerbMotifs(FVoidsongData VoidsongData)
{
	return VoidsongData.GetVerbMotifs();
}

/**
 * Gets a copy of the Factions (enum) in the Voidsong
 *
 * @param VoidsongData - The struct to get the Factions of
 *
 * @return - Copy of Factions
 */
TSet<EFaction> UVoidsongBlueprintUtilityLibrary::GetFactions(FVoidsongData VoidsongData)
{
	return VoidsongData.GetFactions();
}

/**
 * Gets a copy of the Nouns (enum) in the Voidsong
 *
 * @param VoidsongData - The struct to get the Nouns of
 * 
 * @return - Copy of Nouns
 */
TSet<ENoun> UVoidsongBlueprintUtilityLibrary::GetNouns(FVoidsongData VoidsongData)
{
	return VoidsongData.GetNouns();
}

/*
 * Gets the duration of this Voidsong
 * 
 * @param VoidsongData - The struct to get the duration of
 * 
 * @return - The duration of the Voidsong
 */
float UVoidsongBlueprintUtilityLibrary::GetDuration(FVoidsongData VoidsongData)
{
	return VoidsongData.GetDuration();
}

/* /\ Getters /\ *\
\* ------------- */

/**
 * Plays Verbs Motifs with the given information.
 *
 * @param Verbs - The verbs that were played
 * @param Instigator - The object that activated these Motifs
 * @param Hit - The hit result information
 * @param Effectiveness - The effectiveness of the activation
 */
void UVoidsongBlueprintUtilityLibrary::ExecuteVerbMotifs(const TArray<UBaseVerbMotif*>& Verbs, UObject* Instigator, FHitResult Hit, float Effectiveness)
{
	for (UBaseVerbMotif* EachVerb : Verbs)
	{
		EachVerb->ApplyEffect(Instigator, Hit, Effectiveness);
	}
}