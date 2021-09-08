// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"
#include "BaseResourceSystem.h"
#include "Engine/Engine.h"

UBasePart::UBasePart()
{
	Rotation = EPartRotation::Degrees0;
	Location = FIntPoint();
	Mass = 1;
	Cost = 1;
	DesiredShape = TArray<FIntPoint>();
	Bounds = FArrayBounds();
	RotatedShape = TArray<FIntPoint>();
	ActualShape = TArray<FIntPoint>();
}

const TArray<FIntPoint> UBasePart::GetDesiredShape()
{
	
	return GetDesiredShape(Rotation);
}

const TArray<FIntPoint> UBasePart::GetDesiredShape(TEnumAsByte<EPartRotation> Rot)
{
	if (0 == RotatedShape.Num())
	{
		for (int i = 0; i < DesiredShape.Num(); i++)
		{
			switch (Rot)
			{
			case Degrees0:
				RotatedShape.Emplace(DesiredShape[i]);
				break;
			case Degrees90:
				RotatedShape.Emplace(FIntPoint(-DesiredShape[i].Y, DesiredShape[i].X));
				break;
			case Degrees180:
				RotatedShape.Emplace(DesiredShape[i] * -1);
				break;
			case Degrees270:
				RotatedShape.Emplace(FIntPoint(DesiredShape[i].Y, -DesiredShape[i].X));
				break;
			default:
				break;
			}
		}
	}
	return RotatedShape;
}

const FArrayBounds UBasePart::GetShapeBounds()
{
	return GetShapeBounds(Rotation);
}

const FArrayBounds UBasePart::GetShapeBounds(TEnumAsByte<EPartRotation> Rot)
{
	if (Bounds.LowerBounds == FArrayBounds().LowerBounds && Bounds.UpperBounds == FArrayBounds().UpperBounds)
	{
		
		for (FIntPoint i : GetDesiredShape(Rot))
		{
			if (i.X > Bounds.UpperBounds.X)
			{
				Bounds.UpperBounds.X = i.X;
			}
			if (i.Y > Bounds.UpperBounds.Y)
			{
				Bounds.UpperBounds.Y = i.Y;
			}

			if (i.X < Bounds.LowerBounds.X)
			{
				Bounds.LowerBounds.X = i.X;
			}
			if (i.Y < Bounds.LowerBounds.Y)
			{
				Bounds.LowerBounds.Y = i.Y;
			}
		}
	}
	return Bounds;
}

const FIntPoint UBasePart::GetLocation()
{
	return Location;
}

const TEnumAsByte<EPartRotation> UBasePart::GetRotation()
{
	return Rotation;
}

const TArray<FIntPoint> UBasePart::GetShape()
{
	return ActualShape;
}

float UBasePart::GetMass()
{
	
	//UE_LOG(LogTemp, Warning, TEXT("MASS = %f, Grr = %i"), Mass / GetDesiredShape().Num(), GetDesiredShape().Num());
	return Mass / GetDesiredShape().Num();
}

void UBasePart::DestroyPixel(FIntPoint RelativeLoc)
{
	ActualShape.Remove(RelativeLoc);
}

TMap<TEnumAsByte<EResourceType>, FIntPointArray> UBasePart::GetResourceTypes()
{
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ReturnValue;
	TArray<FIntPoint> IntPointArray;

	for (auto& i : ResourceTypes)
	{
		IntPointArray.Empty();
		for (auto& j : i.Value.IntPointArray)
		{
			IntPointArray.Add(UFunctionLibrary::RotateIntPoint(j, GetRotation()) + GetLocation());
		}
		UE_LOG(LogTemp, Warning, TEXT("i.key = %i"), i.Key.GetValue());
		ReturnValue.Add(i.Key, FIntPointArray(IntPointArray));
	}
	return ReturnValue;
}

void UBasePart::Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType)
{
	UE_LOG(LogTemp, Warning, TEXT("INIT"))
	Rotation = Rot;
	Location = Loc;
	PartGridComponent = PartGrid;
	ActualShape = GetDesiredShape();

	for (auto& i : GetResourceTypes())
	{
		bool SystemFound = false;
		for (auto& j : i.Value.IntPointArray)
		{	
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X + 1, j.Y)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)) != this)
			{
				UE_LOG(LogTemp, Warning, TEXT("x + 1 is valid"));
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y))->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X + 1, j.Y))
							{
								UE_LOG(LogTemp, Warning, TEXT("x + 1 system found"));
								for (auto& m : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y))->GetSystems())
								{
									UE_LOG(LogTemp, Warning, TEXT("systems on x + 1 types: %i"), m->GetType().GetValue());
								}

								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y))->GetSystemByType(i.Key));
								SystemFound = true;
							}
						}
					}
				}

			}
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X, j.Y + 1)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)) != this)
			{
				UE_LOG(LogTemp, Warning, TEXT("y + 1 is valid"));
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1))->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X, j.Y + 1))
							{
								UE_LOG(LogTemp, Warning, TEXT("y + 1 system Found"));
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1))->GetSystemByType(i.Key));
								SystemFound = true;
							}
						}
					}
				}
			}
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X, j.Y - 1)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)) != this)
			{
				UE_LOG(LogTemp, Warning, TEXT("y - 1 is valid"));
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1))->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X, j.Y - 1))
							{
								UE_LOG(LogTemp, Warning, TEXT("y - 1 system found"));
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1))->GetSystemByType(k.Key));
								SystemFound = true;
							}
						}
					}
				}
			}
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X - 1, j.Y)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)) != this)
			{
				UE_LOG(LogTemp, Warning, TEXT("x - 1 is valid"));
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X - 1, j.Y))->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X - 1, j.Y))
							{
								UE_LOG(LogTemp, Warning, TEXT("x - 1 system found"));
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X - 1, j.Y))->GetSystemByType(k.Key));
								SystemFound = true;
							}
						}
					}
				}
			}
		}
		if (SystemFound == false)
		{
			CreateNewSystem(i.Key);
		}
	}
}

void UBasePart::CreateNewSystem(TEnumAsByte<EResourceType> ResourceType)
{
	UE_LOG(LogTemp, Warning, TEXT("resource type = %i"), ResourceType.GetValue());
	//FString bruhvar = ToString(UEnum::GetValueAsString(ResourceType.GetValue()));
	//UE_LOG(LogTemp, Warning, TEXT("resource type but string = %s"), bruhvar);
	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>());
	NewSystem->SetType(ResourceType);
	NewSystem->AddPart(this);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddResourceSystem(NewSystem);
	AddToSystem(NewSystem);
}

TArray<UBaseResourceSystem*> UBasePart::GetSystems()
{
	return Systems;
}

UBaseResourceSystem* UBasePart::GetSystemByType(TEnumAsByte<EResourceType> Type)
{
	for (auto& i : GetSystems())
	{
		if (i->GetType() == Type)
		{
			return i;
		}
	}
	return nullptr;
}

const UPartGridComponent* UBasePart::GetPartGrid()
{
	return PartGridComponent;
}

void UBasePart::AddToSystem(UBaseResourceSystem* System)
{
	System->AddPart(this);
	if (IsValid(GetSystemByType(System->GetType())) && GetSystemByType(System->GetType()) != System)
	{
		UE_LOG(LogTemp, Warning, TEXT("IT IS VALid though@@@!"));
		//UE_LOG(LogTemp, Warning, TEXT("Merge systems"));
		//UE_LOG(LogTemp, Warning, TEXT("%i"), IsValid(GetSystemByType(System->GetType())));
		//Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		GetSystemByType(System->GetType())->MergeSystems(System);
	}
	else
	{
		Systems.Add(System);
		UE_LOG(LogTemp, Warning, TEXT("Why isn't it valid though???"));
		//UE_LOG(LogTemp, Warning, TEXT("ees not valid"));
		//UE_LOG(LogTemp, Warning, TEXT("%i"), IsValid(GetSystemByType(System->GetType())));
	}
}
