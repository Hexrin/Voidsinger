// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Parts/BaseResourcePart.h"
#include "VoidsingerTypes.h"

UBaseResourcePart::UBaseResourcePart()
{

}

void UBaseResourcePart::Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGridComponent)
{
	Super::Init(Loc, Rot, PartGridComponent);

}
