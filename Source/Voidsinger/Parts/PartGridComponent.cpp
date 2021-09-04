// Fill out your copyright notice in the Description page of Project Settings.


#include "PartGridComponent.h"
#include "BasePart.h"

// Sets default values for this component's properties
UPartGridComponent::UPartGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	GridBounds.UpperBounds = FIntPoint();
	GridBounds.LowerBounds = FIntPoint();

	GridSize = FIntPoint(25);
	
	PartGrid = TMap<FIntPoint, UBasePart*>();

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

	TArray<FIntPoint> DesiredShape = PartType.GetDefaultObject()->GetDesiredShape(Rotation);
	FArrayBounds PartBounds = PartType.GetDefaultObject()->GetShapeBounds(Rotation);
	

	if (GridSize.X >= Location.X + PartBounds.UpperBounds.X && -GridSize.X <= Location.X + PartBounds.LowerBounds.X
		&&
		GridSize.Y >= Location.Y + PartBounds.UpperBounds.Y && -GridSize.Y <= Location.Y + PartBounds.LowerBounds.Y
		&&
		(bAlwaysPlace || CanShapeFit(Location, DesiredShape)))
	{		

		if (Location.X < GridBounds.LowerBounds.X)
		{
			GridBounds.LowerBounds.X = Location.X;
		}
		if (Location.Y < GridBounds.LowerBounds.Y)
		{
			GridBounds.LowerBounds.Y = Location.Y;
		}

		UBasePart* Part = NewObject<UBasePart>(this, PartType);
		Part->Init(Location, Rotation, this, PartType);

		for (int i = 0; i < DesiredShape.Num(); i++)
		{
			PartGrid.Add(FIntPoint(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y), Part);

			class UActorComponent* NewPlane = GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform(FRotator(), FVector(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y, 0) * GridScale, FVector(GridScale)), false);
			Cast<UStaticMeshComponent>(NewPlane)->SetStaticMesh(PartType.GetDefaultObject()->PixelMesh);

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
	return false;
}

void UPartGridComponent::BuildShip(TArray<FSavePartInfo> Parts)
{
	/*for (int i = 0; i < Parts.Num(); i++)
	{
		AddPart(Parts[i].PartClass, Parts[i].PartLocation, Parts[i].PartRotation, false);
	}*/
}

void UPartGridComponent::SaveShip(FString ShipName)
{
	/*
	TArray<UBasePart*> Parts;

	PartGrid.GenerateValueArray(Parts);
	
	USaveGame* SaveGameInstance = UGameplayStatics::CreateSaveGameObject(USaveShip::StaticClass());

	for (int i = 0; i < Parts.Num(); i++)
	{
		Cast<USaveShip>(SaveGameInstance)->SavedShip.Add(FSavePartInfo(Parts[i]->GetClass(), Parts[i]->GetLocation(), Parts[i]->GetRotation()));
	}
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, ShipName, 0);*/

}

void UPartGridComponent::LoadSavedShip(FString ShipName)
{
	/*USaveGame* SaveGameInstance = UGameplayStatics::LoadGameFromSlot(ShipName, 0);
	BuildShip(Cast<USaveShip>(SaveGameInstance)->SavedShip);*/
}

const FVector2D UPartGridComponent::GetCenterOfMass()
{
	FVector2D Center = FVector2D(0,0);
	float Mass = GetMass();
	UE_LOG(LogTemp, Warning, TEXT("PRE cofmass?? x=%f, y=%f, Mass=%f"), Center.X, Center.Y, Mass);
	for (auto& Elem : PartGrid)
	{
		Center += FVector2D(Elem.Key) * Elem.Value->GetMass() / Mass;
	}
	UE_LOG(LogTemp, Warning, TEXT("cofmass?? x=%f, y=%f"), Center.X, Center.Y);
	return Center;
}

const float UPartGridComponent::GetMass()
{
	float Mass = 0;
	for (auto& Elem : PartGrid)
	{
		Mass += Elem.Value->GetMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("other mass = %f"), Mass);
	return Mass == 0 ? 1 : Mass;
}

bool const UPartGridComponent::CanShapeFit(FIntPoint Loc, TArray<FIntPoint> DesiredShape)
{
	for (int i = 0; i < DesiredShape.Num(); i++)
	{
		
		if (PartGrid.Contains(Loc + DesiredShape[i]))
		{
			return false;
		}
	}
	return true;
}

