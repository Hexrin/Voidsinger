// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseThrusterPart.h"
#include "Voidsinger/Ships/BaseShip.h"

//Copy comment from .h -Mabel
UBaseThrusterPart::UBaseThrusterPart()
{
	//Could these be done in the .h file?
	//Why is thrust force initialized to 10? That seems super random. No magic numbers
	// Why is the construction script overriding "EditDefaultsOnly" variables? -Mabel
	ThrustForce = 10;
	ThrustLocation = FVector2D();
	ThrustRotation = 0;
}

//Copy comment from .h -Mabel
const float UBaseThrusterPart::GetThrustForce()
{
	return ThrustForce;
}

//Copy comment from .h -Mabel
const float UBaseThrusterPart::GetThrustRotation()
{
	return ThrustRotation + GetRelativeRotation();
}

//Copy comment from .h -Mabel
const FVector2D UBaseThrusterPart::GetThrustRelativeLocation()
{
	return ThrustLocation.GetRotated(GetRelativeRotation()) + FVector2D(GetPartRelativeLocation());
}

//Copy comment from .h -Mabel
const bool UBaseThrusterPart::IsThrusting()
{
	return Throttle != 0;
}

//Copy comment from .h -Mabel
const float UBaseThrusterPart::GetThrottle()
{
	return Throttle;
}

//Copy comment from .h -Mabel
void UBaseThrusterPart::Thrust(float NewThrottle)
{
	//Shouldn't the max of this clamp be like "MaxThrottle" or something? or is throttle a percentage? -Mabel
	Throttle = FMath::Clamp(NewThrottle, 0.f, 1.f);
}

//Copy comment from .h -Mabel
void UBaseThrusterPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//I mean you did make a function for the condition of this if statement... maybe it isn't optimal to call the function
	//though, idk -Mabel
	if (Throttle != 0)
	{
		//Why is delta time being clamped? Why 0.0666666667 specifically? no magic numbers
		//Isn't there a rotate function for 2d vectors?
		//If you don't delete the "GetShip()" function then use that instead of "GetPartGrid()->GetOwner()" -Mabel
		Cast<ABaseShip>(GetPartGrid()->GetOwner())->PhysicsComponent->AddImpulse(FVector2D(ThrustForce * Throttle * FMath::Clamp(DeltaTime, 0.f, 0.0666666667f), 0).GetRotated(GetThrustRotation() + GetShip()->GetActorRotation().Yaw), FVector2D(GetShip()->GetActorQuat().RotateVector(FVector(GetThrustRelativeLocation(), 0))));
		
		//Why is throttle reset to 0?
		Throttle = 0;
	}
}

