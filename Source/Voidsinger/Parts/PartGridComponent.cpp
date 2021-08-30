// Fill out your copyright notice in the Description page of Project Settings.


#include "PartGridComponent.h"

// Sets default values for this component's properties
UPartGridComponent::UPartGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	GridBounds.UpperBounds = FIntPoint();
	GridBounds.LowerBounds = FIntPoint();

	GridSize = FIntPoint(250);
	
	PartGrid.AddZeroed(GridSize.X);
	for (int i = 0; i < GridSize.X; i++)
	{
		PartGrid[i].AddZeroed(GridSize.Y);
	}

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

bool UPartGridComponent::AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, TEnumAsByte<EPartRotation> Rotation, bool bAlwaysPlace)
{

	TArray<FIntPoint> DesiredShape = PartType.GetDefaultObject()->GetDesiredShape();
	FIntPoint PartBounds = PartType.GetDefaultObject()->GetShapeBounds();


	if (PartGrid.IsValidIndex(Location.X + PartBounds.X) && PartGrid[Location.X + PartBounds.X].IsValidIndex(Location.Y + PartBounds.Y) && (bAlwaysPlace || CanShapeFit(Location, DesiredShape)))
	{
		
		if (Location.X < GridBounds.LowerBounds.X)
		{
			GridBounds.LowerBounds.X = Location.X;
		}
		if (Location.Y < GridBounds.LowerBounds.Y)
		{
			GridBounds.LowerBounds.Y = Location.Y;
		}

		
		
		UBasePart* Part = NewObject<UBasePart>(PartType);
		Part->Rotation = Rotation;

		for (int i = 0; i < DesiredShape.Num(); i++)
		{
			PartGrid[DesiredShape[i].X + Location.X][DesiredShape[i].Y + Location.Y] = Part;
			
			class UActorComponent* NewPlane = GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform(FRotator(), FVector(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y, 0) * GridScale, FVector(GridScale)), false);
			Cast<UStaticMeshComponent>(NewPlane)->SetStaticMesh(PixelMesh);

			if (Location.X > GridBounds.UpperBounds.X)
			{
				GridBounds.UpperBounds.X = Location.X;
			}
			if (Location.Y > GridBounds.UpperBounds.Y)
			{
				GridBounds.UpperBounds.Y = Location.Y;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

void UPartGridComponent::BuildShip(TArray<FSavePartInfo> Parts)
{
	for (int i = 0; i < Parts.Num(); i++)
	{
		AddPart(Parts[i].PartClass, Parts[i].PartLocation, Parts[i].PartRotation, false);
	}
}

void UPartGridComponent::SaveShip()
{
	TArray<UBasePart*> Parts;
	for (int i = GridBounds.LowerBounds.X; i < GridBounds.UpperBounds.X; i++)
	{
		for (int j = GridBounds.LowerBounds.Y; j < GridBounds.UpperBounds.Y; j++)
		{
			if (IsValid(PartGrid[i][j]) && !Parts.Contains(PartGrid[i][j]))
			{
				Parts.Add(PartGrid[i][j]);
			}
		}
	}
	
	USaveGame* SaveGameInstance = UGameplayStatics::CreateSaveGameObject(USaveShip::StaticClass());

	for (int i = 0; i < Parts.Num(); i++)
	{

		Cast<USaveShip>(SaveGameInstance)->SavedShip.Add(FSavePartInfo(Parts[i]->GetClass(), Parts[i]->GetPartLocation(), Parts[i]->GetPartRotation()));
	}
}

bool const UPartGridComponent::CanShapeFit(FIntPoint Loc, TArray<FIntPoint> DesiredShape)
{
	for (int i = 0; i < DesiredShape.Num(); i++)
	{
		if (IsValid(PartGrid[DesiredShape[i].X + Loc.X][DesiredShape[i].Y + Loc.Y]))
		{
			return false;
		}
	}
	return true;
}

