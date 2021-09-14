// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Parts/BaseResourceSystem.h"
#include "Voidsinger/Ships/BaseShip.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UBaseResourceSystem::UBaseResourceSystem()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resource system init"));
}

void UBaseResourceSystem::AddResources(float Amount)
{
	ResourceAmount += Amount;
}

void UBaseResourceSystem::DrawResources(float Amount)
{
	ResourceAmount -= Amount;
}

void UBaseResourceSystem::AddPart(UBasePart* AddedPart)
{
	ConnectedParts.Add(AddedPart);
}

void UBaseResourceSystem::RemovePart(UBasePart* RemovedPart)
{

	ConnectedParts.Remove(RemovedPart);

}

void UBaseResourceSystem::RemovePixel(FIntPoint Pixel)
{
	if (GetMapFromConnectedParts().Contains(Pixel))
	{
		TArray<FIntPoint> NumbersFound;

		//Check for previously adjacent parts
		for (auto& i : ConnectedParts)
		{
			if (i->GetShape().Contains(FIntPoint(Pixel.X + 1, Pixel.Y)))
			{
				NumbersFound.Add(FIntPoint(Pixel.X + 1, Pixel.Y));
			}
			if (i->GetShape().Contains(FIntPoint(Pixel.X - 1, Pixel.Y)))
			{
				NumbersFound.Add(FIntPoint(Pixel.X - 1, Pixel.Y));
			}
			if (i->GetShape().Contains(FIntPoint(Pixel.X, Pixel.Y + 1)))
			{
				NumbersFound.Add(FIntPoint(Pixel.X, Pixel.Y + 1));
			}
			if (i->GetShape().Contains(FIntPoint(Pixel.X, Pixel.Y - 1)))
			{
				NumbersFound.Add(FIntPoint(Pixel.X, Pixel.Y - 1));
			}
		}

		//If NumbersFound is less than 2 then you don't need to bother checking anything since there will be no breaks in the system
		if (NumbersFound.Num() > 1)
		{

			//For each in NumbersFound.Num() - 1 because of how PointsConnected works
			for (int i = 0; i < NumbersFound.Num() - 1; i++)
			{
				//This needs to be improved, but right now it checks if the current index is connected to the next index.
				//actually it might not need to be improved but i need to think about it
				if (!UFunctionLibrary::PointsConnectedWithFunctionality(GetMapFromConnectedParts(), NumbersFound[i], NumbersFound[i + 1]))
				{
					//If they're not connected, then call FindConnectedShape to figure out what part is not connected. Anything connected to the part that is not connected will
					//also not be connected.
					TArray<FIntPoint> Temp;
					Temp.Emplace(NumbersFound[i + 1]);
					TMap<FIntPoint, FPartData> ConnectedPartsMap = GetMapFromConnectedParts();
					TSet<UBasePart*> RemovedSet;
					for (auto& j : FindConnectedShape(ConnectedParts, Temp))
					{
						RemovedSet.Emplace(ConnectedPartsMap.Find(j)->Part);
					}
					CreateNewSystem(RemovedSet.Array());

					//since there will never be more than 1 system removed at a time, this should not need to continue after this point
					break;
				}
			}
		}
	}
}

void UBaseResourceSystem::MergeSystems(UBaseResourceSystem* MergedSystem)
{
	ConnectedParts.Append(MergedSystem->ConnectedParts);
	if (IsValid(GetWorld()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Merge Systems"));
		Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->RemoveResourceSystem(MergedSystem);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The world is not valid on the resource system for some unexplicable reason (ask Mabel)"));
	}
}

void UBaseResourceSystem::CreateNewSystem(TArray<UBasePart*> RemovedParts)
{
	for (int i = 0; i < RemovedParts.Num(); i++)
	{
		ConnectedParts.Remove(RemovedParts[i]);
	}

	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>(ThisClass::StaticClass()));
	NewSystem->AddSection(RemovedParts);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddResourceSystem(NewSystem);

}

void UBaseResourceSystem::AddSection(TArray<UBasePart*> AddedParts)
{
	ConnectedParts.Append(AddedParts);
}

void UBaseResourceSystem::RemoveSection(TArray<UBasePart*> RemovedParts)
{
	for (int i = 0; i < RemovedParts.Num() - 1; i++)
	{
		RemovePart(RemovedParts[i]);
	}
}

//Finds the shape that is connected to the shape given
TArray<FIntPoint> UBaseResourceSystem::FindConnectedShape(TArray<UBasePart*> Parts, TArray<FIntPoint> Shape)
{

	//Initalize ConnectedPartsMap here so GetMapFromConnectedParts doesn't have to be called every single time
	TMap<FIntPoint, FPartData> ConnectedPartsMap = GetMapFromConnectedParts();

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
				//And the pixel at that location is functional
				if (ConnectedPartsMap.Find(FIntPoint(i.X + 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X + 1, i.Y)))
				{
					//Add that location to the new shape, because it is connected
					NewShape.Emplace(FIntPoint(i.X + 1, i.Y));
				}
			}
		}

		//Do the same thing done for X + 1 for X - 1
		if (!Shape.Contains(FIntPoint(i.X - 1, i.Y)))
		{
			if (ConnectedPartsMap.Contains(FIntPoint(i.X - 1, i.Y)))
			{
				if (ConnectedPartsMap.Find(FIntPoint(i.X - 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X - 1, i.Y)))
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
				if (ConnectedPartsMap.Find(FIntPoint(i.X, i.Y + 1))->Part->IsPixelFunctional(FIntPoint(i.X , i.Y + 1)))
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
				if (ConnectedPartsMap.Find(FIntPoint(i.X, i.Y - 1))->Part->IsPixelFunctional(FIntPoint(i.X, i.Y - 1)))
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
		NewShape = FindConnectedShape(Parts, NewShape);
	}

	//Once everything has figured itself out, return the New Shape
	return NewShape;
}

//Deprecated function to check if 2 shapes are adjacent to each other.
//bool UBaseResourceSystem::AreShapesAdjacent(TArray<FIntPoint> Shape1, TArray<FIntPoint> Shape2)
//{
//
//	for (int i = 0; i < Shape1.Num(); i++)
//	{
//		for (int j = 0; j < Shape2.Num(); i++)
//		{
//			if (UKismetMathLibrary::InRange_FloatFloat(float(Shape1[i].X), float(Shape2[j].X - 1), float(Shape2[j].X + 1)) && UKismetMathLibrary::InRange_FloatFloat(float(Shape1[i].Y), float(Shape2[j].Y - 1), float(Shape2[j].Y + 1)))
//			{
//				return true;
//			}
//		}
//	}
//	
//	return false;
//}

TEnumAsByte<EResourceType> UBaseResourceSystem::GetType()
{
	return SystemType;
}

void UBaseResourceSystem::SetType(TEnumAsByte<EResourceType> Type)
{
	SystemType = Type;
}

UWorld* UBaseResourceSystem::GetWorld() const
{

	if (ConnectedParts.Num() > 0)
	{
		return ConnectedParts[0]->GetWorld();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("You forgot to add a part to the system before calling MergeSystem() or CreateNewSystem(). AddPart(). Ask Mabel."));
		return nullptr;
	}
}

TMap<FIntPoint, FPartData> UBaseResourceSystem::GetMapFromConnectedParts()
{
	TMap<FIntPoint, FPartData> Temp;
	for (auto& i : ConnectedParts)
	{
		for (auto& j : i->GetShape())
		{
			Temp.Emplace(j, FPartData(i, 0, nullptr));
		}
	}
	return Temp;
}


