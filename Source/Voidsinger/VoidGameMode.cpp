// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidGameMode.h"

void IActivateInterface::Activate()
{
}

AVoidGameMode::AVoidGameMode()
{
	//LoadConfig();
}

int AVoidGameMode::PrintTestaroo()
{
	UE_LOG(LogTemp, Warning, TEXT("Testaroo = %i"), Testaroo)
	return Testaroo;
}

void AVoidGameMode::ActivateWithEffects(UObject* ThingActivated)
{
	if (ThingActivated->Implements<UActivateInterface>())
	{
		Cast<IActivateInterface>(ThingActivated)->Activate();
	}
}

void AVoidGameMode::Broadcast(TEnumAsByte<EFactions> Faction, TSubclassOf<UObject> NounClass, TSubclassOf<UBaseVerbVoidsong> Verb)
{
	VerbActive = Verb;
	OnVoidsongDelegate.Broadcast(Faction, NounClass);
}

FVoidsongDelegate AVoidGameMode::GetVoidsongDelegate()
{
	return OnVoidsongDelegate;
}
