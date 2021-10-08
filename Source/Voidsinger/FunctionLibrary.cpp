// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary.h"
#include "Parts/BasePart.h"


//Returns true if StartPoint and EndPoint are connected via PartGrid
bool UFunctionLibrary::PointsConnected(TMap<FIntPoint, FPartData> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint)
{
	//Initate Conectiveity Array
	TArray<FIntPoint> ConectivityArray = TArray<FIntPoint>();
	return PointsConnected(PartGrid, StartPoint, EndPoint, ConectivityArray);
}
bool UFunctionLibrary::PointsConnected(TMap<FIntPoint, FPartData> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConnectivityArray)
{
	//Detect if funtion has reached target
	if (StartPoint == EndPoint)
	{
		return true;
	}
	
	//Prevent Infinte Loops
	ConnectivityArray.Emplace(StartPoint);

	//Initate Variables
	bool ReturnValue = false;
	const bool IsXCloser = abs((EndPoint - StartPoint).X) < abs((EndPoint - StartPoint).Y);
	bool XIsPosive = (EndPoint - StartPoint).X > 0;
	bool YIsPosive = (EndPoint - StartPoint).Y > 0;
	//UE_LOG(LogTemp, Warning, TEXT("Direction x=%i, y=%i"), (EndPoint - StartPoint).X, (EndPoint - StartPoint).Y);


	//Iterate though and run recursive function for all adjecent pixels
	for (int i = 0; i < 4; i++)
	{
		//Select next pixel to scan based of of direction to EndPoint
		FIntPoint TargetPoint = (!IsXCloser ^ (i % 2 == 1)) ? FIntPoint((XIsPosive ^ (i > 1)) ? 1 : -1, 0) : FIntPoint(0, (YIsPosive ^ (i > 1)) ? 1 : -1);
		//UE_LOG(LogTemp, Warning, TEXT("Target Point x=%i, y=%i, Xclose=%i, Xpos=%i, Ypos=%i"), TargetPoint.X, TargetPoint.Y, (IsXCloser ^ (i % 2 == 1)) ? 1 : 0, !(XIsPosive ^ (i > 1)) ? 1 : 0, !(YIsPosive ^ (i > 1)) ? 1 : 0);
		
		//Scan Pixel
		if (!ConnectivityArray.Contains(StartPoint + TargetPoint) && PartGrid.Contains(StartPoint + TargetPoint))
		{
			ReturnValue = PointsConnected(PartGrid, StartPoint + TargetPoint, EndPoint, ConnectivityArray);
			if (ReturnValue)
			{
				break;
			}
		}
	}


	return ReturnValue;
}

//Returns true if StartPoint and EndPoint are connected via PartGrid
bool UFunctionLibrary::PointsConnectedWithFunctionality(TMap<FIntPoint, FPartData> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint)
{
	//Initate Conectiveity Array
	TArray<FIntPoint> ConectivityArray = TArray<FIntPoint>();
	return PointsConnected(PartGrid, StartPoint, EndPoint, ConectivityArray);
}
bool UFunctionLibrary::PointsConnectedWithFunctionality(TMap<FIntPoint, FPartData> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConnectivityArray)
{
	//Detect if funtion has reached target
	if (StartPoint == EndPoint)
	{
		return true;
	}

	//Prevent Infinte Loops
	ConnectivityArray.Emplace(StartPoint);

	//Initate Variables
	bool ReturnValue = false;
	const bool IsXCloser = abs((EndPoint - StartPoint).X) < abs((EndPoint - StartPoint).Y);
	bool XIsPosive = (EndPoint - StartPoint).X > 0;
	bool YIsPosive = (EndPoint - StartPoint).Y > 0;
	//UE_LOG(LogTemp, Warning, TEXT("Direction x=%i, y=%i"), (EndPoint - StartPoint).X, (EndPoint - StartPoint).Y);


	//Iterate though and run recursive function for all adjecent pixels
	for (int i = 0; i < 4; i++)
	{
		//Select next pixel to scan based of of direction to EndPoint
		FIntPoint TargetPoint = (!IsXCloser ^ (i % 2 == 1)) ? FIntPoint((XIsPosive ^ (i > 1)) ? 1 : -1, 0) : FIntPoint(0, (YIsPosive ^ (i > 1)) ? 1 : -1);
		//UE_LOG(LogTemp, Warning, TEXT("Target Point x=%i, y=%i, Xclose=%i, Xpos=%i, Ypos=%i"), TargetPoint.X, TargetPoint.Y, (IsXCloser ^ (i % 2 == 1)) ? 1 : 0, !(XIsPosive ^ (i > 1)) ? 1 : 0, !(YIsPosive ^ (i > 1)) ? 1 : 0);

		//Scan Pixel
		if (!ConnectivityArray.Contains(StartPoint + TargetPoint) && PartGrid.Contains(StartPoint + TargetPoint) && PartGrid.Find(StartPoint + TargetPoint)->Part->IsPixelFunctional(StartPoint + TargetPoint))
		{
			ReturnValue = PointsConnected(PartGrid, StartPoint + TargetPoint, EndPoint, ConnectivityArray);
			if (ReturnValue)
			{
				break;
			}
		}
	}


	return ReturnValue;
}


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

bool UFunctionLibrary::SetActorTransformSweepComponets(AActor* Target, FHitResult& Hit, TArray<UPrimitiveComponent*> PrimComps, const FTransform& Start, const FTransform& End)
{
	Hit = FHitResult();
	TArray<FHitResult> Hits;
	bool ReturnValue = true;
	FQuat Rot = Start.Rotator().GetNormalized().Quaternion();
	FCollisionShape Shape = FCollisionShape();
	Shape.MakeBox(FVector(0.5f));
	FTransform DeltaTransform = (End.Inverse() * Start);

	for (UPrimitiveComponent* Comp : PrimComps)
	{
		FVector StartLoc = Comp->GetComponentLocation();
		FVector EndLoc = DeltaTransform.TransformVector(StartLoc);
		FHitResult ThisHit = FHitResult();
		ReturnValue = Target->GetWorld()->SweepSingleByObjectType(ThisHit, StartLoc, EndLoc, Rot, FCollisionObjectQueryParams(), Shape);
		if (ReturnValue)
		{
			Hits.Emplace(ThisHit);
		}
	}

	if (ReturnValue)
	{
		for (FHitResult Value : Hits)
		{
			if (Value.Time < Hit.Time)
			{
				Hit = Value;
			}
		}
	}
	else
	{
		Target->SetActorTransform(End);
	}
	return ReturnValue;
}

TArray<FIntPoint> UFunctionLibrary::FindConnectedShape(TArray<FIntPoint> Shape, TMap<FIntPoint, FPartData> ConnectedPartsMap, bool CheckFunctionality)
{

	//New shape will return the entire connected shape, indcluding the starting shape
	TArray<FIntPoint> NewShape = Shape;

	//Check each pixel of the shape
	for (auto& i : Shape)
	{
		//If the shape does NOT contain the checked location
		if (!Shape.Contains(FIntPoint(i.X + 1, i.Y)))
		{
			//And the connected parts ARE at that location
			if (ConnectedPartsMap.Contains(FIntPoint(i.X + 1, i.Y)))
			{
				if (CheckFunctionality)
				{
					//And the pixel at that location is functional
					if (ConnectedPartsMap.Find(FIntPoint(i.X + 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X + 1, i.Y)))
					{
						//Add that location to the new shape, because it is connected
						NewShape.Emplace(FIntPoint(i.X + 1, i.Y));
					}
				}
				else
				{
					NewShape.Emplace(FIntPoint(i.X + 1, i.Y));
				}
			}
		}

		//Do the same thing done for X + 1 for X - 1
		if (!Shape.Contains(FIntPoint(i.X - 1, i.Y)))
		{
			if (ConnectedPartsMap.Contains(FIntPoint(i.X - 1, i.Y)))
			{
				if (CheckFunctionality)
				{
					if (ConnectedPartsMap.Find(FIntPoint(i.X - 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X - 1, i.Y)))
					{
						NewShape.Emplace(FIntPoint(i.X - 1, i.Y));
					}
				}
				else
				{
					NewShape.Emplace(FIntPoint(i.X - 1, i.Y));
				}
			}
		}

		//Do the same thing done for X + 1 for Y + 1
		if (!Shape.Contains(FIntPoint(i.X, i.Y + 1)))
		{
			if (ConnectedPartsMap.Contains(FIntPoint(i.X, i.Y + 1)))
			{
				if (CheckFunctionality)
				{
					if (ConnectedPartsMap.Find(FIntPoint(i.X, i.Y + 1))->Part->IsPixelFunctional(FIntPoint(i.X, i.Y + 1)))
					{
						NewShape.Emplace(FIntPoint(i.X, i.Y + 1));
					}
				}
				else
				{
					NewShape.Emplace(FIntPoint(i.X, i.Y + 1));
				}
			}
		}

		//Do the same thing done for X + 1 for Y - 1
		if (!Shape.Contains(FIntPoint(i.X, i.Y - 1)))
		{
			if (ConnectedPartsMap.Contains(FIntPoint(i.X, i.Y - 1)))
			{
				if (CheckFunctionality)
				{
					if (ConnectedPartsMap.Find(FIntPoint(i.X, i.Y - 1))->Part->IsPixelFunctional(FIntPoint(i.X, i.Y - 1)))
					{
						NewShape.Emplace(FIntPoint(i.X, i.Y - 1));
					}
				}
				else
				{
					NewShape.Emplace(FIntPoint(i.X, i.Y - 1));
				}
			}
		}
	}

	//If the new shape has changed at all
	if (NewShape != Shape)
	{
		//Continue to check for connections by calling the function recursively.
		NewShape = FindConnectedShape(NewShape, ConnectedPartsMap, CheckFunctionality);
	}

	//Once everything has figured itself out, return the New Shape
	return NewShape;
}

