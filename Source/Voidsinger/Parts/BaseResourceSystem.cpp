// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Parts/BaseResourceSystem.h"
#include "Voidsinger/Ships/BaseShip.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

/*
* Function comments from the .h should be copied to the .cpp
*
* This should be deleted because
* If a constuctor is empty it doesn't need to be declared or defined, you can just used the constuctor from its parent.
* - Liam Suggestion
*/
UBaseResourceSystem::UBaseResourceSystem()
{
	//Delete print string or come up with a good way for c++ debug modes - Liam Suggestion
	//UE_LOG(LogTemp, Warning, TEXT("Resource system init"));
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::AddResources(float Amount)
{
	ResourceAmount += Amount;
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
bool UBaseResourceSystem::DrawResources(float Amount)
{
	if (ResourceAmount - Amount < 0)
	{
		/*
		* Resouce amount should not be changed when there is insuficant resources to succeed.
		* This would mean that if you tried to fire a cannon that required 5 ammo but only had 3 you would lose all your ammo while nothing happened.
		* - Liam Suggestion
		*/
		ResourceAmount = 0;
		return false;
	}
	else
	{
		ResourceAmount -= Amount;
		return true;
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::AddPart(UBasePart* AddedPart)
{
	if (IsValid(AddedPart))
	{
		ConnectedParts.Add(AddedPart);

		if (ConnectedParts.Num() == 1)
		{
			OwningShip = Cast<ABaseShip>(ConnectedParts[0]->GetOuter()->GetOuter());
		}

		for (const FIntPoint& PartShape : AddedPart->GetShape())
		{
			ResourceSystemGrid.Emplace(PartShape + AddedPart->GetPartGridLocation(), AddedPart);
		}
	}
	else
	{
		//Delete print string or come up with a good way for c++ debug modes - Liam Suggestion
		UE_LOG(LogTemp, Error, TEXT("Accessed none when trying to add a part to a resource system"))
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::RemovePart(UBasePart* RemovedPart)
{
	ConnectedParts.Remove(RemovedPart);
}

/*
* Function comments from the .h should be copied to the .cpp
* 
* Logic incorect.
* Line 128 does not account for multiple simultaneous breaks caused by the same pixel removal or
* resource system connection points
*
* Large room for improvement in optimization.
* Too many unnessesary nested for loops.
* For optimal solution be sure to used the predicate in TGridMap<>::PointsConnected()
* - Liam Suggestion
*/
void UBaseResourceSystem::RemovePixel(FIntPoint Pixel)
{
	if (ResourceSystemGrid.Contains(Pixel))
	{
		TArray<FIntPoint> NumbersFound;

		/*
		* I've seen lines 98-150 before in PartGridCommponent::DestroyPixel. Should be a function on TGridMap<>
		* This function should have the signatures:
		* 1) void CheckForBreaks(TArray<TGridMap<ValueType>>& NewGrids)
		* 2) void CheckForBreaks(TArray<TGridMap<ValueType>>& NewGrids, Location)
		*
		* This function will set NewGrids to all grids created by: 1) any breaks in the map 2) Any breaks created around Location.
		* 2) assumes pixel at Location is empty.
		*/
		//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
		//Check for previously adjacent parts
		for (auto& i : ConnectedParts)
		{
			//When repeating a task use an iterator/for loop - Liam Suggestion
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

		//Comment should match logic - Liam Suggestion
		//If NumbersFound is less than 2 then you don't need to bother checking anything since there will be no breaks in the system
		if (NumbersFound.Num() > 1)
		{
			//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
			//For each in NumbersFound.Num() - 1 because of how PointsConnected works
			for (int i = 0; i < NumbersFound.Num() - 1; i++)
			{
				TGridMap<FPartData> ConnectedPartsMap = GetMapFromConnectedParts();
				if (ResourceSystemGrid.Contains(NumbersFound[i]) && ResourceSystemGrid.Contains(NumbersFound[i + 1]))
				{
					//This needs to be improved, but right now it checks if the current index is connected to the next index.
					//actually it might not need to be improved but i need to think about it
					if (!ResourceSystemGrid.PointsConnected(NumbersFound[i], NumbersFound[i + 1], AlwaysConnect<UBasePart*>))
					{
						//Bad variable name. What is it storing? - Liam Suggestion
						//If they're not connected, then call FindConnectedShape to figure out what part is not connected. Anything connected to the part that is not connected will
						//also not be connected.
						TArray<FIntPoint> Temp;
						Temp.Emplace(NumbersFound[i + 1]);
						TSet<UBasePart*> RemovedSet;

						//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
						for (auto& j : UPartGridComponent::FindConnectedShape(Temp, ConnectedPartsMap, true))
						{
							RemovedSet.Emplace(ConnectedPartsMap.Find(j)->Part);
						}

						CreateNewSystem(RemovedSet.Array());

					}
				}
			}
		}

		ResourceSystemGrid.Remove(Pixel);
		if (ConnectedParts.IsEmpty() || ResourceSystemGrid.Num() == 0)
		{
			DestroyResourceSystem();
		}
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::MergeSystems(UBaseResourceSystem* MergedSystem)
{
	//You can use += instead of appened - Liam Suggestion
	ConnectedParts.Append(MergedSystem->ConnectedParts);
	if (IsValid(GetWorld()))
	{
		//Delete print string or come up with a good way for c++ debug modes - Liam Suggestion
		//UE_LOG(LogTemp, Warning, TEXT("Merge Systems"));
		//Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->RemoveResourceSystem(MergedSystem);
		OwningShip->RemoveResourceSystem(MergedSystem);
	}
	else
	{
		//Delete print string and else{} or come up with a good way for c++ debug modes - Liam Suggestion
		UE_LOG(LogTemp, Error, TEXT("The world is not valid on the resource system for some unexplicable reason (ask Mabel)"));
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::CreateNewSystem(TArray<UBasePart*> RemovedParts)
{
	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
	for (int i = 0; i < RemovedParts.Num(); i++)
	{
		ConnectedParts.Remove(RemovedParts[i]);
	}

	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>(ThisClass::StaticClass()));
	NewSystem->AddSection(RemovedParts);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddResourceSystem(NewSystem);
}

void UBaseResourceSystem::DestroyResourceSystem()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resource system outer %s"), *GetOuter()->GetClass()->GetDisplayNameText().ToString())
	//Cast<ABaseShip>(GetOuter())->RemoveResourceSystem(this);
	OwningShip->RemoveResourceSystem(this);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::AddSection(TArray<UBasePart*> AddedParts)
{
	//You can use += instead of appened - Liam Suggestion
	ConnectedParts.Append(AddedParts);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::RemoveSection(TArray<UBasePart*> RemovedParts)
{
	/*
	* Iterator should have a name that tells what it actualy is and what its iterating through.
	* 
	* Use a for each loop insead.
	* - Liam Suggestion
	*/ 
	for (int i = 0; i < RemovedParts.Num() - 1; i++)
	{
		RemovePart(RemovedParts[i]);
	}
}

//Delete outdated function - Liam Suggestion
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

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
TEnumAsByte<EResourceType> UBaseResourceSystem::GetType()
{
	return SystemType;
}

/*
* Function comments from the .h should be copied to the .cpp 
* 
* This function should not exist because it makes no sense for a resouse system to change types.
* - Liam Suggestion
*/ 
void UBaseResourceSystem::SetType(TEnumAsByte<EResourceType> Type)
{
	//Delete print string or come up with a good way for c++ debug modes - Liam Suggestion
	//UE_LOG(LogTemp, Warning, TEXT("New Resource System Type = %i"), Type.GetValue());
	SystemType = Type;
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
UWorld* UBaseResourceSystem::GetWorld() const
{
	if (ConnectedParts.Num() > 0)
	{
		/*
		* Hard coded.
		* What if the part isn't valid?.
		* Why index [0].
		* Using the get world of the ship would be preferable.
		* - Liam Suggestion
		*/ 
		return ConnectedParts[0]->GetWorld();
	}
	else
	{
		//Delete print string or come up with a good way for c++ debug modes - Liam Suggestion
		UE_LOG(LogTemp, Error, TEXT("You forgot to add a part to the system before calling MergeSystem() or CreateNewSystem(). AddPart(). Ask Mabel."));
		return nullptr;
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
TGridMap<FPartData> UBaseResourceSystem::GetMapFromConnectedParts()
{
	//Bad variable name. What is it storing? - Liam Suggestion
	TGridMap<FPartData> Temp;

	/*
	* Iterator should have a name that tells what it actualy is and what its iterating through.
	* Don't use auto&.
	* - Liam Suggestion
	*/ 
	for (auto& i : ConnectedParts)
	{
		/*
		* Iterator should have a name that tells what it actualy is and what its iterating through.
		* Don't use auto&.
		* 
		* Logic is incorect not all pixels on a part are guaranteed to be connected to the resouce system.
		* - Liam Suggestion
		*/
		for (auto& j : i->GetShape())
		{
			FPartData PartData = FPartData(i, 0.f, (int32)0, i->GetPixelMaterial());
			Temp.Emplace(j, PartData);
		}
	}
	return Temp;
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
float UBaseResourceSystem::GetResourceAmount()
{
	return ResourceAmount;
}


