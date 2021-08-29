// Fill out your copyright notice in the Description page of Project Settings.


#include "PartGridComponent.h"

// Sets default values for this component's properties
UPartGridComponent::UPartGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PartGrid.AddDefaulted(GridSize.X);
	for (int i = 0; i < GridSize.X; i++)
	{
		PartGrid[i].AddDefaulted(GridSize.Y);
	}

	GridBounds.UpperBounds = FIntPoint();
	GridBounds.LowerBounds = FIntPoint();

	GridSize = FIntPoint(250);
	

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

bool UPartGridComponent::AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, bool bAlwaysPlace)
{
	TArray<FIntPoint> DesiredShape = PartType.GetDefaultObject()->GetDesiredShape();
	FIntPoint PartBounds = PartType.GetDefaultObject()->GetShapeBounds();


	if (PartGrid.IsValidIndex(Location.X + PartBounds.X) && PartGrid[Location.X + PartBounds.X].IsValidIndex(Location.Y + PartBounds.Y) && (bAlwaysPlace || CanShapeFit(Location, DesiredShape)))
	{
		UBasePart* Part = NewObject<UBasePart>(PartType);
		if (Location.X < GridBounds.LowerBounds.X)
		{
			GridBounds.LowerBounds.X = Location.X;
		}
		if (Location.Y < GridBounds.LowerBounds.Y)
		{
			GridBounds.LowerBounds.Y = Location.Y;
		}

		class UStaticMeshComponent* NewPlane = NewObject<UStaticMeshComponent>(this);
		


		for (int i = 0; i < DesiredShape.Num(); i++)
		{
			PartGrid[DesiredShape[i].X + Location.X][DesiredShape[i].Y + Location.Y] = Part;
			
			NewPlane->SetStaticMesh(PixelMesh);
			GetOwner()->AddInstanceComponent(NewPlane);
			NewPlane->SetRelativeLocation(FVector(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y, 0));

			if (Location.X > GridBounds.UpperBounds.X)
			{
				GridBounds.UpperBounds.X = Location.X;
			}
			if (Location.Y > GridBounds.UpperBounds.Y)
			{
				GridBounds.UpperBounds.Y = Location.Y;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
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

