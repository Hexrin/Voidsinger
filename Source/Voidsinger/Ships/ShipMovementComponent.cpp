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
	//Why is this overriden if there is nothing here? -Mabel Suggestion
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	//Why is this overriden if there is nothing here? -Mabel Suggestion
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

	//Edit: upon revisit, I see why you want to recalculate all the ThrustersForDirection/Rotation. I still think you shouldn't
	//iterate through the part grid and find all the thrusters again, because I think that breaks encapsulation. The movement component
	//shouldn't care what the part grid looks like. 
	//As for recalculating ThrustersForRotation, I don't think you need to. When you add the new thruster, you can
	//immediately calculate what rotation the new thruster would do. 

	//However, there is a problem. Say you start off with a really small ship, and you put thrusters to left and right of the center of mass on one side. Then, you take some armor,
	//and you build your ship to the right a lot, in fact, so much so that it moves the center of mass pass the thruster on the right. This system would break, because it wouldn't know
	//that the center of mass had moved, because it was never told to recalculate rotation or direction. UpdateThrusters() is only called when a thruster is added. -Mabel Suggestion
	Thrusters.Empty();

	ThrustersForDirection.Empty();
	ThrustersForRotation.Empty();
		
	//This shouldn't be needed. You don't need to find all the Thrusters again when a new thruster is added, you can just Thrusters.Emplace(NewThruster). You just need to keep track of 
	//when the thruster is destroyed. -Mabel Suggestion
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
		Thruster->SetThrottle(Throttle);
	}
}

//Comment -Mabel Suggestion
void UShipMovementComponent::Move(FVector2D Direction, float Throttle)
{
	Direction.Normalize();
	Throttle = FMath::Clamp(Throttle, 0.f, 1.f);
	for (UBaseThrusterPart* Thruster : GetThrustersForDirection(Direction.GetSafeNormal()))
	{
		Thruster->SetThrottle(Throttle * FVector2D::DotProduct(Direction, FVector2D(1,0).GetRotated(Thruster->GetThrustRotation())));
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
		TSet<UBaseThrusterPart*> ThrustersToAdd = TSet<UBaseThrusterPart*>();

		for (UBaseThrusterPart* Thruster : Thrusters)
		{
			FVector2D ThrustDirection = FVector2D(1, 0).GetRotated(+Thruster->GetThrustRotation());
			//Why do you normalize a location? -Mabel Suggestion
			FVector2D ThrusterLocation = FVector2D(Thruster->GetThrustRelativeLocation()).GetSafeNormal();
			float CrossProduct = FVector2D::CrossProduct(ThrustDirection, ThrusterLocation);

			//This if statement needs comment -Mabel Suggestion
			if (!FMath::IsNearlyZero(CrossProduct, 0.001f) && ((CrossProduct > 0) ^ Clockwise))
			{
				//deeeeeebuuuuuuuug -Mabel Suggestion
				UE_LOG(LogTemp, Warning, TEXT("Is Rotatable: %s   |   Because: %f"), *Thruster->GetThrustRelativeLocation().ToString(), CrossProduct);
				ThrustersToAdd.Emplace(Thruster);
			}
		}

		//What if thrusters to add is empty? -Mabel Suggestion
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
