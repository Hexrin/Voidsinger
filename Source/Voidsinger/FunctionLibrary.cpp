// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary.h"
#include "DrawDebugHelpers.h" // Remove this when debugging is done
#include "Parts/BasePart.h"
#include "Ships/BaseShip.h"

//Function comments from the .h should be copied to the .cpp. - Liam Suggestion
TArray<UClass*> UFunctionLibrary::GetClasses(UClass* ParentClass)
{
	//Delete print string or come up with a good way for c++ debug modes. - Liam Suggestion
	//UE_LOG(LogTemp, Error, TEXT("Don't Get Classes at runtime!!"));
	TArray<UClass*> Results;

	//Our doesnt make sense - Liam Suggestion
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

//Function comments from the .h should be copied to the .cpp. - Liam Suggestion
void UFunctionLibrary::ExplodeAtWorldLocation(const UObject* WorldContextObject, FVector WorldLocation, float ExplosionRadius)
{
	TArray <TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	TArray<AActor*> OutActors;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	UKismetSystemLibrary::SphereOverlapActors(World, WorldLocation, ExplosionRadius, ObjectTypeArray, ABaseShip::StaticClass(),TArray<AActor*>(), OutActors);

	//Delete debug code or come up with a good way for c++ debug modes. - Liam Suggestion
	//DrawDebugSphere(World, WorldLocation, ExplosionRadius, 32, FColor::Red, true);
	
	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
	for (auto& i : OutActors)
	{
		if (IsValid(Cast<ABaseShip>(i)))
		{
			Cast<ABaseShip>(i)->PartGrid->ExplodeAtLocation(WorldLocation, ExplosionRadius);
		}
	}
}

//Function comments from the .h should be copied to the .cpp. - Liam Suggestion
int UFunctionLibrary::GetBitNumberFromLocation(FIntPoint Location)
{
	//There is definently a way to do this with a loop & some math
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

/*
* Function comments from the .h should be copied to the .cpp. 
* 
* Shouldnt this be on the PartGrid as it already requries a refernce to one?
* 
* Shouldnt this only ever return one part?
* - Liam Suggestion
*/
TArray<UBasePart*> UFunctionLibrary::GetPartsHitFromWorldLocation(FVector Location, UPartGridComponent* PartGrid)
{

	TSet<UBasePart*> PartsHit;

	FVector RelativeLoc = UKismetMathLibrary::InverseTransformLocation(PartGrid->GetOwner()->GetActorTransform(), Location) + FVector(Cast<ABaseShip>(PartGrid->GetOwner())->PhysicsComponent->GetCenterOfMass(), 0);
	FIntPoint LocalIntPoint = FVector2D(RelativeLoc.X, RelativeLoc.Y).IntPoint();
	FIntPoint CheckGridLocation;

	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
	for (int i = 0; i < 4; i++)
	{
		CheckGridLocation.X = LocalIntPoint.X + (i % 2);
		if (i > 1)
		{
			CheckGridLocation.Y = LocalIntPoint.Y + 1;
		}
		else
		{
			CheckGridLocation.Y = LocalIntPoint.Y;
		}

		if (PartGrid->GetPartGrid().Contains(CheckGridLocation))
		{
			PartsHit.Emplace(PartGrid->GetPartGrid().Find(CheckGridLocation)->Part);
		}

	}

	return PartsHit.Array();

}

