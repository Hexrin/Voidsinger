// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"
#include "BaseResourceSystem.h"
#include "Engine/Engine.h"

UBasePart::UBasePart()
{
	//Initalize All Variables
	Rotation = EPartRotation::Degrees0;
	Location = FIntPoint();
	Mass = 1;
	Cost = 1;
	DesiredShape = TArray<FIntPoint>();
	Bounds = FArrayBounds();
	RotatedShape = TArray<FIntPoint>();
	ActualShape = TArray<FIntPoint>();
}

//Tick
void UBasePart::Tick(float DeltaTime)
{
	//Call Blueprint Implementable Event
	EventTick(DeltaTime);
}
bool UBasePart::IsTickable() const
{
	return true;
}
TStatId UBasePart::GetStatId() const
{
	return TStatId();
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

	if (IsFunctional())
	{
		for (auto& i : Systems)
		{
			i->RemovePixel(RelativeLoc);
		}
	}
	else
	{
		for (auto& i : Systems)
		{
			i->RemovePart(this);
			i->RemovePixel(RelativeLoc);
		}
	}
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
			if (GetShape().Contains(UFunctionLibrary::RotateIntPoint(j, GetRotation()) + GetLocation()))
			{
				IntPointArray.Add(UFunctionLibrary::RotateIntPoint(j, GetRotation()) + GetLocation());
			}
		}
		if (!IntPointArray.IsEmpty())
		{
			ReturnValue.Add(i.Key, FIntPointArray(IntPointArray));
		}
	}
	return ReturnValue;
}

void UBasePart::Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType)
{
	//UE_LOG(LogTemp, Warning, TEXT("INIT"))
	Rotation = Rot;
	Location = Loc;
	PartGridComponent = PartGrid;
	ActualShape = GetDesiredShape();

	for (auto& i : GetResourceTypes())
	{
		bool SystemFound = false;
		for (auto& j : i.Value.IntPointArray)
		{	
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X + 1, j.Y)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part != this)
			{
				//UE_LOG(LogTemp, Warning, TEXT("x + 1 is valid"));
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X + 1, j.Y))
							{
								//UE_LOG(LogTemp, Warning, TEXT("x + 1 system found"));
								for (auto& m : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part->GetSystems())
								{
									//UE_LOG(LogTemp, Warning, TEXT("systems on x + 1 types: %i"), m->GetType().GetValue());
								}

								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part->GetSystemByType(i.Key));
								SystemFound = true;
							}
						}
					}
				}

			}
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X, j.Y + 1)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part != this)
			{
				//UE_LOG(LogTemp, Warning, TEXT("y + 1 is valid"));
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1)).Part->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X, j.Y + 1))
							{
								//UE_LOG(LogTemp, Warning, TEXT("y + 1 system Found"));
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1)).Part->GetSystemByType(i.Key));
								SystemFound = true;
							}
						}
					}
				}
			}
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X, j.Y - 1)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part != this)
			{
				//UE_LOG(LogTemp, Warning, TEXT("y - 1 is valid"));
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1)).Part->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X, j.Y - 1))
							{
								//UE_LOG(LogTemp, Warning, TEXT("y - 1 system found"));
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1)).Part->GetSystemByType(k.Key));
								SystemFound = true;
							}
						}
					}
				}
			}
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X - 1, j.Y)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part != this)
			{
				//UE_LOG(LogTemp, Warning, TEXT("x - 1 is valid"));
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X - 1, j.Y)).Part->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X - 1, j.Y))
							{
								//UE_LOG(LogTemp, Warning, TEXT("x - 1 system found"));
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X - 1, j.Y)).Part->GetSystemByType(k.Key));
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
		GetSystemByType(System->GetType())->MergeSystems(System);
	}
	else
	{
		Systems.Add(System);
	}
}

//This should only return false if NO pixel is functional
bool UBasePart::IsFunctional()
{
	return Functional;
}

bool UBasePart::IsPixelFunctional(FIntPoint Loc)
{
	return false;
}
