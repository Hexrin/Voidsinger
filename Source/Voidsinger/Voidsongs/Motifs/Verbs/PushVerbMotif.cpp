// Fill out your copyright notice in the Description page of Project Settings.


#include "PushVerbMotif.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

void UPushVerbMotif::ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness)
{
	Cast<AVoidgrid>(Hit.GetActor())->AddImpulse(FVector2D((Hit.TraceStart - Hit.Location).Normalize() * Effectiveness * PushForce), FVector2D(Hit.Location));
}
