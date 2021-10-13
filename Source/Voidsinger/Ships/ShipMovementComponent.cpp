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

void UShipMovementComponent::UpdateThrusters()
{
	Thrusters.Empty();
	ThrustersForDirection.Empty();
	ThrustersForRotation.Empty();

	UPartGridComponent* PartGrid = Cast<ABaseShip>(GetOwner())->PartGrid;

	for (auto& Val : PartGrid->GetPartGrid())
	{
		UBaseThrusterPart* Thruster = Cast<UBaseThrusterPart>(Val.Value.Part);
		if (Thruster)
		{
			Thrusters.Emplace(Thruster);
		}
	}
}

void UShipMovementComponent::RotateShip(bool Clockwise)
{
	if (ThrustersForRotation.Contains(Clockwise))
	{
		for (UBaseThrusterPart* Thruster : ThrustersForRotation.FindRef(Clockwise))
		{
			Thruster->Thrust();
		}
	}
	else
	{
		TSet<UBaseThrusterPart*> ThrustersToAdd = TSet<UBaseThrusterPart*>();

		for (UBaseThrusterPart* Thruster : Thrusters)
		{
			FVector2D ThrustDirection = FVector2D(1, 0).GetRotated(+Thruster->GetThrustRotation());
			FVector2D ThrusterLocation = FVector2D(Thruster->GetThrustRelativeLocation()).GetSafeNormal();
			if ((FVector2D::CrossProduct(ThrustDirection, ThrusterLocation) > 0) ^ Clockwise)
			{
				Thruster->Thrust();
				ThrustersToAdd.Emplace(Thruster);
			}
		}
	}
}

void UShipMovementComponent::Move(FVector2D Direction)
{
	Direction = Direction.GetSafeNormal();
	if (ThrustersForDirection.Contains(Direction))
	{
		for (UBaseThrusterPart* Thruster : ThrustersForDirection.FindRef(Direction))
		{
			Thruster->Thrust();
		}
	}
	else
	{
		TSet<UBaseThrusterPart*> ThrustersToAdd = TSet<UBaseThrusterPart*>();

		for (UBaseThrusterPart* Thruster : Thrusters)
		{
			if (FVector2D::DotProduct(FVector2D(1, 0).GetRotated(Thruster->GetThrustRotation()), Direction) >= .5)
			{
				Thruster->Thrust();
				ThrustersToAdd.Emplace(Thruster);
			}
		}

		ThrustersForDirection.Emplace(Direction, ThrustersToAdd);
	}
}

float UShipMovementComponent::GetLookDirectionTollerance()
{
	return LookDirectionTollerance;
}
