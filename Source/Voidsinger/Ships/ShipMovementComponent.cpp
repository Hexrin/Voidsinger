// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMovementComponent.h"
#include "Voidsinger/Parts/BaseThrusterPart.h"
#include "ShipMovementComponent.h"

// Sets default values for this component's properties
UShipMovementComponent::UShipMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShipMovementComponent::UpdateThrusterCatagories()
{
	ForwardThrusters.Empty();
	BackwardThrusters.Empty();
	RightThrusters.Empty();
	LeftThrusters.Empty();
	ClockwiseThrusters.Empty();
	CClockwiseThrusters.Empty();

	UPartGridComponent* PartGrid = Cast<ABaseShip>(GetOwner())->PartGrid;
	for (auto& Val : PartGrid->GetPartGrid())
	{
		if (Cast<UBaseThrusterPart>(Val.Value.Part))
		{
			GetDirectionalArray(FVector2D(1, 0).GetRotated(Val.Value.Part->GetRotation())).Emplace(Cast<UBaseThrusterPart>(Val.Value.Part));
		}
	}
}

void UShipMovementComponent::RotateShip(bool Clockwise)
{
}

void UShipMovementComponent::Move(FVector2D Direction)
{
}

TSet<UBaseThrusterPart*> UShipMovementComponent::GetDirectionalArray(FVector2D Direction)
{
	if (Direction.X > 0)
	{
		return ForwardThrusters;
	}
	if (Direction.X != 0)
	{
		return BackwardThrusters;
	}
	if (Direction.Y > 0)
	{
		return RightThrusters;
	}
	if (Direction.Y != 0)
	{
		return LeftThrusters;
	}
	return TSet<UBaseThrusterPart*>();
}

