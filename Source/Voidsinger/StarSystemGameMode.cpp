// Fill out your copyright notice in the Description page of Project Settings.

#include "StarSystemGameMode.h"
#include "Ships/PlayerShip.h"

/*
* Function comments from the .h should be copied to the .cpp 
* 
* You may not need to define this function if you mark it as abstract?
* - Liam Suggestion
*/ 
void IActivateInterface::Activate(float Duration)
{
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
AStarSystemGameMode::AStarSystemGameMode()
{
	//LoadConfig();
	DefaultPawnClass = APlayerShip::StaticClass();
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void AStarSystemGameMode::ActivateWithEffects(AActor* ActorHit, AActor* ActorThatActivated, TArray<UBasePart*> PartsHit, FVector LocationCalledFrom, FVector WorldLocation, float Effectiveness, float Duration)
{
	//UE_LOG(LogTemp, Warning, TEXT("Verbiness is called"));
	if (!VerbsActive.IsEmpty())
	{
		//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
		for (auto& i : VerbsActive)
		{
			i->PreActivate(ActorHit, ActorThatActivated, PartsHit, LocationCalledFrom, WorldLocation, Effectiveness, Duration);
		}
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void AStarSystemGameMode::Broadcast(TArray<TEnumAsByte<EFactions>> Factions, TArray<TSubclassOf<UObject>> NounClasses, TArray<UBaseVerbVoidsong*> Verbs, TArray<UBaseVoidsong*> AvailableVoidsongs, float Duration)
{
	//Shouldnt this be append(+=) not = - Liam Suggestion
	VerbsActive = Verbs;
	OnVoidsongDelegate.Broadcast(Factions, NounClasses, AvailableVoidsongs, Duration);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void AStarSystemGameMode::UnsetVerbs()
{
	//Delete print string or come up with a good way for c++ debug modes
	//UE_LOG(LogTemp, Warning, TEXT("UnsetVerbs"));
	for (auto& i : VerbsActive)
	{
		i->Deactivate();
	}

	VerbsActive.Empty();

	OnDeactivateVoidsongDelegate.Broadcast();

}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
FVoidsongDelegate AStarSystemGameMode::GetVoidsongDelegate()
{
	return OnVoidsongDelegate;
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
TArray<UBaseVerbVoidsong*> AStarSystemGameMode::GetVerbsActive()
{
	return VerbsActive;
}