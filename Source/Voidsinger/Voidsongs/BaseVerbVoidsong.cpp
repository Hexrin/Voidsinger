// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidsongs/BaseVerbVoidsong.h"
#include "Kismet/GameplayStatics.h"


/*
* Why does this function exist? It sets variables that arent used anywhere and is only called where
* it could be replaced with Activate(so long as you make Activate blueprint callable).
* Function comments from the .h should be copied to the .cpp
* - Liam Suggestion
*/
void UBaseVerbVoidsong::PreActivate(AActor* NewActorHit, AActor* NewActorThatActivated, TArray<UBasePart*> NewPartsHit, FVector NewLocationCalledFrom, FVector NewWorldLocation, float NewEffectiveness, float NewDuration)
{
	
	ActorHit = NewActorHit;
	WorldLocation = NewWorldLocation;
	Effectiveness = NewEffectiveness;
	ActorThatActivated = NewActorThatActivated;
	LocationCalledFrom = NewLocationCalledFrom;
	PartsHit = NewPartsHit;
	FullVoidsongLength = NewDuration;

	Activate(ActorHit, NewActorThatActivated, NewPartsHit, NewLocationCalledFrom, NewWorldLocation, NewEffectiveness, NewDuration);

}

