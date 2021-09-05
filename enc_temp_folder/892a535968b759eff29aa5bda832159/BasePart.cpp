// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"
#include "BaseResourceSystem.h"

UBasePart::UBasePart()
{
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
		for (int i = 0; i < GetDesiredShape(Rot).Num(); i++)
		{
			if (GetDesiredShape(Rot)[i].X > Bounds.UpperBounds.X)
			{
				Bounds.UpperBounds.X = GetDesiredShape(Rot)[i].X;
			}
			if (GetDesiredShape(Rot)[i].Y > Bounds.UpperBounds.Y)
			{
				Bounds.UpperBounds.Y = GetDesiredShape(Rot)[i].Y;
			}

			if (GetDesiredShape(Rot)[i].X < Bounds.LowerBounds.X)
			{
				Bounds.LowerBounds.X = GetDesiredShape(Rot)[i].X;
			}
			if (GetDesiredShape(Rot)[i].Y < Bounds.LowerBounds.Y)
			{
				Bounds.LowerBounds.Y = GetDesiredShape(Rot)[i].Y;
			}
		}
	}
	return Bounds;
}

const FIntPoint UBasePart::GetLocation()
{
	return FIntPoint();
}

const TEnumAsByte<EPartRotation> UBasePart::GetRotation()
{
	return TEnumAsByte<EPartRotation>();
}

const TArray<FIntPoint> UBasePart::GetShape()
{
	return ActualShape;
}

float UBasePart::GetMass()
{
	
	UE_LOG(LogTemp, Warning, TEXT("MASS = %f, Grr = %i"), Mass / GetDesiredShape().Num(), GetDesiredShape().Num());
	return Mass / GetDesiredShape().Num();
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
			IntPointArray.Add(UFunctionLibrary::RotateIntPoint(j, GetRotation()));
		}
		
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
			if (IsValid(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y))) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)) != this)
			{
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y))->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X + 1, j.Y))
							{
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y))->GetSystemByType(k.Key));
								SystemFound = true;
							}
						}
					}
				}

			}
			if (IsValid(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1))) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)) != this)
			{
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1))->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X + 1, j.Y + 1))
							{
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1))->GetSystemByType(k.Key));
								SystemFound = true;
							}
						}
					}
				}
			}
			if (IsValid(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1))) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)) != this)
			{
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1))->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X, j.Y + 1))
							{
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1))->GetSystemByType(k.Key));
								SystemFound = true;
							}
						}
					}
				}
			}
			if (IsValid(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X - 1, j.Y))) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)) != this)
			{
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X - 1, j.Y))->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X + 1, j.Y + 1))
							{
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
	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>());
	NewSystem->SetType(ResourceType);
	NewSystem->AddPart(this);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->ResourceSystems.Add(NewSystem);
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

void UBasePart::AddToSystem(UBaseResourceSystem* System)
{
	System->AddPart(this);
	Systems.Add(System);
}
