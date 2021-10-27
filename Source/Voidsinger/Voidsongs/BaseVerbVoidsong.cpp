// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidsongs/BaseVerbVoidsong.h"
#include "Kismet/GameplayStatics.h"

void UBaseVerbVoidsong::PreActivate(AActor* NewActorHit, FVector NewWorldLocation)
{
	
	ActorHit = NewActorHit;
	WorldLocation = NewWorldLocation;
	Activate(ActorHit, NewWorldLocation);

}

