// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShip.h"


APlayerShip::APlayerShip()
{
    Pixels=55;
    
    if (!CameraHeight)
    {
        CameraHeight = 2000;
    }


    //Setup Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Camera->SetRelativeLocation(FVector(0, 0, CameraHeight));
    Camera->SetRelativeRotation(FRotator(90, 0, 0));

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

FTravelCost APlayerShip::GetTravelCost(class UStarSystemData* Target)
{
    TMap<TEnumAsByte<EFluidType>, float> FCosts;
    FCosts.Add(EFluidType::Fuel, 100);
    return FTravelCost(FCosts, 55);
}
