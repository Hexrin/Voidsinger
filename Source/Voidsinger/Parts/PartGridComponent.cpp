// Fill out your copyright notice in the Description page of Project Settings.


#include "PartGridComponent.h"
#include "BasePart.h"

// Sets default values for this component's properties
UPartGridComponent::UPartGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Initiate Variables
	GridBounds.UpperBounds = FIntPoint();
	GridBounds.LowerBounds = FIntPoint();

	GridSize = FIntPoint(50);
	
	PartGrid = TMap<FIntPoint, UBasePart*>();
	MeshGrid = TMap<FIntPoint, UActorComponent*>();

	if (!GridScale)
	{
		GridScale = 1;
	}
	// ...
}


// Called when the game starts
void UPartGridComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPartGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}


//Adds a compleate part to the part grid
bool UPartGridComponent::AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, TEnumAsByte<EPartRotation> Rotation, bool bAlwaysPlace)
{
	//Create Part
	UBasePart* Part = NewObject<UBasePart>(this, PartType);
	Part->Init(Location, Rotation, this, PartType);

	//Initalize Variables
	TArray<FIntPoint> DesiredShape = Part->GetDesiredShape(Rotation);
	FArrayBounds PartBounds = Part->GetShapeBounds(Rotation);
	
	//Detect if placement is in valid position
	if (GridSize.X >= Location.X + PartBounds.UpperBounds.X && -GridSize.X <= Location.X + PartBounds.LowerBounds.X
		&&
		GridSize.Y >= Location.Y + PartBounds.UpperBounds.Y && -GridSize.Y <= Location.Y + PartBounds.LowerBounds.Y
		&&
		(bAlwaysPlace || CanShapeFit(Location, DesiredShape)))
	{		

		//Update GridBounds
		if (Location.X + PartBounds.LowerBounds.X < GridBounds.LowerBounds.X)
		{
			GridBounds.LowerBounds.X = Location.X + PartBounds.LowerBounds.X;
		}
		if (Location.Y + PartBounds.LowerBounds.Y < GridBounds.LowerBounds.Y)
		{
			GridBounds.LowerBounds.Y = Location.Y + PartBounds.LowerBounds.Y;
		}
		if (Location.X + PartBounds.UpperBounds.X > GridBounds.UpperBounds.X)
		{
			GridBounds.UpperBounds.X = Location.X + PartBounds.UpperBounds.X;
		}
		if (Location.Y + PartBounds.UpperBounds.Y > GridBounds.UpperBounds.Y)
		{
			GridBounds.UpperBounds.Y = Location.Y + PartBounds.UpperBounds.Y;
		}

		//Iterate though desiered shape and add to part grid
		for (int i = 0; i < DesiredShape.Num(); i++)
		{
			//Remove Overlaping Parts
			if (bAlwaysPlace)
			{
				RemovePart(FIntPoint(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y));
			}

			PartGrid.Add(FIntPoint(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y), Part);

			//Create Mesh
			class UActorComponent* NewPlane = GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform(FRotator(), FVector(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y, 0) * GridScale, FVector(GridScale)), false);
			Cast<UStaticMeshComponent>(NewPlane)->SetStaticMesh(PartType.GetDefaultObject()->PixelMesh);
			MeshGrid.Emplace(FIntPoint(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y), NewPlane);
		}
		return true;
	}
	return false;
}


//Remove an entire part. Returns True if a part was destroyed
bool UPartGridComponent::RemovePart(FIntPoint Location)
{
	//Check if location is valid
	if (IsValid(PartGrid.FindRef(Location)))
	{
		//Intialize Variables
		class UBasePart* PartToRemove = PartGrid.FindRef(Location);
		FIntPoint PartLoc = PartToRemove->GetLocation();

		//Iterate though the shape of PartToRemove and remove them from the part grid
		for (FIntPoint Loc : PartToRemove->GetShape())
		{
			DestroyPixel(Loc);
		}
		return true;
	}
	else
	{
		return false;
	}
}

//Remove a single Pixel from the PartGrid. Returns true if a pixel was removed
bool UPartGridComponent::DestroyPixel(FIntPoint Location)
{
	class UBasePart* DamagedPart = NewObject<UBasePart>(this);
	return DestroyPixel(Location, DamagedPart);
}
bool UPartGridComponent::DestroyPixel(FIntPoint Location, class UBasePart*& DamagedPart)
{
	//Check if pixel is valid
	if (IsValid(PartGrid.FindRef(Location)))
	{
		//Remove from grid
		DamagedPart = PartGrid.FindRef(Location);
		DamagedPart->DestroyPixel(Location - DamagedPart->GetLocation());
		PartGrid.Remove(Location);

		//Destroy Mesh
		MeshGrid.FindRef(Location)->DestroyComponent();
		MeshGrid.Remove(Location);
		return true;
	}
	else
	{
		return false;
	}
}


void UPartGridComponent::BuildShip(TArray<FSavePartInfo> Parts)
{
	TArray<FIntPoint> AllParts;

	PartGrid.GenerateKeyArray(AllParts);

	for (auto& i : AllParts)
	{
		RemovePart(i);
	}
	for (int i = 0; i < Parts.Num(); i++)
	{
		AddPart(Parts[i].PartClass, Parts[i].PartLocation, Parts[i].PartRotation, false);
	}
}

void UPartGridComponent::SaveShip(FString ShipName)
{
	
	TArray<UBasePart*> Parts;

	PartGrid.GenerateValueArray(Parts);
	
	USaveGame* SaveGameInstance = UGameplayStatics::CreateSaveGameObject(USaveShip::StaticClass());

	for (int i = 0; i < Parts.Num(); i++)
	{
		Cast<USaveShip>(SaveGameInstance)->SavedShip.Add(FSavePartInfo(Parts[i]->GetClass(), Parts[i]->GetLocation(), Parts[i]->GetRotation()));
	}
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, ShipName, 0);

}

void UPartGridComponent::LoadSavedShip(FString ShipName)
{
	USaveGame* SaveGameInstance = UGameplayStatics::LoadGameFromSlot(ShipName, 0);
	BuildShip(Cast<USaveShip>(SaveGameInstance)->SavedShip);
}


//Gets the center of mass of the PartGrid
const FVector2D UPartGridComponent::GetCenterOfMass()
{
	//Initalize Variables
	FVector2D Center = FVector2D(0,0);
	float Mass = GetMass();
	//UE_LOG(LogTemp, Warning, TEXT("PRE cofmass?? x=%f, y=%f, Mass=%f"), Center.X, Center.Y, Mass);

	//Iterate though Pixels and adjust center of mass
	for (auto& Elem : PartGrid)
	{
		Center += FVector2D(Elem.Key) * Elem.Value->GetMass() / Mass;
	}
	//UE_LOG(LogTemp, Warning, TEXT("cofmass?? x=%f, y=%f"), Center.X, Center.Y);
	
	
	return Center;
}

//Gets the mass of the PartGrid
const float UPartGridComponent::GetMass()
{
	//Initialize Variable
	float Mass = 0;

	//Interate though Pixels and summate their mass
	for (auto& Elem : PartGrid)
	{
		Mass += Elem.Value->GetMass();
	}
	//UE_LOG(LogTemp, Warning, TEXT("other mass = %f"), Mass);


	return Mass == 0 ? 1 : Mass;
}

//Gets the PartGrid Map
TMap<FIntPoint, UBasePart*> UPartGridComponent::GetPartGrid()
{
	return PartGrid;
}

//Detect if a Part can fit in the PartGrid
bool const UPartGridComponent::CanShapeFit(FIntPoint Loc, TArray<FIntPoint> DesiredShape)
{
	//Iterate through desired shape
	for (int i = 0; i < DesiredShape.Num(); i++)
	{
		//Cheak if there is already a pixel in the grid
		if (PartGrid.Contains(Loc + DesiredShape[i]))
		{
			return false;
		}
	}
	return true;
}

