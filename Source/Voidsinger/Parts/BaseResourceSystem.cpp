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

		for (const FIntPoint& PartShape : AddedPart->GetShape())
		{
			FPartData PartData;
			PartData.Part = AddedPart;

			if (IsValid(PartData.Part))
			{
				UE_LOG(LogTemp, Warning, TEXT(" is valid though? "));
			}
			ResourceSystemGrid.Emplace(PartShape + AddedPart->GetPartGridLocation(), PartData);

			GetOwningShip();
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

	UE_LOG(LogTemp, Warning, TEXT("Remove pixel called"));
	if (ResourceSystemGrid.Contains(Pixel))
	{

		ResourceSystemGrid.Remove(Pixel);

		UE_LOG(LogTemp, Warning, TEXT("grid contains the pixel removed + pixel should be removed?"));

		if (!ResourceSystemGrid.Num() == 0)
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
			/*for (auto& i : ConnectedParts)
			{
				UE_LOG(LogTemp, Warning, TEXT("There are connected parts"));
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
			}*/

			if (ResourceSystemGrid.Contains(FIntPoint(Pixel.X + 1, Pixel.Y)))
			{
				NumbersFound.Add(FIntPoint(Pixel.X + 1, Pixel.Y));
			}
			if (ResourceSystemGrid.Contains(FIntPoint(Pixel.X - 1, Pixel.Y)))
			{
				NumbersFound.Add(FIntPoint(Pixel.X - 1, Pixel.Y));
			}
			if (ResourceSystemGrid.Contains(FIntPoint(Pixel.X, Pixel.Y + 1)))
			{
				NumbersFound.Add(FIntPoint(Pixel.X, Pixel.Y + 1));
			}
			if (ResourceSystemGrid.Contains(FIntPoint(Pixel.X, Pixel.Y - 1)))
			{
				NumbersFound.Add(FIntPoint(Pixel.X, Pixel.Y - 1));
			}


			//Comment should match logic - Liam Suggestion
			//If NumbersFound is less than 2 then you don't need to bother checking anything since there will be no breaks in the system
			if (NumbersFound.Num() > 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("numbers found num is greater than 1"));
				//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
				//For each in NumbersFound.Num() - 1 because of how PointsConnected works
				for (int i = 0; i < NumbersFound.Num() - 1; i++)
				{
					//TGridMap<FPartData> ConnectedPartsMap = GetMapFromConnectedParts();

					if (ResourceSystemGrid.Contains(NumbersFound[i]) && ResourceSystemGrid.Contains(NumbersFound[i + 1]))
					{
						//This needs to be improved, but right now it checks if the current index is connected to the next index.
						//actually it might not need to be improved but i need to think about it

						UE_LOG(LogTemp, Warning, TEXT("resource system grid contains both locations"));
						if (!ResourceSystemGrid.PointsConnected(NumbersFound[i], NumbersFound[i + 1], AlwaysConnect<FPartData>))
						{
							//Bad variable name. What is it storing? - Liam Suggestion
							//If they're not connected, then call FindConnectedShape to figure out what part is not connected. Anything connected to the part that is not connected will
							//also not be connected.
							UE_LOG(LogTemp, Warning, TEXT("Points are not connected"));
							TSet<FIntPoint> Temp;
							Temp.Emplace(NumbersFound[i + 1]);
							TGridMap<FPartData> RemovedGridMap;

							//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
							for (auto& j : UPartGridComponent::FindConnectedShape(Temp, ResourceSystemGrid, true))
							{
								//if (IsValid(ConnectedPartsMap.Find))
								RemovedGridMap.Emplace(j, ResourceSystemGrid.FindRef(j));
							}

							if (!RemovedGridMap.Num() == 0)
							{
								CreateNewSystem(RemovedGridMap, GetType());
							}
						}
					}
				}
			}
		}
		else
		{
			DestroyResourceSystem();
		}
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::MergeSystems(UBaseResourceSystem* MergedSystem)
{

	if (MergedSystem != this)
	{
		//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResourceType"), true);

		//UE_LOG(LogTemp, Warning, TEXT("Merged system %s"), *MergedSystem->GetFName().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Merged system type %s"), *EnumPtr->GetDisplayNameTextByValue(MergedSystem->GetType().GetValue()).ToString());

		//UE_LOG(LogTemp, Warning, TEXT("This system % s"), *GetFName().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("this system type %s"), *EnumPtr->GetDisplayNameTextByValue(GetType().GetValue()).ToString());

		//You can use += instead of appened - Liam Suggestion
		//ConnectedParts.Append(MergedSystem->ConnectedParts);

		AddResources(MergedSystem->GetResourceAmount());

		for (int OtherGridIndex = 0; OtherGridIndex < MergedSystem->ResourceSystemGrid.Num(); OtherGridIndex++)
		{
			UBasePart* PartMergedIn = MergedSystem->ResourceSystemGrid.ValueAtIndex(OtherGridIndex).Part;
			//UE_LOG(LogTemp, Warning, TEXT("part merged in class %s"), *PartMergedIn->GetClass()->GetDisplayNameText().ToString())
			ResourceSystemGrid.Emplace(MergedSystem->ResourceSystemGrid.LocationAtIndex(OtherGridIndex), MergedSystem->ResourceSystemGrid.ValueAtIndex(OtherGridIndex));

			for (UBaseResourceSystem* Systems : PartMergedIn->GetSystems())
			{
				//UE_LOG(LogTemp, Warning, TEXT("systems on part merged in before change %s"), *Systems->GetFName().ToString())
				//UE_LOG(LogTemp, Warning, TEXT("systems on part merged in type before change %s"), *EnumPtr->GetDisplayNameTextByValue(Systems->GetType().GetValue()).ToString());
			}

			PartMergedIn->GetSystems().Remove(MergedSystem);
			PartMergedIn->GetSystems().Emplace(this);

			for (UBaseResourceSystem* Systems : PartMergedIn->GetSystems())
			{
				//UE_LOG(LogTemp, Warning, TEXT("systems on part merged in after change %s"), *Systems->GetFName().ToString())
				//UE_LOG(LogTemp, Warning, TEXT("systems on part merged in type after change %s"), *EnumPtr->GetDisplayNameTextByValue(Systems->GetType().GetValue()).ToString());
			}

		}

		//if (IsValid(GetWorld()))
		//{
			//Delete print string or come up with a good way for c++ debug modes - Liam Suggestion
			//UE_LOG(LogTemp, Warning, TEXT("Merge Systems"));
			//Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->RemoveResourceSystem(MergedSystem);
		GetOwningShip()->RemoveResourceSystem(MergedSystem);

			//UE_LOG(LogTemp, Warning, TEXT("------------------------"));
		//}
		//else
		//{
			//Delete print string and else{} or come up with a good way for c++ debug modes - Liam Suggestion
			//UE_LOG(LogTemp, Error, TEXT("The world is not valid on a resource system."));
		//}
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::CreateNewSystem(TGridMap<FPartData> ResourceGrid, EResourceType Type)
{

	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResourceType"), true);

	//UE_LOG(LogTemp, Warning, TEXT("Create new system type %s"), *EnumPtr->GetDisplayNameText(GetType().GetValue()).ToString());

	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
	/*for (int i = 0; i < RemovedParts.Num(); i++)
	{
		ConnectedParts.Remove(RemovedParts[i]);
	}*/

	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>(ThisClass::StaticClass()));
	NewSystem->SetType(Type);
	NewSystem->AddSection(ResourceGrid);
	GetOwningShip()->AddResourceSystem(NewSystem);
}

void UBaseResourceSystem::DestroyResourceSystem()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resource system outer %s"), *GetOuter()->GetClass()->GetDisplayNameText().ToString())
	//Cast<ABaseShip>(GetOuter())->RemoveResourceSystem(this);
	GetOwningShip()->RemoveResourceSystem(this);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseResourceSystem::AddSection(TGridMap<FPartData> AddedResourceGrid)
{
	//You can use += instead of appened - Liam Suggestion
	//ConnectedParts.Append(AddedParts);
	TArray<FIntPoint> Keys = AddedResourceGrid.GetKeyArray();
	TArray<FPartData> Values = AddedResourceGrid.GetValueArray();

	for (int i = 0; i < AddedResourceGrid.Num(); i++)
	{
		ResourceSystemGrid.Emplace(Keys[i], Values[i]);
		GetOwningShip();
		Values[i].Part->AddToSystem(this);
		/*if (IsValid(Values[i].Part->GetSystemByType(GetType())))
		{
			if (ResourceSystemGrid.GetKeyArray() != Values[i].Part->GetSystemByType(GetType())->ResourceSystemGrid.GetKeyArray() || ResourceSystemGrid.GetValueArray() != Values[i].Part->GetSystemByType(GetType())->ResourceSystemGrid.GetValueArray())
			{
				Values[i].Part->AddToSystem(this);
			}
			else
			{
				DestroyResourceSystem();
			}
		}
		else
		{
			Values[i].Part->AddToSystem(this);
		}*/
	}
}

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
	TGridMap<FPartData> CPlusPlusIsDumbVar = ResourceSystemGrid;
	return CPlusPlusIsDumbVar.ValueAtIndex(0).Part->GetWorld();
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

ABaseShip* UBaseResourceSystem::GetOwningShip()
{
	if (!IsValid(OwningShip))
	{
		OwningShip = ResourceSystemGrid.ValueAtIndex(0).Part->GetShip();
	}

	return OwningShip;
}
