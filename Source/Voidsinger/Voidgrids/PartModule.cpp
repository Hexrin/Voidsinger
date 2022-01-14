// Fill out your copyright notice in the Description page of Project Settings.


#include "PartModule.h"

void UPartModule::Activate(float Effectivness)
{
	OnActivate();
}

void UPartModule::Activate(const TArray<EFactions>& Factions, const TArray<TSubclassOf<UPart>>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, const TArray<TSubclassOf<UBaseVoidsong>>& PlayableVoidsongs, float Effectivness)
{
	//((Factions.IsEmpty() && AvailableFactions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) != Factions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) && NounsCheck
	
	for (TSubclassOf<UBaseVoidsong> EachPlayableVoidsong : PlayableVoidsongs)
	{
		if (EachPlayableVoidsong->IsChildOf(UBaseFactionVoidsong::StaticClass()))
		{

		}
	}
	bool bFactionsCheck = ((Factions.IsEmpty() && PlayableVoidsongs.Contains()))
	if (Factions.IsEmpty())
}
