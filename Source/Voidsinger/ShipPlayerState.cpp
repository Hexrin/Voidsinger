// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPlayerState.h"

TArray<FPartData> AShipPlayerState::GetParts()
{
	return ShipBlueprint.GetValueArray();
}
