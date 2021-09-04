// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"
#include "BaseResourceSystem.h"

UBasePart::UBasePart()
{
}

const TArray<FIntPoint> UBasePart::GetDesiredShape()
{
	TArray<FIntPoint> TempShape = TArray<FIntPoint>();
	for (int i = 0; i < _DesiredShape.Num(); i++)
	{
		switch (Rotation)
		{
		case Degrees0:
			TempShape.Add(_DesiredShape[i]);
			break;
		case Degrees90:
			TempShape.Add(FIntPoint(-DesiredShape[i].Y, DesiredShape[i].X));
			break;
		case Degrees180:
			TempShape.Add(DesiredShape[i] * -1);
			break;
		case Degrees270:
			TempShape.Add(FIntPoint(DesiredShape[i].Y, -DesiredShape[i].X));
			break;
		default:
			break;
		}
	}
	return TempShape;
}

const TArray<FIntPoint> UBasePart::GetDesiredShape(TEnumAsByte<EPartRotation> Rot)
{
	TArray<FIntPoint> TempShape = TArray<FIntPoint>();
	for (int i = 0; i < _DesiredShape.Num(); i++)
	{
		switch (Rot)
		{
		case Degrees0:
			TempShape.Add(_DesiredShape[i]);
			break;
		case Degrees90:
			TempShape.Add(FIntPoint(-_DesiredShape[i].Y, _DesiredShape[i].X));
			break;
		case Degrees180:
			TempShape.Add(_DesiredShape[i] * -1);
			break;
		case Degrees270:
			TempShape.Add(FIntPoint(_DesiredShape[i].Y, -_DesiredShape[i].X));
			break;
		default:
			break;
		}
	}
	return TempShape;
}

const FArrayBounds UBasePart::GetShapeBounds()
{
	FArrayBounds Bounds = FArrayBounds();

	for (int i = 0; i < GetDesiredShape().Num(); i++)
	{
		if (GetDesiredShape()[i].X > Bounds.UpperBounds.X)
		{
			Bounds.UpperBounds.X = GetDesiredShape()[i].X;
		}
		if (GetDesiredShape()[i].Y > Bounds.UpperBounds.Y)
		{
			Bounds.UpperBounds.Y = GetDesiredShape()[i].Y;
		}

		if (GetDesiredShape()[i].X < Bounds.LowerBounds.X)
		{
			Bounds.LowerBounds.X = GetDesiredShape()[i].X;
		}
		if (GetDesiredShape()[i].Y < Bounds.LowerBounds.Y)
		{
			Bounds.LowerBounds.Y = GetDesiredShape()[i].Y;
		}
	}

	return Bounds;
}

const FArrayBounds UBasePart::GetShapeBounds(TEnumAsByte<EPartRotation> Rot)
{
	FArrayBounds Bounds = FArrayBounds();

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
	TArray<FIntPoint> ActualShape = GetDesiredShape();
	for (int i = 0; i < DestroyedPixels.Num(); i++)
	{
		ActualShape.Remove(DestroyedPixels[i]);
	}
	return ActualShape;
}

float UBasePart::GetMass()
{
	return _Mass/ _DesiredShape.Num();
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

void UBasePart::Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGridComp)
{
	Rotation = Rot;
	Location = Loc;
	PartGridComponent = PartGridComp;
	_Mass = Mass;
	_Cost = Cost;
	_DesiredShape = DesiredShape;

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
