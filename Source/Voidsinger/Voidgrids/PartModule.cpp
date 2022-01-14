// Fill out your copyright notice in the Description page of Project Settings.


#include "PartModule.h"

void UPartModule::Activate(float Effectiveness)
{
	TArray<TSubclassOf<UBaseVerbVoidsong>> EmptyVerbArray;
	OnActivate(EmptyVerbArray, Effectiveness);
}

void UPartModule::Activate(const TArray<EFactions>& Factions, const TArray<TSubclassOf<UPart>>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, const TArray<TSubclassOf<UBaseVoidsong>>& PlayableVoidsongs, float Effectiveness)
{
	//((Factions.IsEmpty() && AvailableFactions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) != Factions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) && NounsCheck
	
	TArray<EFactions> PlayableFactions;
	TArray <TSubclassOf<UPart>> PlayableNouns;

	for (TSubclassOf<UBaseVoidsong> EachPlayableVoidsong : PlayableVoidsongs)
	{
		if (EachPlayableVoidsong->IsChildOf(UBaseFactionVoidsong::StaticClass()))
		{
			PlayableFactions.Emplace(Cast<UBaseFactionVoidsong>(EachPlayableVoidsong->GetDefaultObject())->Faction);
		}
		if (EachPlayableVoidsong->IsChildOf(UBaseNounVoidsong::StaticClass()))
		{
			PlayableNouns.Emplace(Cast<UBaseNounVoidsong>(EachPlayableVoidsong->GetDefaultObject())->Noun);
		}
	}
	//bool bFactionsCheck = ((Factions.IsEmpty() && PlayableFactions.Contains()))
	//if (Factions.IsEmpty())
}
