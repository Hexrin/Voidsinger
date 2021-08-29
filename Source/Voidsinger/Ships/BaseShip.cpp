// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShip.h"

// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	MovementComponent = CreateDefaultSubobject<UShipMovementComponent>(TEXT("Movement Component"));
	PartGrid = CreateDefaultSubobject<UPartGridComponent>(TEXT("Part Grid"));
	
}

// Called when the game starts or when spawned
void ABaseShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float const ABaseShip::GetFluidCapacity(TEnumAsByte<EFluidType> Fluid)
{
	return 100.0f;
}

float const ABaseShip::GetFluidAmount(TEnumAsByte<EFluidType> Fluid)
{
	return 100.0f;
}

FVector2D ABaseShip::GetCenterOfMass()
{
	return FVector2D();
}

float ABaseShip::GetMass()
{
	return 1.0f;
}

