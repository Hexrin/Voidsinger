// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPhysicsComponent.h"
#include "BaseShip.h"
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
	DebugColor = FColor(FMath::RoundToInt(FMath::GetMappedRangeValueClamped(FVector2D(0, RAND_MAX), FVector2D(0, 255), FMath::Rand())), FMath::RoundToInt(FMath::GetMappedRangeValueClamped(FVector2D(0, RAND_MAX), FVector2D(0, 255), FMath::Rand())), FMath::RoundToInt(FMath::GetMappedRangeValueClamped(FVector2D(0, RAND_MAX), FVector2D(0, 255), FMath::Rand())));
}


// Called when the game starts
void UShipPhysicsComponent::BeginPlay()
{
	Super::BeginPlay();
}


//Called every tick
void UShipPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetLinearVelocity(LinearVelocity + LinearAcceleration * DeltaTime);
	SetAngularVelocity(AngularVelocity + AngularAcceleration * DeltaTime);

	DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation(), FVector(LinearVelocity, 0) + GetOwner()->GetActorLocation(), 5, DebugColor, false, -1.0F, 0U, 1.f);

	FHitResult Result = FHitResult();
	FTransform NewTransform = FTransform(FRotator(0, FMath::RadiansToDegrees(AngularVelocity) * DeltaTime + Ship->GetActorRotation().Yaw, 0), (FVector(LinearVelocity, 0) * DeltaTime) + Ship->GetActorTransform().GetTranslation(), FVector(1));
	//UE_LOG(LogTemp, Warning, TEXT("\n\tNewTransform: %s"), *NewTransform.ToString());

	if (!FMath::IsNearlyEqual(LinearVelocity.SizeSquared(), 0, MinLinearVelocity) || !FMath::IsNearlyEqual(AngularVelocity, 0, MinAngularVelocity))
	{
		GetOwner()->SetActorTransform(NewTransform);
		if (UShipPhysicsComponent::SweepShip(NewTransform, Result))
		{
			DrawDebugPoint(GetWorld(), Result.Location, 25, FColor::Orange, true);
			FVector2D RelativeHitLocation = FVector2D(Result.Location - GetOwner()->GetActorLocation());

			FVector2D ImpactNormal = FVector2D(Result.Normal);
			ImpactNormal.Normalize();

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

					OtherPhysicsComponent->AddImpulse(-1 * CollisionImpulseFactor * ImpactNormal, OtherRelativeHitLocation);
				}
				else
				{
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
void UShipPhysicsComponent::AddForce(FVector2D RelativeForceLocation, FVector2D Force)
{
	if (!Force.IsZero())
	{
		RelativeForceLocation = RelativeForceLocation.GetRotated(GetOwner()->GetActorRotation().Yaw);
		DrawDebugDirectionalArrow(GetWorld(), FVector(RelativeForceLocation, 0) + GetOwner()->GetActorLocation(), FVector(RelativeForceLocation + (Force / Mass), 0) + GetOwner()->GetActorLocation(), 5, DebugColor, true, -1.0F, 0U, 0.1f);
		UE_LOG(LogTemp, Warning, TEXT("Add Linear Acceleration to %s: %s"), *GetReadableName(), *(Force / Mass).ToString());
		LinearAcceleration += Force / Mass;
		AngularAcceleration += ((RelativeForceLocation.X * Force.Y - RelativeForceLocation.Y * Force.X) / GetMomentOfInertia());
	}
}

void UShipPhysicsComponent::AddImpulse(FVector2D Impulse, FVector2D RelativeImpulseLocation)
{
	SetLinearVelocity(LinearVelocity + Impulse / GetMass());
	SetAngularVelocity(AngularVelocity + FVector2D::CrossProduct(RelativeImpulseLocation, Impulse) / GetMomentOfInertia());
}

FVector2D UShipPhysicsComponent::GetVelocity()
{
	return LinearVelocity;
}
void UShipPhysicsComponent::SetLinearVelocity(FVector2D NewVelocity)
{
	LinearVelocity = FMath::Clamp(NewVelocity, FVector2D(-1 * MaxLinearVelocity), FVector2D(MaxLinearVelocity));
}

void UShipPhysicsComponent::SetAngularVelocity(float NewVelocity)
{
	AngularVelocity = FMath::Clamp(NewVelocity, -1 * MaxAngularVelocity, MaxAngularVelocity);
}
FVector2D UShipPhysicsComponent::GetVelocityOfPoint(FVector2D RelativePointLocation)
{
	float AngularVelocityRadians = FMath::DegreesToRadians(GetAngularVelocity());
	return GetVelocity() + FVector2D(-1 * AngularVelocityRadians * RelativePointLocation.Y, AngularVelocityRadians * RelativePointLocation.X);
}

void UShipPhysicsComponent::UpdateMassCalculations()
{
	//UE_LOG(LogTemp, Warning, TEXT("%s Has Updated Mass"), *GetReadableName());

	Mass = Ship->PartGrid->GetMass();
	FVector2D DeltaCoM = Ship->PartGrid->GetCenterOfMass() - CenterOfMass;
	CenterOfMass = Ship->PartGrid->GetCenterOfMass();
	MomentOfInertia = Ship->PartGrid->GetMomentOfInertia();

	Ship->SetMeshRelativeLocation(CenterOfMass);
	Ship->AddActorWorldOffset(-1 * FVector(DeltaCoM, 0));
}

bool UShipPhysicsComponent::SweepShip(const FTransform& NewTransform, FHitResult& Hit)
{
	//Return Values
	Hit = FHitResult();
	bool ReturnValue = true;
	TArray<FHitResult> Hits = TArray<FHitResult>();

	FTransform Start = Ship->GetActorTransform();
	FQuat TraceRot = Start.GetRotation();

	FVector BoundsLocation;
	FVector BoundsExtent;
	Ship->GetActorBounds(true, BoundsLocation, BoundsExtent);
	BoundsLocation -= Ship->GetActorLocation();

	FCollisionQueryParams QueryParams = FCollisionQueryParams().DefaultQueryParam;
	QueryParams.AddIgnoredActor(Ship);

	if (Ship->GetWorld()->SweepSingleByObjectType(Hit, Start.GetTranslation() + BoundsLocation, NewTransform.GetTranslation() + BoundsLocation, TraceRot, FCollisionObjectQueryParams::DefaultObjectQueryParam, FCollisionShape::MakeBox(BoundsExtent), QueryParams))
	{
		Hit = FHitResult();

		FQuat DeltaRot = NewTransform.GetRelativeTransform(Start).GetRotation();
		FVector DeltaTranslation = NewTransform.GetTranslation() - Start.GetTranslation();

		for (auto& Pixel : Ship->PartGrid->GetPartGrid())
		{
			FVector StartLoc = FVector(FVector2D(Pixel.Key).GetRotated(Ship->GetActorRotation().Yaw), 0) + Ship->GetActorLocation();
			FVector EndLoc = DeltaTranslation + (Start.GetRotation() * DeltaRot).RotateVector(FVector(FVector2D(Pixel.Key), 0)) + Ship->GetActorLocation();
			FHitResult ThisHit = FHitResult();

			if (Ship->GetWorld()->SweepSingleByObjectType(ThisHit, StartLoc, EndLoc, TraceRot, FCollisionObjectQueryParams::AllObjects, FCollisionShape::MakeBox(FVector(0.5f)), QueryParams))
			{
				ReturnValue = false;
				Hits.Emplace(ThisHit);
				//UE_LOG(LogTemp, Warning, TEXT("HIT"));
				///DrawDebugDirectionalArrow(Target->GetWorld(), StartLoc + FVector(0, 0, 1), EndLoc + FVector(0, 0, 1), .25f, FColor::Red, false, 5, 0U, 0.05);
				//DrawDebugBox(Target->GetWorld(), ThisHit.Location, FVector(.5), TraceRot, FColor::Red, false, 5);
			}
			/*else
			{
				DrawDebugDirectionalArrow(Target->GetWorld(), StartLoc + FVector(0, 0, 1), EndLoc + FVector(0, 0, 1), .25f, FColor::Green, false, 5, 0U, 0.05);
			}*/
		}

	}

	if (!ReturnValue)
	{
		for (FHitResult Value : Hits)
		{
			if (Value.Time < Hit.Time)
			{
				Hit = Value;
				//DrawDebugDirectionalArrow(Target->GetWorld(), Hit.TraceStart+FVector(0,0,1), Hit.Location + FVector(0, 0, 1), .5f, FColor::Blue, true, 5, 0U, 0.1);
				//UE_LOG(LogTemp, Warning, TEXT("Hit time: %f"), Hit.Time);
			}
		}
	}
	
	return ReturnValue;
}

float UShipPhysicsComponent::GetAngularVelocity()
{
	return AngularVelocity;
}

float UShipPhysicsComponent::GetMass()
{
	return Mass;
}

float UShipPhysicsComponent::GetMomentOfInertia()
{
	return MomentOfInertia;
}
FVector2D UShipPhysicsComponent::GetCenterOfMass()
{
	return CenterOfMass;
}
////////// CHANGE THIS BACK TO JUST return MomentOfInertia; (when done testing)\\\\\\\\\\\\\\