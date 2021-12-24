//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "BaseThrusterPart.h"
//#include "Voidsinger/Parts/BaseResourceSystem.h"
//#include "Voidsinger/Ships/BaseShip.h"
//
////Copy comment from .h -Mabel Suggestion
//UBaseThrusterPart::UBaseThrusterPart()
//{
//	//Could these be done in the .h file?
//	//Why is thrust force initialized to 10? That seems super random. No magic numbers
//	// Why is the construction script overriding "EditDefaultsOnly" variables? -Mabel Suggestion
//	ThrustForce = 10;
//	ThrustLocation = FVector2D();
//	ThrustRotation = 0;
//}
//
////Copy comment from .h -Mabel Suggestion
//const float UBaseThrusterPart::GetThrustForce()
//{
//	return ThrustForce;
//}
//
////Copy comment from .h -Mabel Suggestion
////What if there were a thruster that thrusted in multiple directions? This only returns 1 direction. Are we planning on having any thrusters that thrust in multiple directions? -Mabel Suggestion
//const float UBaseThrusterPart::GetThrustRotation()
//{
//	return ThrustRotation + GetRelativeRotation();
//}
//
////Copy comment from .h -Mabel Suggestion
//const FVector2D UBaseThrusterPart::GetThrustRelativeLocation()
//{
//
//	//Doesn't this return the location in world space, not in relative? Shouldn't you be subtracting the part's world location to go to relative location? 
//	// Edit: 
//	//I guess I am assuming ThrustLocation is already in WorldSpace... But if it's in world space, why do you have to rotate it? But if it's already in relative space,
//	//why do you have to change it to relative space? I am confuzzled. 
//	//What is this function supposed to be relative to? The PartGrid? The Part? The Center of Mass? The actor location?
//	//What is ThrustLocation already relative to? World space? The Part? The Center of Mass? The Actor Location? The PartGrid? -Mabel Suggestion
//	return ThrustLocation.GetRotated(GetRelativeRotation()) + FVector2D(GetPartRelativeLocation());
//}
//
////Copy comment from .h -Mabel Suggestion
//const bool UBaseThrusterPart::IsThrusting()
//{
//	return Throttle != 0;
//}
//
////Copy comment from .h -Mabel Suggestion
//const float UBaseThrusterPart::GetThrottle()
//{
//	return Throttle;
//}
//
////Copy comment from .h -Mabel Suggestion
//void UBaseThrusterPart::SetThrottle(float NewThrottle)
//{
//	//Shouldn't the max of this clamp be like "MaxThrottle" or something? or is throttle a percentage? -Mabel Suggestion
//	Throttle = FMath::Clamp(NewThrottle, Throttle, 1.f);
//}
//
////Copy comment from .h -Mabel Suggestion
//void UBaseThrusterPart::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	//I mean you did make a function for the condition of this if statement... maybe it isn't optimal to call the function
//	//though, idk -Mabel Suggestion
//	if (IsFunctional() && Throttle != 0 && GetSystemByType(EResourceType::Fuel)->DrawResources(FuelConsumptionRate * DeltaTime * Throttle))
//	{
//		//Why is delta time being clamped? Why 0.0666666667 specifically? no magic numbers
//		//Isn't there a rotate function for 2d vectors?
//		//If you don't delete the "GetShip()" function then use that instead of "GetPartGrid()->GetOwner()" -Mabel Suggestion
////		Cast<ABaseShip>(GetPartGrid()->GetOwner())->PhysicsComponent->AddImpulse(FVector2D(ThrustForce * Throttle * FMath::Clamp(DeltaTime, 0.f, 0.0666666667f), 0).GetRotated(GetThrustRotation() + GetShip()->GetActorRotation().Yaw), FVector2D(GetShip()->GetActorQuat().RotateVector(FVector(GetThrustRelativeLocation(), 0))));
//		
//		//Why is throttle reset to 0 every tick?
//		Throttle = 0;
//		Cast<ABaseShip>(GetPartGrid()->GetOwner())->PhysicsComponent->AddImpulse(FVector2D(ThrustForce * Throttle * FMath::Clamp(DeltaTime, 0.f, 0.0666666667f), 0).GetRotated(GetThrustRotation() + GetShip()->GetActorRotation().Yaw), FVector2D(GetShip()->GetActorQuat().RotateVector(FVector(GetThrustRelativeLocation(), 0))));
//
//	}
//
//	//Why is throttle reset to 0 every tick?
//	Throttle = 0;
//}
//
