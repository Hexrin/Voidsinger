// Fill out your copyright notice in the Description page of Project Settings.


#include "PartGridComponent.h"
#include "BaseThrusterPart.h"
#include "BasePart.h"

// Sets default values for this component's properties
UPartGridComponent::UPartGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = .5;

	//Initiate Variables
	GridBounds.UpperBounds = FIntPoint();
	GridBounds.LowerBounds = FIntPoint();

	GridSize = FIntPoint(50);
	
	PartGrid = TMap<FIntPoint, FPartData>();
	if (!GridScale)
	{
		GridScale = 1;
	}
	
	TimesSinceHeatTick = 0.f;
	HeatTickRate = 0.5f;
	HeatPropagationFactor = 0.5f;
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
	for (int i = 0; i < TimesSinceHeatTick / HeatTickRate; TimesSinceHeatTick -= HeatTickRate)
	{
		DistrubuteHeat();
	}
	TimesSinceHeatTick += DeltaTime;
}


//Adds a compleate part to the part grid
bool UPartGridComponent::AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace)
{
	TArray<FIntPoint> PartialPartShape = PartType.GetDefaultObject()->GetDesiredShape(Rotation);
	return AddPart(PartialPartShape, PartType, Location, Rotation, bAlwaysPlace);
}
//Adds a partial part to PartPrid
bool UPartGridComponent::AddPart(TArray<FIntPoint> PartialPartShape, TSubclassOf<UBasePart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace)
{
	//Create Part
	UBasePart* Part = NewObject<UBasePart>(this, PartType);
	Part->InitializeVariables(Location, Rotation, this, PartType);

	//Initalize Variables
	TArray<FIntPoint> DesiredShape = Part->GetDesiredShape();
	FArrayBounds PartBounds = Part->GetPartBounds();

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
			if (PartialPartShape.Contains(DesiredShape[i]))
			{
				//Remove Overlaping Parts
				if (bAlwaysPlace)
				{
					RemovePart(FIntPoint(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y));
				}

				//Create Mesh
				class UActorComponent* NewPlane = GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform(FRotator(), FVector(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y, 0) * GridScale, FVector(GridScale)), false);
				Cast<UStaticMeshComponent>(NewPlane)->SetStaticMesh(PixelMesh);
				Cast<UStaticMeshComponent>(NewPlane)->SetMaterial(0, Part->GetPixelMaterial());

				PartGrid.Emplace(FIntPoint(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y), FPartData(Part, 0.f, Cast<UStaticMeshComponent>(NewPlane)));
			}
		}
		Part->InitializeFunctionality();
		Cast<ABaseShip>(GetOwner())->PhysicsComponent->UpdateMassCalculations();
		if (Cast<UBaseThrusterPart>(Part))
		{
			Cast<ABaseShip>(GetOwner())->MovementComponent->UpdateThrusters();
		}
		return true;
	}

	Part->DestroyPart();
	return false;
}


//Remove an entire part. Returns True if a part was destroyed
bool UPartGridComponent::RemovePart(FIntPoint Location)
{
	//Check if location is valid
	if (PartGrid.Contains(Location))
	{
		//Intialize Variables
		class UBasePart* PartToRemove = PartGrid.Find(Location)->Part;
		FIntPoint PartLoc = PartToRemove->GetLocation();

		//Iterate though the shape of PartToRemove and remove them from the part grid
		for (FIntPoint Loc : PartToRemove->GetShape())
		{
			DestroyPixel(Loc + PartLoc);
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
	if (PartGrid.Contains(Location))
	{
		//Remove from grid
		UBasePart* DamagedPart = PartGrid.FindRef(Location).Part;
		DamagedPart->DestroyPixel(Location - DamagedPart->GetLocation());

		//Destroy Mesh
		PartGrid.FindRef(Location).PixelMesh->DestroyComponent();

		PartGrid.Remove(Location);
		Cast<ABaseShip>(GetOwner())->PhysicsComponent->UpdateMassCalculations();
		return true;
	}
	else
	{
		return false;
	}
}
void UPartGridComponent::ApplyHeatAtLocation(FVector WorldLocation, float HeatToApply)
{
	ApplyHeatAtLocation((FVector2D(WorldLocation - GetOwner()->GetActorLocation()).GetRotated(-1 * GetOwner()->GetActorRotation().Yaw) + GetCenterOfMass()).RoundToVector().IntPoint(), HeatToApply);
		
	//PartGrid.FindRef(FVector2D(WorldLocation - GetOwner()->GetActorLocation()).RoundToVector().IntPoint()).SetTemperature(HeatToApply);
}
void UPartGridComponent::ApplyHeatAtLocation(FIntPoint RelativeLocation, float HeatToApply)
{
	if (PartGrid.Contains(RelativeLocation))
	{
		PartGrid.Find(RelativeLocation)->SetTemperature(PartGrid.Find(RelativeLocation)->Temperature + HeatToApply);
	}
	
}
void UPartGridComponent::ExplodeAtLocation(FVector WorldLocation, float ExplosionRadius)
 {

	FVector FloatRelativeLoc = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetActorTransform(), WorldLocation);
	float CheckX = -ExplosionRadius;
	float CheckY = -ExplosionRadius;
	FIntPoint CheckGridLocation;
	TArray<FIntPoint> LocationsToBeDestroyed;
	//DrawDebugPoint(GetWorld(), WorldLocation, ExplosionRadius * 50, FColor::Red, false, 2.0F);
	
	//UE_LOG(LogTemp, Warning, TEXT("Explosion %f"), ExplosionRadius);
	//UE_LOG(LogTemp, Warning, TEXT("CheckY %f"), CheckY);

	while (ExplosionRadius > CheckY)
	{
		
		CheckGridLocation = FIntPoint(CheckX + FloatRelativeLoc.X, CheckY + FloatRelativeLoc.Y);
		if (PartGrid.Contains(CheckGridLocation) && CheckX * CheckX + CheckY * CheckY <= ExplosionRadius * ExplosionRadius)
		{
			//obviously if it contains the FloatRelativeLoc then it ded
			if (BoxContainsLocation(FVector2D(CheckGridLocation.X - GridScale / 2, CheckGridLocation.Y + GridScale / 2), FVector2D(CheckGridLocation.X + GridScale / 2, CheckGridLocation.Y - GridScale / 2), FVector2D(FloatRelativeLoc)))
			{
				LocationsToBeDestroyed.Emplace(CheckGridLocation);
			}
			// PartGrid.Find(CheckGridLocation)->Part->GetStrength())
			//y = mx + b
			// Assume for a second FloatRelativeLoc is the origin of the ship ((0,0) on the part grid) and the part is somewhere between directly above and directly to the right
			// of the FloatRelativeLoc
			// b is 0
			//slope will be CheckGridLocation.Y/CheckGridLocation.X
			//How to figure out what parts to check?
			//assume for a second the CheckGridLocation is (1,2)
			//Slope is 2
			//y = 2x
			//if we just go by integers of the slope next point checked is the origin. not super helpful
			// Alternate x and y, but if the pixel doesn't contain something in the slope check the other?
			// Check X - 1 from the CheckGridLocation, if the bounds of the pixel do not contain something in the slope, check y - 1
			// How to check if bounds of pixel contain something in the slope?
			// in this case, if top left location Y is > the Y with the slope of the line at that X location 
			// and bottom right Y is < the Y with the slop of the line at that X location
			// Divide radius by the strength of the pixel found, then check y-1 x-1. Divide by that strength then check y-2 x-1
			// and so on till you get to origin. So there needs to be a recursive function. or maybe a while()? Yes, while(Location checking does not contain origin)
			//So separate slope rise and slope run.
			// What if FloatRelativeLoc is not 0,0 (still assuming part is up and to the right)
			//slope rise and run will be (CheckGridLocation.Y - FloatRelativeLoc.Y) and (CheckGridLocation.X - FloatRelativeLoc.X)
			// Check X - 1 from the CheckGridLocation, if the bounds of the pixel do not contain something in the slope, check y - 1
			// Divide radius by the strength of all the pixels found
			//and so on until you get to the location that contains the FloatRelativeLoc. So there should also be a check for each location if it contains the FloatRelativeLoc
			//How to check if it contains the FloatRelativeLoc?
			//Top left of location X is < FloatRelativeLoc.X and Y is > FloatRelativeLoc.Y,
			//and bottom right of location X is > FloatRelationLoc.X and Y is < FloatRelativeLoc.Y
			//What if the part is not up and to the right?
			//Need to figure out what quadrant the part is in or what axis it's on compared to the FloatRelativeLoc
			//Okay so a switch based on the quadrant
			else
			{
				int XDirection = 0;
				int YDirection = 0;
				bool StopChecking = false;
				float SlopeRise = CheckGridLocation.X - FloatRelativeLoc.X;
				float SlopeRun = CheckGridLocation.Y - FloatRelativeLoc.Y;

 				switch (GetQuadrantFromLocation(FVector2D(CheckGridLocation.X, CheckGridLocation.Y), FVector2D(FloatRelativeLoc)))
				{

				case 0:
					StopChecking = true;
					break;
				case 1:
					XDirection = -1;
					YDirection = -1;
					break;
				case 2:
					XDirection = -1;
					YDirection = 1;
					break;
				case 3:
					XDirection = 1;
					YDirection = 1;
					break;
				case 4:
					XDirection = 1;
					YDirection = -1;
					break;
				case 5:
					YDirection = -1;
					break;
				case 6:
					XDirection = -1;
					break;
				case 7:
					YDirection = 1;
					break;
				case 8:
					XDirection = 1;
					break;
				}

				int XLocation = CheckGridLocation.X;
				int YLocation = CheckGridLocation.Y;
				float NewRadius = ExplosionRadius;
				while (!StopChecking)
				{
					//XLocation += XDirection;
					//YLocation += YDirection;

					if (BoxContainsLocation(FVector2D(float(XLocation) + GridScale / 2, float(YLocation) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation) + GridScale / 2), FVector2D(FloatRelativeLoc)))
					{
						if (PartGrid.Contains(FIntPoint(XLocation + XDirection, YLocation)))
						{
							NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation + XDirection, YLocation))->Part->GetStrength();
						}
						StopChecking = true;
						break;
					}
					if (BoxContainsLocation(FVector2D(float(XLocation + XDirection) + GridScale / 2, float(YLocation) - GridScale / 2), FVector2D(float(XLocation + XDirection) - GridScale / 2, float(YLocation) + GridScale / 2), FVector2D(FloatRelativeLoc)))
					{
						if (PartGrid.Contains(FIntPoint(XLocation + XDirection, YLocation)))
						{
							NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation + XDirection, YLocation))->Part->GetStrength();
						}
						StopChecking = true;
						break;
					}
					else if (YDirection != 0)
					{
						if (XDirection != 0)
						{
							if (DoesLineIntersectBox(FVector2D(float(XLocation + XDirection) + GridScale / 2, float(YLocation) - GridScale / 2), FVector2D(float(XLocation + XDirection) - GridScale / 2, float(YLocation) + GridScale / 2), SlopeRise, SlopeRun, FVector2D(FloatRelativeLoc)))
							{
								XLocation += XDirection;
								if (PartGrid.Contains(FIntPoint(XLocation, YLocation)))
								{
									NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation))->Part->GetStrength();
								}
							}
							else if (BoxContainsLocation(FVector2D(float(XLocation) + GridScale / 2, float(YLocation + YDirection) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation + YDirection) + GridScale / 2), FVector2D(FloatRelativeLoc)))
							{
								if (PartGrid.Contains(FIntPoint(XLocation, YLocation + YDirection)))
								{
									NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation + YDirection))->Part->GetStrength();
								}
								StopChecking = true;
								break;
							}
							else if (DoesLineIntersectBox(FVector2D(float(XLocation) + GridScale / 2, float(YLocation + YDirection) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation + YDirection) + GridScale / 2), SlopeRise, SlopeRun, FVector2D(FloatRelativeLoc)))
							{
								YLocation += YDirection;
								if (PartGrid.Contains(FIntPoint(XLocation, YLocation)))
								{
									NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation))->Part->GetStrength();
								}
							}
						}
						else if (BoxContainsLocation(FVector2D(float(XLocation) + GridScale / 2, float(YLocation + YDirection) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation + YDirection) + GridScale / 2), FVector2D(FloatRelativeLoc)))
						{
							if (PartGrid.Contains(FIntPoint(XLocation, YLocation + YDirection)))
							{
								NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation + YDirection))->Part->GetStrength();
							}
							StopChecking = true;
							break;
						}
						else if (DoesLineIntersectBox(FVector2D(float(XLocation) + GridScale / 2, float(YLocation + YDirection) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation + YDirection) + GridScale / 2), SlopeRise, SlopeRun, FVector2D(FloatRelativeLoc)))
						{
							YLocation += YDirection;
							if (PartGrid.Contains(FIntPoint(XLocation, YLocation)))
							{
								NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation))->Part->GetStrength();
							}
						}
					}
					else
					{
						if (DoesLineIntersectBox(FVector2D(float(XLocation + XDirection) + GridScale / 2, float(YLocation) - GridScale / 2), FVector2D(float(XLocation + XDirection) - GridScale /2, float(YLocation) + GridScale / 2), FloatRelativeLoc.Y))
						{
							XLocation += XDirection;
							if (PartGrid.Contains(FIntPoint(XLocation, YLocation)))
							{
								NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation))->Part->GetStrength();
							}
						}
					}
				}
				if (PartGrid.Contains(CheckGridLocation) && CheckX * CheckX + CheckY * CheckY <= NewRadius * NewRadius)
				{
					LocationsToBeDestroyed.Emplace(CheckGridLocation);
				}
			}
		}
		CheckX += 1;
		if (CheckX >= ExplosionRadius)
		{
			CheckY += 1;
			CheckX = -ExplosionRadius;
		}
	}
	for (auto& i : LocationsToBeDestroyed)
	{
		DestroyPixel(i);
	}

	//FIntPoint IntRelativeLoc = FVector2D(FloatRelativeLoc).GetRotated(-1 * GetOwner()->GetActorRotation().Yaw).RoundToVector().IntPoint());
}

bool UPartGridComponent::BoxContainsLocation(FVector2D TopLeft, FVector2D BottomRight, FVector2D Location)
{
	if (TopLeft.X >= Location.X && TopLeft.Y <= Location.Y && BottomRight.X <= Location.X && BottomRight.Y >= Location.Y)
	{
		return true;
	}
	return false;
}

int UPartGridComponent::GetQuadrantFromLocation(FVector2D Location, FVector2D Origin)
{

	//Check if the location is the origin.
	if (Location == Origin)
	{
		return 0;
	}

	//Check if the location is on the Y axis.
	if (Location.X == Origin.X)
	{
		if (Location.Y > Origin.Y)
		{
			return 5;
		}
		else
		{
			return 7;
		}
	}

	//Check if the location is on the X axis.
	if (Location.Y == Origin.Y)
	{
		if (Location.X > Origin.X)
		{
			return 6;
		}
		else
		{
			return 8;
		}
	}

	//Check if the location is in the first or second quadrants.
	if (Location.X > Origin.X)
	{
		//Check if the location is in the first quadrant.
		if (Location.Y > Origin.Y)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		//Check if the location is in the fourth quadrant.
		if (Location.Y > Origin.Y)
		{
			return 4;
		}
		else
		{
			return 3;
		}
	}
}

bool UPartGridComponent::DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float SlopeRise, float SlopeRun, FVector2D origin)
{
	float XIntercept = 0;
	FVector2D LocalTopLeft = TopLeft - origin;
	FVector2D LocalBottomRight = BottomRight - origin;

	if (SlopeRise == 0)
	{
		if (LocalTopLeft.X > XIntercept && LocalBottomRight.X < XIntercept)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (SlopeRise < 0 || SlopeRun < 0)
	{
		if (!(SlopeRise < 0 && SlopeRun < 0))
		{
			FVector2D LocalBottomLeft = FVector2D(LocalBottomRight.X, LocalTopLeft.Y);
			FVector2D LocalTopRight = FVector2D(LocalTopLeft.X, LocalBottomRight.Y);

			if (LocalBottomLeft.X <= (SlopeRise / SlopeRun) * (LocalBottomLeft.Y) + XIntercept && LocalTopRight.X >= (SlopeRise/SlopeRun) * (LocalTopRight.Y) + XIntercept)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	if (LocalTopLeft.X >= (SlopeRise / SlopeRun) * (LocalTopLeft.Y) + XIntercept && LocalBottomRight.X <= (SlopeRise / SlopeRun) * (LocalBottomRight.Y) + XIntercept)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UPartGridComponent::DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float YIntercept)
{
	return TopLeft.Y < YIntercept&& BottomRight.Y > YIntercept;
}

void UPartGridComponent::DistrubuteHeat()
{
	TMap<FIntPoint, float> NewHeatMap = TMap<FIntPoint, float>();
	NewHeatMap.Reserve(PartGrid.Num());
	for (auto& Data : PartGrid)
	{
		float NewHeat = 0;
		for (int i = 0; i < 4; i++)
		{
			FIntPoint TargetPoint = ((i % 2 == 1) ? FIntPoint((i > 1) ? 1 : -1, 0) : FIntPoint(0, (i > 1) ? 1 : -1));
			
			if (PartGrid.Contains(TargetPoint + Data.Key))
			{
				NewHeat += PartGrid.FindRef(TargetPoint + Data.Key).Temperature * HeatPropagationFactor / (4);
			}
		}
		NewHeat = Data.Value.Temperature * (1-HeatPropagationFactor) + NewHeat;
		NewHeatMap.Emplace(Data.Key, NewHeat > .05 ? NewHeat : 0);
	}

	TArray<FIntPoint> KeysToDestroy = TArray<FIntPoint>();
	for (auto& Data : PartGrid)
	{
		if (NewHeatMap.FindRef(Data.Key) == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Key X=%i, y=%i     |     Temp =%f"), Data.Key.X, Data.Key.Y, NewHeatMap.FindRef(Data.Key));
		}
		else
			UE_LOG(LogTemp, Error, TEXT("Key X=%i, y=%i     |     Temp =%f"), Data.Key.X, Data.Key.Y, NewHeatMap.FindRef(Data.Key));

		if (NewHeatMap.FindRef(Data.Key) > 2)
		{
			KeysToDestroy.Emplace(Data.Key);
		}
		else
		{
			Data.Value.SetTemperature(NewHeatMap.FindRef(Data.Key));
		}
	}
	for (FIntPoint Val : KeysToDestroy)
	{
		DestroyPixel(Val);
	}
}
bool UPartGridComponent::DestroyPixel(FIntPoint Location, class UBasePart*& DamagedPart)
{
	//Check if pixel is valid
	if (PartGrid.Contains(Location))
	{
		//Remove from grid
		DamagedPart = PartGrid.FindRef(Location).Part;
		DamagedPart->DestroyPixel(Location - DamagedPart->GetLocation());
		
		//Destroy Mesh
		PartGrid.FindRef(Location).PixelMesh->DestroyComponent();

		PartGrid.Remove(Location);
		Cast<ABaseShip>(GetOwner())->PhysicsComponent->UpdateMassCalculations();
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
	
	TArray<FPartData> Parts;

	PartGrid.GenerateValueArray(Parts);
	
	USaveGame* SaveGameInstance = UGameplayStatics::CreateSaveGameObject(USaveShip::StaticClass());

	for (int i = 0; i < Parts.Num(); i++)
	{
		Cast<USaveShip>(SaveGameInstance)->SavedShip.Add(FSavePartInfo(Parts[i].Part->GetClass(), Parts[i].Part->GetLocation(), Parts[i].Part->GetRotation()));
	}
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, ShipName, 0);

}

bool UPartGridComponent::LoadSavedShip(FString ShipName)
{
	USaveGame* SaveGameInstance = UGameplayStatics::LoadGameFromSlot(ShipName, 0);
	if (IsValid(SaveGameInstance))
	{
		BuildShip(Cast<USaveShip>(SaveGameInstance)->SavedShip);
		return true;
	}
	else
	{
		return false;
	}
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
		Center += FVector2D(Elem.Key) * Elem.Value.Part->GetMass() / Mass;
	}
	//UE_LOG(LogTemp, Warning, TEXT("cofmass?? x=%f, y=%f"), Center.X, Center.Y);
	
	
	return Center;
}

const float UPartGridComponent::GetMomentOfInertia()
{
	float ReturnValue = 0;
	FVector2D CenterOfMass = GetCenterOfMass();
	for (auto& Part : PartGrid)
	{
		ReturnValue += (Part.Value.Part->GetMass() / 6) + (FVector2D(Part.Value.Part->GetLocation()) - CenterOfMass).SizeSquared();
	}
	return ReturnValue;
}

//Gets the mass of the PartGrid
const float UPartGridComponent::GetMass()
{
	//Initialize Variable
	float Mass = 0;

	//Interate though Pixels and summate their mass
	for (auto& Elem : PartGrid)
	{
		Mass += Elem.Value.Part->GetMass();
	}
	//UE_LOG(LogTemp, Warning, TEXT("other mass = %f"), Mass);


	return Mass == 0 ? 1 : Mass;
}

//Gets the PartGrid Map
TMap<FIntPoint, FPartData> UPartGridComponent::GetPartGrid()
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

