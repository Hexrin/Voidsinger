// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"
#include "BaseResourceSystem.h"
#include "Engine/Engine.h"


/*Initializer Funtions*\
\*--------------------*/
UBasePart::UBasePart()
{
	//Initalize All Variables
	Rotation = 0;
	Location = FIntPoint();
	TotalPartMass = 1;
	Cost = 1;
	DesiredShape = TSet<FIntPoint>();
	Bounds = FArrayBounds();
	//RotatedShape = TArray<FIntPoint>();
	ActualShape = TArray<FIntPoint>();
	bFunctional = true;
	bIsPoopBeingDestroyed = false;
}

void UBasePart::InitializeVariables(FIntPoint Loc, float Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType)
{
	//Initalize Variables
	Rotation = FMath::GridSnap<float>(Rot, 90);
	Location = Loc;
	PartGridComponent = PartGrid;
	ActualShape = GetDesiredShape();

	
}

void UBasePart::InitializeFunctionality()
{
	//Initialize Resource System

	//Basic description: for each pixel on this part that has a resource type, check around that location for another part that has a pixel with that resource type next to the
	// pixel currently being checked. If there is no adjacent resource system create a new one.

	//This needs to be called for each resource type so a resource system for each type is created.
	for (auto& i : GetResourceTypes())
	{
		//System found will be useful later to determine if the part should be added to an existing or the part should make a new system.
		bool SystemFound = false;

		//For each pixel location that has this resource type applied to it
		for (auto& j : i.Value.IntPointArray)
		{
			//Check the X + 1 location for a part on the part grid
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X + 1, j.Y)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part != this)
			{
				//For each resource type location on the adjacent part
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part->GetResourceTypes())
				{
					//check if the type of the resource system is the same as the type of of the adjecent part's resource system
					if (k.Key == i.Key)
					{
						//For each pixel location on the adjacent part that has this resource type
						for (auto& l : k.Value.IntPointArray)
						{
							//Check if the adjacent part's pixel is actually adjacent to the pixel in question on this part
							if (l == FIntPoint(j.X + 1, j.Y))
							{
								//if all the above checks succeed, then add this part to the resource system that it found it was adjacent to.
								//Get system by type works because a part can only ever be part of 1 system with a given type, with various different reasons and logic to get
								// that result. 
								//(although... i may need to think about that)
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part->GetSystemByType(i.Key));

								//A system was found!
								SystemFound = true;
							}
						}
					}
				}

			}

			//Do everything done for X + 1 for Y + 1
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X, j.Y + 1)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part != this)
			{
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1)).Part->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X, j.Y + 1))
							{
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y + 1)).Part->GetSystemByType(i.Key));
								SystemFound = true;
							}
						}
					}
				}
			}

			//Do everything done for X + 1 for Y -1
			if (PartGridComponent->GetPartGrid().Contains(FIntPoint(j.X, j.Y - 1)) && PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X + 1, j.Y)).Part != this)
			{
				for (auto& k : PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1)).Part->GetResourceTypes())
				{
					if (k.Key == i.Key)
					{
						for (auto& l : k.Value.IntPointArray)
						{
							if (l == FIntPoint(j.X, j.Y - 1))
							{
								AddToSystem(PartGridComponent->GetPartGrid().FindRef(FIntPoint(j.X, j.Y - 1)).Part->GetSystemByType(k.Key));
								SystemFound = true;
							}
						}
					}
				}
			}

			//Do everything done for X + 1 for X - 1
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

		//If the code gets to this point without finding a system, then it will create a new resource system
		if (SystemFound == false)
		{
			CreateNewSystem(i.Key);
		}
	}

	//Call BeginPlay for blueprints
	BeginPlay();
}

UWorld* UBasePart::GetWorld() const
{
	if (GetOuter() == nullptr)
	{
		return nullptr;
	}

	// Special case for behavior tree nodes in the editor
	if (Cast<UPackage>(GetOuter()) != nullptr)
	{
		// GetOuter should return a UPackage and its Outer is a UWorld
		return Cast<UWorld>(GetOuter()->GetOuter());
	}

	// In all other cases...
	return GetOuter()->GetWorld();
}




void UBasePart::DestroyPart()
{
	bIsPoopBeingDestroyed = true/*ConditionalBeginDestroy()*/;
}

/*--------Tick--------*\
\*--------------------*/
void UBasePart::Tick(float DeltaTime)
{

	if (!bIsPoopBeingDestroyed)
	{
		//Call Blueprint Implementable Event
		OnTick(DeltaTime);
	}
		
}

bool UBasePart::IsTickable() const
{
	return (!IsTemplate(RF_ClassDefaultObject));
}

TStatId UBasePart::GetStatId() const
{
	return TStatId();
}




/*--Getter Functions--*\
\*--------------------*/
const TArray<FIntPoint> UBasePart::GetDesiredShape()
{
	
	return GetDesiredShape(Rotation);
}
const TArray<FIntPoint> UBasePart::GetDesiredShape(float Rot)
{
	TArray<FIntPoint> RotatedShape = TArray<FIntPoint>();
	for (FIntPoint PixelLoc : DesiredShape)
	{
		RotatedShape.Emplace(FVector2D(PixelLoc).GetRotated(FMath::GridSnap<float>(Rot, 90)).RoundToVector().IntPoint());
	}
	return RotatedShape;
}

const TArray<FIntPoint> UBasePart::GetShape()
{
	return ActualShape;
}

const FArrayBounds UBasePart::GetPartBounds()
{
	return GetPartBounds(Rotation);
}
const FArrayBounds UBasePart::GetPartBounds(float Rot)
{
	if ((Bounds.LowerBounds == FArrayBounds().LowerBounds && Bounds.UpperBounds == FArrayBounds().UpperBounds) || this == this->GetClass()->GetDefaultObject())
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

const FIntPoint UBasePart::GetPartGridLocation()
{
	return Location;
}

const FVector UBasePart::GetPartWorldLocation()
{
	return GetPartRelativeLocation() + GetShip()->GetActorLocation();
}

const FVector UBasePart::GetPartRelativeLocation()
{
	return FVector((FVector2D(GetPartGridLocation()) * GetPartGrid()->GetPartGridScale()) - GetPartGrid()->GetCenterOfMass(), 0);
}

const float UBasePart::GetRotation()
{
	return Rotation;
}

float UBasePart::GetMass()
{
	//UE_LOG(LogTemp, Warning, TEXT("MASS = %f, Grr = %i"), TotalPartMass / GetDesiredShape().Num(), GetDesiredShape().Num());
	return TotalPartMass / GetDesiredShape().Num();
}

UPartGridComponent* UBasePart::GetPartGrid()
{
	return PartGridComponent;
}

const ABaseShip* UBasePart::GetShip()
{
	return Cast<ABaseShip>(GetPartGrid()->GetOwner());
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

TMap<TEnumAsByte<EResourceType>, FIntPointArray> UBasePart::GetResourceTypes()
{
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ReturnValue;
	TArray<FIntPoint> IntPointArray;

	for (auto& i : ResourceTypes)
	{
		IntPointArray.Empty();
		for (auto& j : i.Value.IntPointArray)
		{
			FIntPoint AdjLocation = FVector2D(j).GetRotated(GetRotation()).RoundToVector().IntPoint() + GetPartGridLocation();
			if (GetShape().Contains(AdjLocation))
			{
				IntPointArray.Emplace(AdjLocation);
			}
		}
		if (!IntPointArray.IsEmpty())
		{
			ReturnValue.Emplace(i.Key, FIntPointArray(IntPointArray));
		}
	}
	return ReturnValue;
}

int UBasePart::GetStrength()
{
	return Strength;
}

int UBasePart::GetHeatResistance()
{
	return HeatResistance;
}

UMaterialInterface* UBasePart::GetPixelMaterial()
{
	return PixelMaterial;
}




/*Condtional  Checkers*\
\*--------------------*/
bool UBasePart::IsFunctional()
{
	return bFunctional;
}

bool UBasePart::IsPixelFunctional(FIntPoint Loc)
{
	return false;
}




/*---Misc. Functions--*\
\*--------------------*/
void UBasePart::DestroyPixel(FIntPoint RelativeLoc)
{
	ActualShape.Remove(RelativeLoc);
	OnDamaged();

	if (IsFunctional())
	{
		for (auto& i : Systems)
		{
			i->RemovePixel(RelativeLoc);
		}
	}
	else
	{
		OnCriticallyDamaged();
		for (auto& i : Systems)
		{
			i->RemovePart(this);
			i->RemovePixel(RelativeLoc);
		}
	}
	if (ActualShape.Num() <= 0)
	{
		OnDestroyed();
		DestroyPart();
	}
}

//Create a new resource system
void UBasePart::CreateNewSystem(TEnumAsByte<EResourceType> ResourceType)
{

	//Make the new system, make sure it's the right type, and add the system to the list of systems on the player character
	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>());
	NewSystem->SetType(ResourceType);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddResourceSystem(NewSystem);

	//Call this to add the part to the system and double check that merges don't need to happen (see AddToSystem)
	AddToSystem(NewSystem);
}

void UBasePart::AddToSystem(UBaseResourceSystem* System)
{
	//Add the part to the system
	System->AddPart(this);

	//If there is already a system of this resource type on this part, then merge System with that system
	if (IsValid(GetSystemByType(System->GetType())) && GetSystemByType(System->GetType()) != System)
	{
		GetSystemByType(System->GetType())->MergeSystems(System);
	}

	//else just add it to the list of systems on this part.
	else
	{
		Systems.Add(System);
	}
}
