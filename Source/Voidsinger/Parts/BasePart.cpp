// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"
#include "BaseResourceSystem.h"
#include "Voidsinger/Parts/PartGridComponent.h"
#include "BaseThrusterPart.h"
#include "BaseResourceSystem.h"
#include "Engine/Engine.h"
#include "Voidsinger/Ships/BaseShip.h"


/*Initializer Funtions*\
\*--------------------*/

//Copy comment from .h -Mabel Suggestion
UBasePart::UBasePart()
{
	//Initalize All Variables
	//This should probably call the "InitializeVariables()" function -Mabel Suggestion
	Rotation = 0;
	Location = FIntPoint();
	TotalPartMass = 1;
	Cost = 1;
	DesiredShape = TSet<FIntPoint>();
	Bounds = FArrayBounds();
	ActualShape = TSet<FIntPoint>();
	bFunctional = true;
	bIsBeingDestroyed = false;

}

//Copy comment from .h -Mabel Suggestion
void UBasePart::InitializeVariables(FIntPoint Loc, float Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType, TSet<FIntPoint> Shape)
{
	//Initalize Variables
	Rotation = FMath::GridSnap<float>(Rot, 90);
	Location = Loc;
	PartGridComponent = PartGrid;
	ActualShape = GetDesiredShape().Intersect(Shape);
}

//Copy comment from .h -Mabel Suggestion
void UBasePart::InitializeFunctionality()
{

	//Would this function ever have a chance of being called again? If not, then this bool isn't needed. -Mabel Suggestion
	if (!bHasFunctionalityBeenInitialized)
	{
		//Bind to delegates
		Cast<AStarSystemGameMode>(GetWorld()->GetAuthGameMode())->OnVoidsongDelegate.AddDynamic(this, &UBasePart::OnDelegateCalled);
		Cast<ABaseShip>(GetOuter()->GetOuter())->OnActivatePartsDelegate.AddDynamic(this, &UBasePart::OnFireDelegateCalled);
		
		//Initialize Resource System
		ConnectToSystems();
	
		//Call BeginPlay for blueprints
		BeginPlay();

		bHasFunctionalityBeenInitialized = true;
	}
}

//Copy comment from .h -Mabel Suggestion
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

//Copy comment from .h -Mabel Suggestion
void UBasePart::DestroyPart()
{
	
	ConditionalBeginDestroy();
	bIsBeingDestroyed = true;
}

/*
* Function comments from the .h should be copied to the .cpp
*
* Why are activate and blueprint activate separate functions?
* You should(?) be able to make the interface function as bluepritn implementable
* - Liam Suggestion
*/
void UBasePart::Activate(float Duration)
{
	if(IsFunctional())
	BlueprintActivate(Duration);
}

/*--------Tick--------*\
\*--------------------*/

//Copy comment from .h -Mabel Suggestion
void UBasePart::Tick(float DeltaTime)
{

	if (!bIsBeingDestroyed)
	{
		//Call Blueprint Implementable Event
		OnTick(DeltaTime);
	}
		
}

//Copy comment from .h -Mabel Suggestion
bool UBasePart::IsTickable() const
{
	return (!IsTemplate(RF_ClassDefaultObject));
}

//Copy comment from .h -Mabel Suggestion
TStatId UBasePart::GetStatId() const
{
	return TStatId();
}

/*--Getter Functions--*\
\*--------------------*/

//Copy comment from .h -Mabel Suggestion
const TSet<FIntPoint> UBasePart::GetDesiredShape()
{
	return GetDesiredShape(Rotation);
}

//Copy comment from .h -Mabel Suggestion
const TSet<FIntPoint> UBasePart::GetDesiredShape(float Rot)
{
	TSet<FIntPoint> RotatedShape = TSet<FIntPoint>();
	for (FIntPoint PixelLoc : DesiredShape)
	{
		RotatedShape.Emplace(FVector2D(PixelLoc).GetRotated(FMath::GridSnap<float>(Rot, 90)).RoundToVector().IntPoint());
	}
	return RotatedShape;
}

//Copy comment from .h -Mabel Suggestion
const TSet<FIntPoint> UBasePart::GetShape()
{
	return ActualShape;
}

//Copy comment from .h -Mabel Suggestion
const FArrayBounds UBasePart::GetPartBounds()
{
	return GetPartBounds(Rotation);
}

//Copy comment from .h -Mabel Suggestion
const FArrayBounds UBasePart::GetPartBounds(float Rot)
{
	//Couldn't you just rotate the current Bounds variable with the parts current rotation
	//instead of recalculating the bounds? If you do want to recalculate
	//bounds this function should be renamed "RecalculateBounds" -Mabel Suggestion
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

//Copy comment from .h -Mabel Suggestion
const FIntPoint UBasePart::GetPartGridLocation()
{
	return Location;
}

//Copy comment from .h -Mabel Suggestion
const FVector UBasePart::GetPartWorldLocation()
{
	return GetShip()->GetActorTransform().TransformPosition(GetPartRelativeLocation());
}

//Copy comment from .h -Mabel Suggestion
const FVector UBasePart::GetPartRelativeLocation()
{
	return FVector((FVector2D(GetPartGridLocation()) * GetPartGrid()->GetPartGridScale()) - GetShip()->PhysicsComponent->GetCenterOfMass(), 0);
}

//Copy comment from .h -Mabel Suggestion
const float UBasePart::GetRelativeRotation()
{
	return Rotation;
}

//Copy comment from .h -Mabel Suggestion
const float UBasePart::GetWorldRotation()
{
	return Rotation + GetShip()->GetActorQuat().GetAngle();
}

//Copy comment from .h -Mabel Suggestion
float UBasePart::GetMass()
{
	//UE_LOG(LogTemp, Warning, TEXT("MASS = %f, Grr = %i"), TotalPartMass / GetDesiredShape().Num(), GetDesiredShape().Num());
	return TotalPartMass / GetDesiredShape().Num();
}

//Copy comment from .h -Mabel Suggestion
UPartGridComponent* UBasePart::GetPartGrid()
{
	return PartGridComponent;
}

//Copy comment from .h -Mabel Suggestion
ABaseShip* UBasePart::GetShip()
{
	return Cast<ABaseShip>(GetPartGrid()->GetOwner());
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
TArray<UBaseResourceSystem*>& UBasePart::GetSystems()
{
	return Systems;
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
UBaseResourceSystem* UBasePart::GetSystemByType(TEnumAsByte<EResourceType> Type)
{
	UBaseResourceSystem* FoundResourceSystem = nullptr;
	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
	for (auto& i : GetSystems())
	{
		//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResourceType"), true);
		//UE_LOG(LogTemp, Warning, TEXT("All systems types get system by type thing %s"), *EnumPtr->GetDisplayNameTextByValue(i->GetType().GetValue()).ToString());
		if (IsValid(i) && i->GetType() == Type)
		{
			FoundResourceSystem = i;
			//return i;
		}
	}
	//return nullptr;
	if (IsValid(FoundResourceSystem))
	{
		return FoundResourceSystem;
	}
	else
	{
		return nullptr;
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
TMap<TEnumAsByte<EResourceType>, FIntPointArray> UBasePart::GetResourceTypes()
{
	return ResourceTypes;
}

//Copy comment from .h -Mabel Suggestion
const int UBasePart::GetStrength()
{
	return Strength;
}

//Copy comment from .h -Mabel Suggestion
const int UBasePart::GetHeatResistance()
{
	return HeatResistance;
}

//Copy comment from .h -Mabel Suggestion
UMaterialInterface* UBasePart::GetPixelMaterial()
{
	return PixelMaterial;
}

/*Condtional  Checkers*\
\*--------------------*/

//Copy comment from .h -Mabel Suggestion
bool UBasePart::IsFunctional()
{

	//If you don't need it, delete it -Mabel Suggestion
	//return bFunctional;

	//Change the comment for this function to say that it uses percent functional instead of saying that it returns true if 
	//there is one pixel functional -Mabel Suggestion

	//If parts do not split into more parts when they are split, then this wouldn't really work. For example, if you cut
	//a part in half, this might still return true even if the part totally shouldn't be functional because there might still
	//be enough pixels for this to be true.
	if (GetShape().Num() >= PercentFunctional * GetDesiredShape().Num())
	{
		return !bIsBeingDestroyed;
	}
	else
	{
		return false;
	}
}

//Copy comment from .h -Mabel Suggestion
bool UBasePart::IsPixelFunctional(FIntPoint Loc)
{
	//Couldn't you just check if Loc is in ActualShape? (as well as IsFunctional() of course, assuming parts split into more parts
	//when they are split) -Mabel Suggestion
	return IsFunctional() && TGridMap<bool>(TSet<FIntPoint>(GetShape()), false).PointsConnected(Loc - GetPartGridLocation(), FIntPoint(0, 0), AlwaysConnect<bool>);
}


/*---Misc. Functions--*\
\*--------------------*/

//Copy comment from .h -Mabel Suggestion
void UBasePart::DestroyPixel(FIntPoint RelativeLoc, bool bCallDamagedEvents)
{
	if (!ActualShape.Contains(RelativeLoc))
	{
		UE_LOG(LogTemp, Error, TEXT("Destroy part pixel location not valid : % s"), *RelativeLoc.ToString());
	}
	ActualShape.Remove(RelativeLoc);
	if (bCallDamagedEvents)
	{
		OnDamaged();
		GetShip()->OnDamaged.Broadcast(this);
	}

	TArray<UBaseResourceSystem*> CurrentSystems = Systems;
	if (IsFunctional())
	{
		//Don't auto, also name iterator better (I yell at myself) -Mabel Suggestion (-Liam suggestion so I see this and don't forget)
		for (auto& i : CurrentSystems)
		{
			UE_LOG(LogTemp, Warning, TEXT("systems on destroy pixel %s"), *i->GetFName().ToString());
			i->RemovePixel(RelativeLoc + GetPartGridLocation());
		}
	}
	else
	{
		if (bCallDamagedEvents)
		{
			OnCriticallyDamaged();
		}

		//Don't auto, also name iterator better (I yell at myself) -Mabel Suggestion (-Liam suggestion so I see this and don't forget)
		for (auto& i : CurrentSystems)
		{
			i->RemovePart(this);
			i->RemovePixel(RelativeLoc + GetPartGridLocation());
		}
	}
	if (ActualShape.Num() <= 0 || !IsValid(GetShip()))
	{
		OnDestroyed();
		DestroyPart();
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBasePart::ConnectToSystems()
{
	//Basic description: for each pixel on this part that has a resource type, check around that location for another part that has a pixel with that resource type next to the
	// pixel currently being checked. If there is no adjacent resource system create a new one.

	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
	//This needs to be called for each resource type so a resource system for each type is created.
	for (auto& i : GetResourceTypes())
	{

		//System found will be useful later to determine if the part should be added to an existing or the part should make a new system.
		bool SystemFound = false;

		//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
		//For each pixel location that has this resource type applied to it
		for (auto& j : i.Value.IntPointArray)
		{
			FIntPoint RelativePartGridLocation = FVector2D(j).GetRotated(GetRelativeRotation()).IntPoint() + GetPartGridLocation();

			if (
				PartGridComponent->GetPartGrid().Contains(RelativePartGridLocation)
				&&
				PartGridComponent->GetPartGrid().Find(RelativePartGridLocation)->Part->GetResourceTypes().Contains(i.Key)
				)
			{
				for (FIntPoint EachConnectionPoint : PartGridComponent->GetPartGrid().Find(RelativePartGridLocation)->Part->GetResourceTypes().Find(i.Key)->GetIntPointArray())
				{
					//UE_LOG(LogTemp, Warning, TEXT("this part location %s"), *GetPartGridLocation().ToString());
					//UE_LOG(LogTemp, Warning, TEXT("Other part location %s"), *PartGridComponent->GetPartGrid().Find(RelativePartGridLocation)->Part->GetPartGridLocation().ToString());
					//UE_LOG(LogTemp, Warning, TEXT("connection point relative to other part %s"), *EachConnectionPoint.ToString());

					FIntPoint ConnectionPointRelativeToPartGrid = FVector2D(EachConnectionPoint).GetRotated(PartGridComponent->GetPartGrid().Find(RelativePartGridLocation)->Part->GetRelativeRotation()).IntPoint() + PartGridComponent->GetPartGrid().Find(RelativePartGridLocation)->Part->GetPartGridLocation();
					
					//UE_LOG(LogTemp, Warning, TEXT("Connection point relative to part grid %s"), *ConnectionPointRelativeToPartGrid.ToString());

					FIntPoint ConnectionPointRelativeToThisPart = FVector2D(ConnectionPointRelativeToPartGrid - GetPartGridLocation()).GetRotated(-GetRelativeRotation()).IntPoint();

					//UE_LOG(LogTemp, Warning, TEXT("Connection point relative to this part %s"), *ConnectionPointRelativeToThisPart.ToString());

					if (GetDesiredShape().Contains(ConnectionPointRelativeToThisPart))
					{
						if (PartGridComponent->GetPartGrid().Find(RelativePartGridLocation)->Part != this)
						{
							AddToSystem(PartGridComponent->GetPartGrid().FindRef(FVector2D(j).GetRotated(GetRelativeRotation()).IntPoint() + GetPartGridLocation()).Part->GetSystemByType(i.Key));

							//A system was found!
							SystemFound = true;
						}
						else
						{
							//If the part found at the location is the same as this part, then resource connection points were set up incorrectly. Print an error message.
							UE_LOG(LogTemp, Error, TEXT("The resource connection points of %s are wrong! Fix them immediately or resources will not work correctly!"), *GetClass()->GetDisplayNameText().ToString());
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
}

//Function comments from the .h should be identical to the one in the .cpp - Liam Suggestion
//Create a new resource system
void UBasePart::CreateNewSystem(TEnumAsByte<EResourceType> ResourceType)
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResourceType"), true);
	//UE_LOG(LogTemp, Warning, TEXT("Create new system type %s"), *EnumPtr->GetDisplayNameTextByValue(ResourceType.GetValue()).ToString());

	//Make the new system, make sure it's the right type, and add the system to the list of systems on the player character
	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>());
	NewSystem->SetType(ResourceType);
	Cast<ABaseShip>(Cast<UActorComponent>(GetOuter())->GetOwner())->AddResourceSystem(NewSystem);

	//Call this to add the part to the system and double check that merges don't need to happen (see AddToSystem)
	AddToSystem(NewSystem);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBasePart::AddToSystem(UBaseResourceSystem* System)
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EResourceType"), true);
	//UE_LOG(LogTemp, Warning, TEXT("Add to system type %s"), *EnumPtr->GetDisplayNameTextByValue(System->GetType().GetValue()).ToString());

	if (!Systems.Contains(System))
	{
		//Add the part to the system
		//System->AddPart(this);

		//If there is already a system of this resource type on this part, then merge System with that system
		if ((IsValid(GetSystemByType(System->GetType()))) && (GetSystemByType(System->GetType()) != System))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Merge systems called"));
			GetSystemByType(System->GetType())->MergeSystems(System);
			Systems.Add(GetSystemByType(System->GetType()));
			//System->MergeSystems(GetSystemByType(System->GetType()));
		}

		//else just add it to the list of systems on this part.
		else
		{
			System->AddPart(this);
			//UE_LOG(LogTemp, Warning, TEXT("Merge systems not called"));
			//UE_LOG(LogTemp, Warning, TEXT("------------------------"));
			Systems.Add(System);
		}
	}
}

/*
* Function comments from the .h should be copied to the .cpp
* 
* Consider coverting avalable voisongs to a stuct containg avalable Verbs, Nouns, and Factions or just Nouns and Factions
* 
* Logic for weather or not a delegate is cabale of being called should be done before it is brodcast.
* In otherwords AvalableVoidsongs should be handeled in UBaseShip::DecideVoidsongsPlayed()
* 
* Confusing parameter names.
* Factions for what?
* Nounclasses for what?
* Inconsitant names. To fix either: rename Factions to FactionNames or FactionTypes, or rename NounClasses to Nouns
* 
* Shouldn't NounClasses have the type: const TArray<TSubclassOf<UBasePart>>&
* - Liam Suggestion
*/
void UBasePart::OnDelegateCalled(const TArray<TEnumAsByte<EFactions>>& Factions, const TArray<TSubclassOf<UObject>>& NounClasses, const TArray<UBaseVoidsong*>& AvailableVoidsongs, float Duration)
{
	/*for (auto& i : NounClasses)
	{
		UE_LOG(LogTemp, Warning, TEXT("Noun classes %s"), *i.Get()->GetDisplayNameText().ToString())
	}

	UE_LOG(LogTemp, Warning, TEXT("part class %s"), *this->GetClass()->GetDisplayNameText().ToString())*/

	// I know this is gross but I was trying to get the game playable when I made this. I realized that parts that you didn't have the Voidsong for would activate if you
	// didn't play any nouns even though they shouldn't.

	
	// Variable sould be global and updated on AddNewVoidsong()	 
	TArray<TEnumAsByte<EFactions>> AvailableFactions;
	TArray<TSubclassOf<UObject>> AvailableNouns;

	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
	for (auto& i : AvailableVoidsongs)
	{
		if (IsValid(Cast<UBaseWhoVoidsong>(i)))
		{
			AvailableFactions.Emplace(Cast<UBaseWhoVoidsong>(i)->Faction);
		}
		else if (IsValid(Cast<UBaseNounVoidsong>(i)))
		{
			//UE_LOG(LogTemp, Warning, TEXT("noun voidsong %s"), *i->GetFName().ToString())
			AvailableNouns.Emplace(Cast<UBaseNounVoidsong>(i)->Noun);
		}
	}

	bool NounsCheck = false;
	if (NounClasses.IsEmpty())
	{
		for (TSubclassOf<UObject> AvailableNounVoidsong : AvailableNouns)
		{

			if (GetClass()->IsChildOf(AvailableNounVoidsong))
			{
				NounsCheck = true;
				break;
			}
		}
	}
	else
	{
		for (TSubclassOf<UObject> NounPlayed : NounClasses)
		{
			if (GetClass()->IsChildOf(NounPlayed))
			{
				NounsCheck = true;
				break;
			}
		}
	}

	//Long chunk of logic should be commented or made legible - Liam Suggestion
	if (((Factions.IsEmpty() && AvailableFactions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) != Factions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) && NounsCheck)
	{
		if (this->Implements<UActivateInterface>())
		{
			Cast<IActivateInterface>(this)->Activate(Duration);
		}
	}
}

void UBasePart::OnFireDelegateCalled(const TArray<TSubclassOf<UObject>>& NounClasses, float Duration)
{
	if (NounClasses.Contains(GetClass()))
	{
		Cast<IActivateInterface>(this)->Activate(Duration);
	}
}

int32 UBasePart::GetCost()
{
	return Cost;
}
