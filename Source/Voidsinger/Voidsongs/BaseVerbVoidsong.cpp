// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidsongs/BaseVerbVoidsong.h"
#include "Kismet/GameplayStatics.h"

void UBaseVerbVoidsong::PreActivate(AActor* NewActorHit, AActor* NewActorThatActivated, TArray<UBasePart*> NewPartsHit, FVector NewLocationCalledFrom, FVector NewWorldLocation, float NewEffectiveness)
{
	
	ActorHit = NewActorHit;
	WorldLocation = NewWorldLocation;
	Effectiveness = NewEffectiveness;
	ActorThatActivated = NewActorThatActivated;
	LocationCalledFrom = NewLocationCalledFrom;
	PartsHit = NewPartsHit;

	Activate(ActorHit, NewActorThatActivated, NewPartsHit, NewLocationCalledFrom, NewWorldLocation, NewEffectiveness);

}

