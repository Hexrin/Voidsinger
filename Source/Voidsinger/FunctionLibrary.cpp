// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary.h"
#include "DrawDebugHelpers.h" // Remove this when debugging is done
#include "Parts/BasePart.h"
#include "Ships/BaseShip.h"


FVector UFunctionLibrary::SafeDivide(FVector V1, FVector V2)
{
	return FVector((V2.X != 0.0f) ? (V1.X / V2.X) : 0.0f, (V2.Y != 0.0f) ? (V1.Y / V2.Y) : 0.0f, (V2.Z != 0.0f) ? (V1.Z / V2.Z) : 0.0f);
}

TArray<UClass*> UFunctionLibrary::GetClasses(UClass* ParentClass)
{
	UE_LOG(LogTemp, Error, TEXT("Don't Get Classes at runtime!!"));
	TArray<UClass*> Results;

	// get our parent blueprint class
	if (IsValid(ParentClass))
	{
		const FString ParentClassName = ParentClass->GetName();
		UObject* ClassPackage = ANY_PACKAGE;
		UClass* ParentBPClass = FindObject<UClass>(ClassPackage, *ParentClassName);

		// iterate over UClass, this might be heavy on performance, so keep in mind..
		// better suggestions for a check are welcome
		for (TObjectIterator<UClass> It; It; ++It)
		{
			if (It->IsChildOf(ParentBPClass))
			{
				// It is a child of the Parent Class
				// make sure we don't include our parent class in the array (weak name check, suggestions welcome)
				if (It->GetName() != ParentClassName)
				{
					Results.Add(*It);
				}
			}
		}

		return Results;
	}

	return TArray<UClass*>();
}

void UFunctionLibrary::ExplodeAtWorldLocation(const UObject* WorldContextObject, FVector WorldLocation, float ExplosionRadius)
{
	TArray <TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	TArray<AActor*> OutActors;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	UKismetSystemLibrary::SphereOverlapActors(World, WorldLocation, ExplosionRadius, ObjectTypeArray, ABaseShip::StaticClass(),TArray<AActor*>(), OutActors);

	//DrawDebugSphere(World, WorldLocation, ExplosionRadius, 32, FColor::Red, true);

	for (auto& i : OutActors)
	{
		if (IsValid(Cast<ABaseShip>(i)))
		{
			Cast<ABaseShip>(i)->PartGrid->ExplodeAtLocation(WorldLocation, ExplosionRadius);
		}
	}
}

int UFunctionLibrary::GetBitNumberFromLocation(FIntPoint Location)
{
	if (Location == FIntPoint(0, 0))
	{
		return 0;
	}
	else if (Location == FIntPoint(0, -1))
	{
		return 1;
	}
	else if (Location == FIntPoint(-1, 0))
	{
		return 2;
	}
	else if (Location == FIntPoint(0, 1))
	{
		return 4;
	}
	else if (Location == FIntPoint(1, 0))
	{
		return 8;
	}
	else if (Location == FIntPoint(-1, -1))
	{
		return 16;
	}
	else if (Location == FIntPoint(-1, 1))
	{
		return 32;
	}
	else if (Location == FIntPoint(1, 1))
	{
		return 64;
	}
	else if (Location == FIntPoint(1, -1))
	{
		return 128;
	}
	return 0;
}

TArray<UBasePart*> UFunctionLibrary::GetPartsHitFromWorldLocation(FVector Location, UPartGridComponent* PartGrid)
{

	FVector RelativeLoc = UKismetMathLibrary::InverseTransformLocation(PartGrid->GetOwner()->GetActorTransform(), Location) + FVector(Cast<ABaseShip>(PartGrid->GetOwner())->PhysicsComponent->GetCenterOfMass(), 0);
	FIntPoint LocalIntPoint = FVector2D(RelativeLoc.X, RelativeLoc.Y).IntPoint();

	for (int i = 0; i < 4; i++)
	{
		LocalIntPoint.X + (i % 2);
		if (i > 1)
		{
			LocalIntPoint.Y + 1;
		}
		else
		{
			LocalIntPoint.Y;
		}
	}
}

