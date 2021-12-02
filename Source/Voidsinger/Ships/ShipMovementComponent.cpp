// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMovementComponent.h"
#include "Voidsinger/Parts/BaseThrusterPart.h"
#include "ShipMovementComponent.h"
#include "Voidsinger/Ships/BaseShip.h"

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

//Comment -Mabel Suggestion
void UShipMovementComponent::UpdateThrusters()
{
	//I saw that this was called when a thruster was added to a ship. Instead of recalculating all the thrusters when a thruster
	//is added, consider simply... adding the thruster. 
	//Thrusters.Emplace(NewThruster) instead of "OH NO THRUSTER WAS ADDED BETTER RECALCULATE LITERALLY ALL OF THEM AND ITERATE
	// THROUGH ALL OF THE PARTS ON THE PART GRID"
	//-Mabel Suggestion
	Thrusters.Empty();

	//These are emptied then never set again? -Mabel Suggestion
	ThrustersForDirection.Empty();
	ThrustersForRotation.Empty();
		

	for (auto& Part : Cast<ABaseShip>(GetOwner())->PartGrid->GetPartGrid().GetValueArray())
	{
		UBaseThrusterPart* Thruster = Cast<UBaseThrusterPart>(Part.Part);
		if (Thruster)
		{
			Thrusters.Emplace(Thruster);
		}
	}
}

//Comment -Mabel Suggestion
void UShipMovementComponent::RotateShip(bool Clockwise, float Throttle)
{
	Throttle = FMath::Clamp(Throttle, 0.f, 1.f);
	for (UBaseThrusterPart* Thruster : GetThrustersForRotation(Clockwise))
	{
		Thruster->Thrust(Throttle);
	}
}

//Comment -Mabel Suggestion
void UShipMovementComponent::Move(FVector2D Direction, float Throttle)
{
	Direction.Normalize();
	Throttle = FMath::Clamp(Throttle, 0.f, 1.f);
	for (UBaseThrusterPart* Thruster : GetThrustersForDirection(Direction.GetSafeNormal()))
	{
		Thruster->Thrust(Throttle * FVector2D::DotProduct(Direction, FVector2D(1,0).GetRotated(Thruster->GetThrustRotation())));
	}
}

//Comment -Mabel Suggestion
const float UShipMovementComponent::GetLookDirectionErrorTollerance()
{
	return LookDirectionErrorTollerance;
}

//Comment -Mabel Suggestion
const float UShipMovementComponent::GetMoveSpeedErrorTollerance()
{
	return MoveSpeedErrorTollerance;
}

//Comment -Mabel Suggestion
const float UShipMovementComponent::GetRotationDirectionUpdateThreshold()
{
	return RotationDirectionUpdateThreshold;
}

//Comment -Mabel Suggestion
const float UShipMovementComponent::GetRotationDecelerationTargetDuration()
{
	return RotationDecelerationTargetDuration;
}

//Comment -Mabel Suggestion
TSet<UBaseThrusterPart*> UShipMovementComponent::GetThrustersForDirection(FVector2D Direction)
{
	Direction = Direction.GetSafeNormal();
	if (ThrustersForDirection.Contains(Direction))
	{
		return ThrustersForDirection.FindRef(Direction);
	}
	else
	{
		//Rename this to OutThrusters? -Mabel Suggestion
		TSet<UBaseThrusterPart*> ThrustersToAdd = TSet<UBaseThrusterPart*>();

		for (UBaseThrusterPart* Thruster : Thrusters)
		{
			if (FVector2D::DotProduct(FVector2D(1, 0).GetRotated(Thruster->GetThrustRotation()), Direction) >= .5)
			{
				ThrustersToAdd.Emplace(Thruster);
			}
		}

		ThrustersForDirection.Emplace(Direction, ThrustersToAdd);
		return ThrustersToAdd;
	}
	
}

//Comment -Mabel Suggestion
TSet<UBaseThrusterPart*> UShipMovementComponent::GetThrustersForRotation(bool Clockwise)
{

	if (ThrustersForRotation.Contains(Clockwise))
	{
		return ThrustersForRotation.FindRef(Clockwise);
	}
	else
	{
		//Rename to out thrusters? -Mabel Suggestion
		TSet<UBaseThrusterPart*> ThrustersToAdd = TSet<UBaseThrusterPart*>();

		for (UBaseThrusterPart* Thruster : Thrusters)
		{
			FVector2D ThrustDirection = FVector2D(1, 0).GetRotated(+Thruster->GetThrustRotation());
			FVector2D ThrusterLocation = FVector2D(Thruster->GetThrustRelativeLocation()).GetSafeNormal();
			float CrossProduct = FVector2D::CrossProduct(ThrustDirection, ThrusterLocation);
			if (!FMath::IsNearlyZero(CrossProduct, 0.001f) && ((CrossProduct > 0) ^ Clockwise))
			{
				UE_LOG(LogTemp, Warning, TEXT("Is Rotatable: %s   |   Because: %f"), *Thruster->GetThrustRelativeLocation().ToString(), CrossProduct);
				ThrustersToAdd.Emplace(Thruster);
			}
		}

		ThrustersForRotation.Emplace(Clockwise, ThrustersToAdd);
		return ThrustersToAdd;
	}
}

//Comment -Mabel Suggestion
const float UShipMovementComponent::GetMaximumAccelerationInDirection(FVector2D Direction, float AtThrottle)
{
	float Sum = 0;
	for (UBaseThrusterPart* Thruster : GetThrustersForDirection(Direction))
	{
		Sum += Thruster->GetThrustForce() * AtThrottle;
	}
	return Sum;
}

//Comment -Mabel Suggestion
const float UShipMovementComponent::GetMaximumAccelerationInRotation(bool Clockwise, float AtThrottle)
{
	float Sum = 0;
	for (UBaseThrusterPart* Thruster : GetThrustersForRotation(Clockwise))
	{
		Sum += FVector2D::CrossProduct(Thruster->GetThrustRelativeLocation(), FVector2D(Thruster->GetThrustForce() * AtThrottle, 0).GetRotated(Thruster->GetThrustRotation())) / Cast<ABaseShip>(GetOwner())->PhysicsComponent->GetMomentOfInertia();
	}
	return Sum;
}
