// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPhysicsComponent.h"
#include "BaseShip.h"
#include "PlayerShip.h"
#include "Voidsinger/Parts/BasePart.h"

// Sets default values for this component's properties
UShipPhysicsComponent::UShipPhysicsComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	Ship = (ABaseShip*)GetOwner();
	AngularVelocity = 0;
	LinearVelocity = FVector2D(0, 0);
	LinearAcceleration = FVector2D(0, 0);
	AngularAcceleration = 0;
	//DebugColor = FColor(FMath::RoundToInt(FMath::GetMappedRangeValueClamped(FVector2D(0, RAND_MAX), FVector2D(0, 255), FMath::Rand())), FMath::RoundToInt(FMath::GetMappedRangeValueClamped(FVector2D(0, RAND_MAX), FVector2D(0, 255), FMath::Rand())), FMath::RoundToInt(FMath::GetMappedRangeValueClamped(FVector2D(0, RAND_MAX), FVector2D(0, 255), FMath::Rand())));
}


// Called when the game starts
void UShipPhysicsComponent::BeginPlay()
{
	Super::BeginPlay();
}


//Called every tick
//This comment should say what you are doing inside tick, not that it is in fact called every tick -Mabel Suggestion
void UShipPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Consider moving this to a new function that has a more descriptive name than "Tick" -Mabel Suggestion

	SetLinearVelocity(LinearVelocity + LinearAcceleration * DeltaTime);
	SetAngularVelocity(AngularVelocity + AngularAcceleration * DeltaTime);

	//Debug -Mabel Suggestion
	//DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation(), FVector(LinearVelocity, 0) + GetOwner()->GetActorLocation(), 5, DebugColor, false, -1.0F, 0U, 1.f);

	//Scope issues, create these variables inside your if statement because they aren't used outside of it -Mabel Suggestion
	FHitResult Result = FHitResult();
	FTransform NewTransform = FTransform(FRotator(0, FMath::RadiansToDegrees(AngularVelocity) * DeltaTime + Ship->GetActorRotation().Yaw, 0), (FVector(LinearVelocity, 0) * DeltaTime) + Ship->GetActorTransform().GetTranslation(), FVector(1));

	//Another debug -Mabel Suggestion
	//UE_LOG(LogTemp, Warning, TEXT("\n\tNewTransform: %s"), *NewTransform.ToString());

	if (!FMath::IsNearlyEqual(LinearVelocity.SizeSquared(), 0, MinLinearVelocity) || !FMath::IsNearlyEqual(AngularVelocity, 0, MinAngularVelocity))
	{
		GetOwner()->SetActorTransform(NewTransform);

		//New function: Collide() -Mabel Suggestion
		if (UShipPhysicsComponent::SweepShip(NewTransform, Result))
		{
			//dEBUG -Mabel Suggestion
			//DrawDebugPoint(GetWorld(), Result.Location, 25, FColor::Orange, true);
			FVector2D RelativeHitLocation = FVector2D(Result.Location - GetOwner()->GetActorLocation());

			FVector2D ImpactNormal = FVector2D(Result.Normal);
			ImpactNormal.Normalize();

			//Comment scary math -Mabel
			if (FVector2D::DotProduct(GetVelocityOfPoint(RelativeHitLocation), ImpactNormal) < 0)
			{
				ABaseShip* OtherShip = Cast<ABaseShip>(Result.GetActor());

				float CollisionImpulseFactor;

				if (IsValid(OtherShip))
				{
					UShipPhysicsComponent* OtherPhysicsComponent = OtherShip->PhysicsComponent;
					FVector2D OtherRelativeHitLocation = FVector2D(Result.Location - OtherShip->GetActorLocation());

					CollisionImpulseFactor = FVector2D::DotProduct(-1 * (1 + CollisionElasticity) * GetVelocityOfPoint(RelativeHitLocation), ImpactNormal) /
						(1 / GetMass() + 1 / OtherPhysicsComponent->GetMass() + FMath::Square(FVector2D::CrossProduct(RelativeHitLocation, ImpactNormal)) / GetMomentOfInertia() + FMath::Square(FVector2D::CrossProduct(OtherRelativeHitLocation, ImpactNormal)) / OtherPhysicsComponent->GetMomentOfInertia());

					//You shouldn't add an impulse to the other physics component *and* this physics component, because the other physics component will be doing the same thing.
					//Only handle *this* physics component's impulse, not the other one. (or only handle the other one and not this one.) -Mabel Suggestion
					OtherPhysicsComponent->AddImpulse(-1 * CollisionImpulseFactor * ImpactNormal, OtherRelativeHitLocation);
				}
				else
				{
					//You shouldn't be colliding with anything that isn't a ship, right? this might not be needed -Mabel Suggestion
					CollisionImpulseFactor = FVector2D::DotProduct(-1 * (1 + CollisionElasticity) * GetVelocityOfPoint(RelativeHitLocation), ImpactNormal) /
						(1 / GetMass() + FMath::Square(FVector2D::CrossProduct(RelativeHitLocation, ImpactNormal)) / GetMomentOfInertia());
				}

				AddImpulse(CollisionImpulseFactor * ImpactNormal, RelativeHitLocation);				
			}
		}
	}
	

	LinearAcceleration = FVector2D(0, 0);
	AngularAcceleration = 0;

}

//When a force is added to the ship
//Well yes, but what does adding a force to the ship do? Comment better, also copy comment to .h -Mabel Suggestion
void UShipPhysicsComponent::AddForce(FVector2D RelativeForceLocation, FVector2D Force)
{
	if (!Force.IsZero())
	{
		RelativeForceLocation = RelativeForceLocation.GetRotated(GetOwner()->GetActorRotation().Yaw);

		//De bug -Mabel Suggestion
		//DrawDebugDirectionalArrow(GetWorld(), FVector(RelativeForceLocation, 0) + GetOwner()->GetActorLocation(), FVector(RelativeForceLocation + (Force / Mass), 0) + GetOwner()->GetActorLocation(), 5, DebugColor, true, -1.0F, 0U, 0.1f);
		UE_LOG(LogTemp, Warning, TEXT("Add Linear Acceleration to %s: %s"), *GetReadableName(), *(Force / Mass).ToString());

		LinearAcceleration += Force / Mass;
		AngularAcceleration += ((RelativeForceLocation.X * Force.Y - RelativeForceLocation.Y * Force.X) / GetMomentOfInertia());
	}
}

//Comment -Mabel Suggestion
void UShipPhysicsComponent::AddImpulse(FVector2D Impulse, FVector2D RelativeImpulseLocation)
{
	SetLinearVelocity(LinearVelocity + Impulse / GetMass());
	SetAngularVelocity(AngularVelocity + FVector2D::CrossProduct(RelativeImpulseLocation, Impulse) / GetMomentOfInertia());
}

//Comment -Mabel Suggestion
FVector2D UShipPhysicsComponent::GetVelocity()
{
	return LinearVelocity;
}

//Comment -Mabel Suggestion
void UShipPhysicsComponent::SetLinearVelocity(FVector2D NewVelocity)
{
	LinearVelocity = FMath::Clamp(NewVelocity, FVector2D(-1 * MaxLinearVelocity), FVector2D(MaxLinearVelocity));
}

//Comment -Mabel Suggestion
void UShipPhysicsComponent::SetAngularVelocity(float NewVelocity)
{
	AngularVelocity = FMath::Clamp(NewVelocity, -1 * MaxAngularVelocity, MaxAngularVelocity);
}

//Comment -Mabel Suggestion
FVector2D UShipPhysicsComponent::GetVelocityOfPoint(FVector2D RelativePointLocation)
{
	float AngularVelocityRadians = FMath::DegreesToRadians(GetAngularVelocity());
	return GetVelocity() + FVector2D(-1 * AngularVelocityRadians * RelativePointLocation.Y, AngularVelocityRadians * RelativePointLocation.X);
}

//Comment -Mabel Suggestion
void UShipPhysicsComponent::UpdateMassCalculations()
{

	//I saw that this was called when a part is added. Now I don't know how all this works, but maybe instead of recalculating
	// all the mass when a part is added you could just
	// Mass += NewPartMass
	// And then you can calculate the new center of mass without recalculating all mass. -Mabel Suggestion
	
	//Doobee doobee debug -Mabel Suggestion
	//UE_LOG(LogTemp, Warning, TEXT("%s Has Updated Mass"), *GetReadableName());

	Mass = Ship->PartGrid->CalcMass();
	FVector2D NewCoM = Ship->PartGrid->CalcCenterOfMass();
	
	MomentOfInertia = Ship->PartGrid->CalcMomentOfInertia();
	Ship->SetMeshRelativeLocation(-1 * NewCoM);

	CenterOfMass = NewCoM;
}

//Comment -Mabel Suggestion
//If you pass anything by ref and you make changes to that thing, you should name it "OutParam". So, "OutHit" instead of "Hit". -Mabel Suggestion
bool UShipPhysicsComponent::SweepShip(const FTransform& NewTransform, FHitResult& Hit)
{
	//Return Values
	Hit = FHitResult();
	bool ReturnValue = false;
	//Is it a return value if it isn't returned? -Mabel Suggestion
	TArray<FHitResult> Hits = TArray<FHitResult>();

	//"Start" is a confusing variable name to me. Start is a verb. StartLocation maybe? -Mabel Suggestion
	FTransform Start = Ship->GetActorTransform();
	FQuat TraceRot = Start.GetRotation();

	FArrayBounds ShipBounds = Ship->PartGrid->GetPartGridBounds();

	//Comment math -Mabel Suggestion
	FVector BoundsExtent = FVector(FVector2D(ShipBounds.UpperBounds - ShipBounds.LowerBounds) / 2, 0.5);

	//Don't you need to put the ship location into this? -Mabel Suggestion
	FVector BoundsLocation = BoundsExtent - FVector(ShipBounds.UpperBounds, 0.5);

	FCollisionQueryParams QueryParams = FCollisionQueryParams().DefaultQueryParam;
	QueryParams.AddIgnoredActor(Ship);
	QueryParams.bTraceComplex = false;

	//More debug -Mabel Suggestion
	//DrawDebugBox(Ship->GetWorld(), NewTransform.GetTranslation() + BoundsLocation, BoundsExtent, TraceRot, FColor::Emerald, false, 5);

	//Comment this if statment -Mabel Suggestion
	//Is this checking if there is an actor in your box that you previously defined? There is a "BoxOverlapActors" node in blueprints that I think you should find the c++ equivilent for. 
	//Or is that what this is and I'm being dumb? -Mabel Suggestion
	if (Ship->GetWorld()->SweepSingleByObjectType(Hit, Start.GetTranslation() + BoundsLocation, NewTransform.GetTranslation() + BoundsLocation, TraceRot, FCollisionObjectQueryParams::DefaultObjectQueryParam, FCollisionShape::MakeBox(BoundsExtent), QueryParams))
	{

		//You've already set Hit to FHitResult() earlier. -Mabel Suggestion
		Hit = FHitResult();

		//Comment math -Mabel Suggestion
		FQuat DeltaRot = NewTransform.GetRelativeTransform(Start).GetRotation();
		FVector DeltaTranslation = NewTransform.GetTranslation() - Start.GetTranslation();

		TGridMap<FPartData> Grid = Ship->PartGrid->GetPartGrid();

		//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion"  -Mabel Suggestion
		//iteration through part grid makes me sad. -Mabel Suggestion
		for (int i = 0; i < Grid.Num(); i++)
		{
			//Comment! -Mabel Suggestion
			FVector StartLoc = FVector(FVector2D(Grid.LocationAtIndex(i)).GetRotated(Ship->GetActorRotation().Yaw), 0) + Ship->GetActorLocation();
			FVector EndLoc = DeltaTranslation + (Start.GetRotation() * DeltaRot).RotateVector(FVector(FVector2D(Grid.LocationAtIndex(i)), 0)) + Ship->GetActorLocation();
			FHitResult ThisHit = FHitResult();

			if (Ship->GetWorld()->SweepSingleByObjectType(ThisHit, StartLoc, EndLoc, TraceRot, FCollisionObjectQueryParams::AllObjects, FCollisionShape::MakeBox(FVector(0.5f)), QueryParams))
			{
				ReturnValue = true;

				//Do your "time" check here instead of down below. That way you don't have to iterate through Hits. -Mabel Suggestion
				Hits.Emplace(ThisHit);

				//Debug -Mabel Suggestion
				//UE_LOG(LogTemp, Warning, TEXT("HIT"));
				///DrawDebugDirectionalArrow(Target->GetWorld(), StartLoc + FVector(0, 0, 1), EndLoc + FVector(0, 0, 1), .25f, FColor::Red, false, 5, 0U, 0.05);
				//DrawDebugBox(Ship->GetWorld(), ThisHit.Location, FVector(.5), TraceRot, FColor::Red, false, 5);
			}
			else
			{
				//Debug -Mabel Suggestion
				//DrawDebugDirectionalArrow(Ship->GetWorld(), StartLoc + FVector(0, 0, 1), EndLoc + FVector(0, 0, 1), .25f, FColor::Green, false, 5, 0U, 0.05);
			}
		}
	}

	if (ReturnValue)
	{
		for (FHitResult Value : Hits)
		{
			if (Value.Time < Hit.Time)
			{
				Hit = Value;

				//Debug -Mabel Suggestion
				//DrawDebugDirectionalArrow(Target->GetWorld(), Hit.TraceStart+FVector(0,0,1), Hit.Location + FVector(0, 0, 1), .5f, FColor::Blue, true, 5, 0U, 0.1);
				//UE_LOG(LogTemp, Warning, TEXT("Hit time: %f"), Hit.Time);
			}
		}
	}
	
	return ReturnValue;
}

//Comment -Mabel Suggestion
float UShipPhysicsComponent::GetAngularVelocity()
{
	return AngularVelocity;
}

//Comment -Mabel Suggestion
float UShipPhysicsComponent::GetMass()
{
	return Mass;
}

//Comment -Mabel Suggestion
float UShipPhysicsComponent::GetMomentOfInertia()
{
	return MomentOfInertia;
}

//Comment -Mabel Suggestion
FVector2D UShipPhysicsComponent::GetCenterOfMass()
{
	return CenterOfMass;
}
////////// CHANGE THIS BACK TO JUST return MomentOfInertia; (when done testing)\\\\\\\\\\\\\\
//Uhhh it does return MomentOfInertia? -Mabel Suggestion