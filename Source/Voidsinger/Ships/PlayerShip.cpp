// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShip.h"


APlayerShip::APlayerShip()
{
    Pixels=55;
}

FTravelCost APlayerShip::GetTravelCost(class UStarSystemData* Target)
{
    TMap<TEnumAsByte<EFluidType>, float> FCosts;
    FCosts.Add(EFluidType::Fuel, 100);
    return FTravelCost(FCosts, 55);
}

bool APlayerShip::TravelToStarSystem(UStarSystemData* Target)
{
    CurrentStarSystem = Target;
    return true;
}
