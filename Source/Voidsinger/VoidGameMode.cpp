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

void AVoidGameMode::ActivateWithEffects(UObject* ObjectHit)
{
	if (!VerbsActive.IsEmpty())
	{
		for (auto& i : VerbsActive)
		{

		}
	}
}

void AVoidGameMode::Broadcast(TArray<TEnumAsByte<EFactions>> Factions, TArray<TSubclassOf<UObject>> NounClasses, TArray<TSubclassOf<UBaseVerbVoidsong>> Verbs)
{
	VerbsActive = Verbs;
	OnVoidsongDelegate.Broadcast(Factions, NounClasses);
}

FVoidsongDelegate AVoidGameMode::GetVoidsongDelegate()
{
	return OnVoidsongDelegate;
}
