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

	LinearVelocity + FMath::Clamp( LinearVelocity + LinearAcceleration * DeltaTime, FVector2D(-1* MaxLinearVelocity), FVector2D(MaxLinearVelocity));
	AngularVelocity + FMath::Clamp(AngularVelocity + AngularAcceleration * DeltaTime, -1 * MaxAngularVelocity, MaxAngularVelocity);

	DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation(), FVector(LinearVelocity, 0) + GetOwner()->GetActorLocation(), 5, DebugColor, false, -1.0F, 0U, 1.f);

	FHitResult Result = FHitResult();
	FTransform NewTransform = (FTransform(FRotator(0, AngularVelocity * DeltaTime + Ship->GetActorTransform().Rotator().Yaw, 0), (FVector(LinearVelocity, 0) * DeltaTime) + Ship->GetActorTransform().GetTranslation(), FVector(1)));
	//UE_LOG(LogTemp, Warning, TEXT("\n\tNewTransform: %s"), *NewTransform.ToString());

	if (!FMath::IsNearlyEqual(LinearVelocity.SizeSquared(), 0, 0.01f), !FMath::IsNearlyEqual(AngularVelocity, 0, 0.01f))
	{
		if (!UFunctionLibrary::SetActorTransformSweepComponets(Ship, Result, PrimComps, NewTransform))
		{
			ABaseShip* OtherShip = Cast<ABaseShip>(Result.GetActor());
			if (IsValid(OtherShip))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s Has collided"), *GetReadableName());

				UShipPhysicsComponent* OtherPhysicsComponent = OtherShip->PhysicsComponent;
				FVector2D RelativeForce = OtherPhysicsComponent->GetVelocityOfPoint(FVector2D(Result.Location - OtherShip->GetActorLocation()) - GetVelocityOfPoint(FVector2D(Result.Location - GetOwner()->GetActorLocation());

				if (FVector2D::DotProduct(FVector2D(Result.Normal), RelativeForce) > 0)
				{
					AddForce(FVector2D(Result.Location - GetOwner()->GetActorLocation()), (0.5 * RelativeForce * OtherPhysicsComponent->GetMass()) / (FVector::Dist2D(Result.Location, Result.TraceEnd)));
					OtherPhysicsComponent->AddForce(FVector2D(Result.Location - OtherShip->GetActorLocation()), (-0.5 * RelativeForce * GetMass()) / (FVector::Dist2D(Result.Location, Result.TraceEnd)));
				}			
			}
			else
			{
				AddForce(FVector2D(Result.Location - GetOwner()->GetActorLocation()), FVector2D(FMath::GetReflectionVector(FVector(GetVelocity(), 0), Result.Normal)));
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
		DrawDebugDirectionalArrow(GetWorld(), FVector(RelativeForceLocation, 0) + GetOwner()->GetActorLocation(), FVector(RelativeForceLocation + (Force / Mass), 0) + GetOwner()->GetActorLocation(), 5, DebugColor, true, -1.0F, 0U, 0.5f);
		UE_LOG(LogTemp, Warning, TEXT("Add Linear Acceleration to %s: %s"), *GetReadableName(), *(Force / Mass).ToString());
		LinearAcceleration += Force / Mass;
		AngularAcceleration += FMath::RadiansToDegrees((RelativeForceLocation.X * Force.Y - RelativeForceLocation.Y * Force.X) / MomentOfInertia);
	}
}

FVector2D UShipPhysicsComponent::GetVelocity()
{
	return LinearVelocity;
}
FVector2D UShipPhysicsComponent::GetVelocityOfPoint(FVector2D RelativePointLocation)
{
	float AngularVelocity = FMath::DegreesToRadians(GetAngularVelocity());
	return GetVelocity() + FVector2D(-1 * AngularVelocity * RelativePointLocation.Y, AngularVelocity * RelativePointLocation.X);
}

void UShipPhysicsComponent::SetVelocityDirectly(FVector2D NewVelocity)
{
	LinearVelocity = NewVelocity;
}

void UShipPhysicsComponent::UpdateMassCalculations()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Has Updated Mass"), *GetReadableName());

	Mass = Ship->PartGrid->GetMass();
	CenterOfMass = Ship->PartGrid->GetCenterOfMass();
	MomentOfInertia = Ship->PartGrid->GetMomentOfInertia();
	FVector DeltaPos = FVector();

	TArray<UActorComponent*> Comps = TArray<UActorComponent*>();
	Ship->GetComponents(UPrimitiveComponent::StaticClass(), Comps);
	PrimComps = TArray<UPrimitiveComponent*>();
	for (UActorComponent* Comp : Comps)
	{
		PrimComps.Emplace(Cast<UPrimitiveComponent>(Comp));
	}

	bool HasSetDeltaPos = false;
	for (auto& Component : Ship->PartGrid->GetPartGrid())
	{
		FVector NewLoc = FVector(-1 * CenterOfMass + Component.Key, 0);
		if (NewLoc != FVector() && !HasSetDeltaPos)
		{
			HasSetDeltaPos = true;
			DeltaPos = Component.Value.PixelMesh->GetRelativeLocation() - NewLoc;
		}
		Component.Value.PixelMesh->SetRelativeLocation(NewLoc);
	}

	Ship->SetActorLocation(Ship->GetActorLocation() + DeltaPos);
}

float UShipPhysicsComponent::GetAngularVelocity()
{
	return AngularVelocity;
}

float UShipPhysicsComponent::GetMass()
{
	return Mass;
}
