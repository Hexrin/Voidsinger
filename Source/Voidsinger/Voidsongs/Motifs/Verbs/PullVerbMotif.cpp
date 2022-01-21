// Fill out your copyright notice in the Description page of Project Settings.


#include "PullVerbMotif.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

void UPullVerbMotif::ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness)
{
	Cast<AVoidgrid>(Hit.GetActor())->AddImpulse(FVector2D((Hit.Location - Hit.TraceStart).Normalize() * Effectiveness * PullForce), FVector2D(Hit.Location));
}
