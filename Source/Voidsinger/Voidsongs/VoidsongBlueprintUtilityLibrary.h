// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoidsongData.h"
#include "VoidsongBlueprintUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UVoidsongBlueprintUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/* ---------------------------- *\
	\* \/ Emplace singular Motif \/ */

	/**
	 * Adds a Faction Motif to the Voidsong
	 *
	 * @param VoidsongData - The struct to edit
	 * @param AddedFactionMotif - The Faction Motif added
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongData|EmplaceSingularMotif")
	static void EmplaceFactionMotif(FVoidsongData VoidsongData, UBaseFactionMotif* AddedFactionMotif);

	/**
	 * Adds a Noun Motif to the Voidsong
	 *
	 * @param VoidsongData - The struct to edit
	 * @param AddedNounMotif - The Noun Motif added
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongData|EmplaceSingularMotif")
	static void EmplaceNounMotif(FVoidsongData VoidsongData, UBaseNounMotif* AddedNounMotif);

	/**
	 * Adds a Verb Motif to the Voidsong
	 *
	 * @param VoidsongData - The struct to edit
	 * @param AddedVerbMotif - The Verb Motif added
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongData|EmplaceSingularMotif")
	static void EmplaceVerbMotif(FVoidsongData VoidsongData, UBaseVerbMotif* AddedVerbMotif);

	/* /\ Emplace singular Motif /\ *\
	\* ---------------------------- */

	/* ----------------------------- *\
	\* \/ Emplace multiple Motifs \/ */

	/**
	 * Adds multiple Faction Motifs to the Voidsong
	 *
	 * @param VoidsongData - The struct to edit
	 * @param AddedFactionMotifs - The Faction Motifs added
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongData|EmplaceMultipleMotifs")
	static void EmplaceFactionMotifs(FVoidsongData VoidsongData, TArray<UBaseFactionMotif*> AddedFactionMotifs);

	/**
	 * Adds multiple Noun Motifs to the Voidsong
	 *
	 * @param VoidsongData - The struct to edit
	 * @param AddedNounMotifs - The Noun Motifs added
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongData|EmplaceMultipleMotifs")
	static void EmplaceNounMotifs(FVoidsongData VoidsongData, TArray<UBaseNounMotif*> AddedNounMotifs);

	/**
	 * Adds multiple Verb Motifs to the Voidsong
	 *
	 * @param VoidsongData - The struct to edit
	 * @param AddedVerbMotifs - The Verb Motifs added
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongData|EmplaceMultipleMotifs")
	static void EmplaceVerbMotifs(FVoidsongData VoidsongData, TArray<UBaseVerbMotif*> AddedVerbMotifs);

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
	UFUNCTION(BlueprintPure, Category = "VoidsongData|Getters")
	static TArray<UBaseMotif*> GetMotifs(FVoidsongData VoidsongData);

	/**
	 * Gets a copy of the Faction Motifs in the Voidsong
	 *
	 * @param VoidsongData - The struct to get the Faction Motifs of
	 * 
	 * @return - Copy of FactionMotifs
	 */
	UFUNCTION(BlueprintPure, Category = "VoidsongData|Getters")
	static TArray<UBaseFactionMotif*> GetFactionMotifs(FVoidsongData VoidsongData);

	/**
	 * Gets a copy of the Noun Motifs in the Voidsong
	 *
	 * @param VoidsongData - The struct to get the Noun Motifs of
	 * 
	 * @return - Copy of NounMotifs
	 */
	UFUNCTION(BlueprintPure, Category = "VoidsongData|Getters")
	static TArray<UBaseNounMotif*> GetNounMotifs(FVoidsongData VoidsongData);

	/**
	 * Gets a copy of all the Verb Motifs in the Voidsong
	 *
	 * @param VoidsongData - The struct to get the Verb Motifs of
	 * 
	 * @return - Copy of VerbMotifs
	 */
	UFUNCTION(BlueprintPure, Category = "VoidsongData|Getters")
	static TArray<UBaseVerbMotif*> GetVerbMotifs(FVoidsongData VoidsongData);

	/**
	 * Gets a copy of the Factions (enum) in the Voidsong
	 *
	 * @param VoidsongData - The struct to get the Factions of
	 * 
	 * @return - Copy of Factions
	 */
	UFUNCTION(BlueprintPure, Category = "VoidsongData|Getters")
	static TArray<EFaction> GetFactions(FVoidsongData VoidsongData);

	/**
	 * Gets a copy of the Nouns (enum) in the Voidsong
	 *
	 * @param VoidsongData - The struct to get the Nouns of
	 * 
	 * @return - Copy of Nouns
	 */
	UFUNCTION(BlueprintPure, Category = "VoidsongData|Getters")
	static TArray<ENoun> GetNouns(FVoidsongData VoidsongData);

	/*
	 * Gets the duration of this Voidsong
	 * 
	 * @param VoidsongData - The struct to get the duration of
	 * 
	 * @return - The duration of the Voidsong
	 */
	UFUNCTION(BlueprintPure, Category = "VoidsongData|Getters")
	static float GetDuration(FVoidsongData VoidsongData);

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
	UFUNCTION(BlueprintCallable)
	static void ExecuteVerbMotifs(const TArray<UBaseVerbMotif*>& Verbs, UObject* Instigator, FHitResult Hit, float Effectiveness);

};
