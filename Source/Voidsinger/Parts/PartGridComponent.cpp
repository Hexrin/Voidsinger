// Fill out your copyright notice in the Description page of Project Settings.


#include "PartGridComponent.h"
#include "BasePart.h"
#include "BaseThrusterPart.h"
#include "CorePart.h"
#include "BaseFreespacePart.h"
#include "Voidsinger/ShipPlayerState.h"
#include "Voidsinger/Ships/BaseShip.h"

// Sets default values for this component's properties
UPartGridComponent::UPartGridComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = .5;

	//Initialize Variables
	//Could GridBounds and GridHalfSize be done in the h file? Why is grid half size 250? that shouldn't be hard
	//coded -Mabel
	GridBounds = FArrayBounds(FIntPoint(0, 0), FIntPoint(0, 0));
	Ship = Cast<ABaseShip>(GetOwner());
	GridHalfSize = FIntPoint(250);
	
	PartGrid = TGridMap<FPartData>();

	if (!GridScale)
	{
		GridScale = 1;
	}
	
	//Stop magic numbering -Mabel
	TimesSinceHeatTick = 0.f;
	HeatTickRate = 0.5f;
	HeatPropagationFactor = 0.5f;
	HeatMeltTransferFactor = 1.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PixelMeshAsset(*PathToPixelMesh);
	if (PixelMeshAsset.Succeeded())
	{
		PixelMesh = PixelMeshAsset.Object;
	}
	
}

// Called when the game starts
void UPartGridComponent::BeginPlay()
{
	//Why is this done in the construction script and begin play? -Mabel
	Ship = Cast<ABaseShip>(GetOwner());
	Super::BeginPlay();
	GridBounds = FArrayBounds(FIntPoint(0, 0), FIntPoint(0, 0));
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

//Adds a complete part to the part grid
//I like how you commented the cpp but not the h -Mabel
bool UPartGridComponent::AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace)
{
	TArray<FIntPoint> PartialPartShape = PartType.GetDefaultObject()->GetDesiredShape(Rotation);
	return AddPart(PartialPartShape, PartType, Location, Rotation, bAlwaysPlace);
}

//Adds a partial part to PartPrid
bool UPartGridComponent::AddPart(TArray<FIntPoint> PartialPartShape, TSubclassOf<UBasePart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace)
{
	//At first glance this function seems a bit long, it could probably be split into some sub-functions. -Mabel
	if (!PartialPartShape.IsEmpty())
	{
		//Create Part
		UBasePart* Part = NewObject<UBasePart>(this, PartType);
		Part->InitializeVariables(Location, Rotation, this, PartType);

		//Initalize Variables
		TArray<FIntPoint> DesiredShape = Part->GetDesiredShape();
		FArrayBounds PartBounds = Part->GetPartBounds();

		//Detect if placement is in valid position
		//Move this to a new function called IsValidPosition or something, or IsWithinGridBounds idk -Mabel
		if 
		(
			GridHalfSize.X >= Location.X + PartBounds.UpperBounds.X && -GridHalfSize.X <= Location.X + PartBounds.LowerBounds.X
			&&
			GridHalfSize.Y >= Location.Y + PartBounds.UpperBounds.Y && -GridHalfSize.Y <= Location.Y + PartBounds.LowerBounds.Y
			&&
			(bAlwaysPlace || CanShapeFit(Location, DesiredShape))
		)
		{

			//Update GridBounds
			GridBounds.LowerBounds = GridBounds.LowerBounds.ComponentMin(Location + PartBounds.LowerBounds);
			GridBounds.UpperBounds = GridBounds.UpperBounds.ComponentMax(Location + PartBounds.UpperBounds);


			//Iterate though desired shape and add to part grid
			//Comment some of these for loops and if statements because it's a lot to try to decipher -Mabel
			for (int i = 0; i < DesiredShape.Num(); i++)
			{
				if (PartialPartShape.Contains(DesiredShape[i]))
				{
					FIntPoint CurrentLoc = FIntPoint(DesiredShape[i].X + Location.X, DesiredShape[i].Y + Location.Y);

					//Remove Overlapping Parts
					//Why is this inside the check for "IsValidPosition"? This will never be called if it isn't in a valid 
					//position which defeats the purpose of bAlwaysPlace -Mabel
					if (bAlwaysPlace)
					{
						RemovePart(CurrentLoc);
					}

					//Create Mesh
					Ship->AddMeshAtLocation(CurrentLoc);

					UBaseFreespacePart* PartAsFreeform = Cast<UBaseFreespacePart>(Part);
					if (PartAsFreeform)
					{
						for (int32 j = 0; j < 4; j++)
						{
							//Comment whatever the heck is going on here -Mabel
							FIntPoint CheckLocation = (((j % 2 == 0)) ? FIntPoint(((j > 1)) ? 1 : -1, 0) : FIntPoint(0, ((j > 1)) ? 1 : -1)) + Location;
							if (PartGrid.Contains(CheckLocation))
							{
								UBaseFreespacePart* PartToMergeWith = Cast<UBaseFreespacePart>(PartGrid.Find(CheckLocation)->Part);
								if (PartToMergeWith)
								{
									Part->ConnectToSystems();
									PartToMergeWith->MergeParts(PartAsFreeform);
									Part = PartToMergeWith;
								}
							}
						}
					}
					//set PartGrid and material
					Ship->SetMeshMaterialAtLocation(CurrentLoc, PartGrid.Emplace(CurrentLoc, FPartData(Part, 0.f, 0, Part->GetPixelMaterial())).DynamicMat);

					//If it's commented out, delete it (unless it's useful debug stuff) -Mabel
					//Cast<AShipPlayerState>(Ship->GetPlayerState())->ShipBlueprint

					//Remember to coment out debug stuff. We should probably add a comment with the word "Debug" in
					//front of debug stuff
					UE_LOG(LogTemp, Warning, TEXT("Added: %s"), *Part->GetFName().ToString());
				}
			}
			Part->InitializeFunctionality();

			Ship->PhysicsComponent->UpdateMassCalculations();

			if (Cast<UBaseThrusterPart>(Part))
			{
				Ship->MovementComponent->UpdateThrusters();
			}

			if (IsValid(Cast<UCorePart>(Part)))
			{
				CorePart = Cast<UCorePart>(Part);
			}

			return true;
		}
		Part->DestroyPart();
		return false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PartialPartShape was empty"));
		return false;
	}
}

//Remove an entire part. Returns True if a part was destroyed
bool UPartGridComponent::RemovePart(FIntPoint Location, bool CheckForBreaks)
{
	//Check if location is valid
	if (PartGrid.Contains(Location))
	{
		//Intialize Variables
		class UBasePart* PartToRemove = PartGrid.FindRef(Location).Part;
		FIntPoint PartLoc = PartToRemove->GetPartGridLocation();

		//Iterate though the shape of PartToRemove and remove them from the part grid
		for (FIntPoint Loc : PartToRemove->GetShape())
		{
			DestroyPixel(Loc + PartLoc, CheckForBreaks);
		}
		return true;
	}
	else
	{
		return false;
	}
}

//Remove a single Pixel from the PartGrid. Returns true if a pixel was removed
bool UPartGridComponent::DestroyPixel(FIntPoint Location, bool CheckForBreaks, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius)
{
	if (PartGrid.Contains(Location))
	{
		//Remove from grid
		UBasePart* DamagedPart = PartGrid.FindRef(Location).Part;
		DamagedPart->DestroyPixel(Location - DamagedPart->GetPartGridLocation());

		//Destroy Mesh
		Ship->RemoveMeshAtLocation(Location);

		for (int i = 0; i < 4; i++)
		{
			FIntPoint TargetPoint = Location +((i % 2 == 1) ? FIntPoint((i > 1) ? 1 : -1, 0) : FIntPoint(0, (i > 1) ? 1 : -1));

			ApplyHeatAtLocation(TargetPoint, (PartGrid.FindRef(Location).GetTemperature() / 4) * HeatMeltTransferFactor);
		}

		TArray<FIntPoint> NumbersFound;
		PartGrid.Remove(Location);

		if (CheckForBreaks)
		{
			if (PartGrid.Contains(FIntPoint(Location.X + 1, Location.Y)))
			{
				NumbersFound.Add(FIntPoint(Location.X + 1, Location.Y));
			}
			if (PartGrid.Contains(FIntPoint(Location.X - 1, Location.Y)))
			{
				NumbersFound.Add(FIntPoint(Location.X - 1, Location.Y));
			}
			if (PartGrid.Contains(FIntPoint(Location.X, Location.Y + 1)))
			{
				NumbersFound.Add(FIntPoint(Location.X, Location.Y + 1));
			}
			if (PartGrid.Contains(FIntPoint(Location.X, Location.Y - 1)))
			{
				NumbersFound.Add(FIntPoint(Location.X, Location.Y - 1));
			}

			//If NumbersFound is less than 2 then you don't need to bother checking anything since there will be no breaks
			if (NumbersFound.Num() > 1)
			{
				if (IsValid(CorePart) && !CorePart->GetShape().IsEmpty())
				{
					for (auto& i : NumbersFound)
					{
						if (PartGrid.Contains(i))
						{
							if (!PartGrid.PointsConnected(CorePart->GetShape()[0], i, AlwaysConnect<FPartData>))
							{
								TArray<FIntPoint> Temp;
								Temp.Emplace(i);
								TArray<FIntPoint> ConnectedShape = UPartGridComponent::FindConnectedShape(Temp, PartGrid);

								RemoveDisconnectedShape(ConnectedShape, FromExplosion, ExplosionLocation, ExplosionRadius);
							}
						}
					}
				}
				else
				{
					//For each in NumbersFound.Num() - 1 because of how PointsConnected works
					for (int i = 0; i < NumbersFound.Num() - 1; i++)
					{
						//This needs to be improved, but right now it checks if the current index is connected to the next index.
						//actually it might not need to be improved but i need to think about it
						if (PartGrid.Contains(NumbersFound[i]) && PartGrid.Contains(NumbersFound[i + 1]))
						{
							if (!PartGrid.PointsConnected(NumbersFound[i], NumbersFound[i + 1], AlwaysConnect<FPartData>))
							{
								//If they're not connected, then call FindConnectedShape to figure out what part is not connected. Anything connected to the part that is not connected will
								//also not be connected.
								TArray<FIntPoint> Temp;
								Temp.Emplace(NumbersFound[i + 1]);
								TArray<FIntPoint> ConnectedShape = UPartGridComponent::FindConnectedShape(Temp, PartGrid);

								RemoveDisconnectedShape(ConnectedShape, FromExplosion, ExplosionLocation, ExplosionRadius);

							}
						}
					}
				}
			}
		}
		Ship->PhysicsComponent->UpdateMassCalculations();
		return true;
	}
	else
	{
		return false;
	}

}

void UPartGridComponent::RemoveDisconnectedShape(TArray<FIntPoint> ConnectedShape, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius)
{
	TSet<UBasePart*> PartsRemoved;

	for (auto& j : ConnectedShape)
	{
		PartsRemoved.Emplace(PartGrid.Find(j)->Part);
	}

	//Create a new ship with these parts
	if (!PartsRemoved.IsEmpty())
	{

		ABaseShip* NewShip = GetWorld()->SpawnActor<ABaseShip>((GetOwner()->GetActorLocation(), FQuat(), FActorSpawnParameters()));

		for (auto& j : PartsRemoved)
		{
			TArray<FIntPoint> PartialPartShape;
			for (auto& k : j->GetShape())
			{
				if (ConnectedShape.Contains(k + j->GetPartGridLocation()))
				{
					PartialPartShape.Emplace(k);
					DestroyPixel(k + j->GetPartGridLocation(), false);
				}
			}

			NewShip->PartGrid->AddPart(PartialPartShape, j->GetClass(), j->GetPartGridLocation(), j->GetRelativeRotation());
		}


		FVector NewLocation = GetOwner()->GetActorLocation() + FVector(NewShip->PhysicsComponent->GetCenterOfMass(), 0) - FVector(Ship->PhysicsComponent->GetCenterOfMass(), 0);
		FVector RotateLocation = GetOwner()->GetActorLocation();

		NewLocation = RotateLocation - GetOwner()->GetActorRotation().RotateVector(RotateLocation - NewLocation);


		NewShip->SetActorLocation(NewLocation);
		NewShip->SetActorRotation(GetOwner()->GetActorRotation());

		float Radius = UKismetMathLibrary::Sqrt(FMath::Square((NewShip->PhysicsComponent->GetCenterOfMass().X + NewShip->GetActorLocation().X) - (Ship->PhysicsComponent->GetCenterOfMass().X + Cast<AActor>(GetOwner())->GetActorLocation().X)) + FMath::Square((NewShip->PhysicsComponent->GetCenterOfMass().Y + NewShip->GetActorLocation().Y) - (Ship->PhysicsComponent->GetCenterOfMass().Y + Cast<AActor>(GetOwner())->GetActorLocation().Y)));
		float VelocityFromRotationMagnitude = Ship->PhysicsComponent->GetAngularVelocity() * Radius;
		FVector2D VectorBetween = NewShip->PhysicsComponent->GetCenterOfMass() + FVector2D(NewShip->GetActorLocation()) - (Ship->PhysicsComponent->GetCenterOfMass() + FVector2D(Cast<AActor>(GetOwner())->GetActorLocation()));
		FVector2D RotatedVector = VectorBetween.GetRotated(90);

		RotatedVector.Normalize();

		//Debug stuff
		/*UE_LOG(LogTemp, Warning, TEXT("velocity from rotation magnitude %f"), VelocityFromRotationMagnitude);
		UE_LOG(LogTemp, Warning, TEXT("angular velocity of the ship %f"), Ship->PhysicsComponent->GetAngularVelocity());
		UE_LOG(LogTemp, Warning, TEXT("radius %f"), Radius);
		DrawDebugDirectionalArrow(GetWorld(), NewShip->GetActorLocation(), NewShip->GetActorLocation() + FVector(RotatedVector * VelocityFromRotationMagnitude, 0), 5, FColor::Red, true);
		DrawDebugDirectionalArrow(GetWorld(), NewShip->GetActorLocation(), NewShip->GetActorLocation() + FVector(Ship->PhysicsComponent->GetVelocity(), 0), 5, FColor::Blue, true);
		DrawDebugDirectionalArrow(GetWorld(), NewShip->GetActorLocation(), NewShip->GetActorLocation() + FVector(Ship->PhysicsComponent->GetVelocity(), 0) + FVector(RotatedVector * VelocityFromRotationMagnitude, 0), 5, FColor::Green, true);
		DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + FVector(VectorBetween, 0), 5, FColor::Yellow, true);*/

		if (FromExplosion)
		{
			NewShip->PartGrid->ExplodeAtLocation(ExplosionLocation, ExplosionRadius);
		}

		NewShip->PhysicsComponent->AddImpulse((RotatedVector * VelocityFromRotationMagnitude) + Ship->PhysicsComponent->GetVelocity(), NewShip->PhysicsComponent->GetCenterOfMass());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("I don't think this should ever happen. Ask Mabel about weird part grid component thing"));
	}
}

//Comment -Mabel
void UPartGridComponent::ApplyHeatAtLocation(FVector WorldLocation, float HeatToApply)
{
	ApplyHeatAtLocation((FVector2D(WorldLocation - GetOwner()->GetActorLocation()).GetRotated(-1 * GetOwner()->GetActorRotation().Yaw) + Ship->PhysicsComponent->GetCenterOfMass()).RoundToVector().IntPoint(), HeatToApply);
		
	//PartGrid.FindRef(FVector2D(WorldLocation - GetOwner()->GetActorLocation()).RoundToVector().IntPoint()).SetTemperature(HeatToApply);
}

//Comment -Mabel
void UPartGridComponent::ApplyHeatAtLocation(FIntPoint RelativeLocation, float HeatToApply)
{
	if (PartGrid.Contains(RelativeLocation))
	{
		PartGrid.Find(RelativeLocation)->SetTemperature(PartGrid.Find(RelativeLocation)->GetTemperature() + HeatToApply);
	}
	
}

//I need to comment -Mabel
void UPartGridComponent::ExplodeAtLocation(FVector WorldLocation, float ExplosionRadius)
 {
	//UE_LOG(LogTemp, Warning, TEXT("explodd but part grid"))
	FVector FloatRelativeLoc = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetActorTransform(), WorldLocation) + FVector(Ship->PhysicsComponent->GetCenterOfMass(), 0);
	float CheckX = -ExplosionRadius;
	float CheckY = -ExplosionRadius;
	FIntPoint CheckGridLocation;
	TArray<FIntPoint> LocationsToBeDestroyed;

	//DrawDebugPoint(GetWorld(), WorldLocation, ExplosionRadius * 50, FColor::Red, false, 2.0F);
	
	//UE_LOG(LogTemp, Warning, TEXT("Explosion %f"), ExplosionRadius);
	//UE_LOG(LogTemp, Warning, TEXT("CheckY %f"), CheckY);

	while (ExplosionRadius >= CheckY)
	{
		CheckGridLocation = FIntPoint(FGenericPlatformMath::RoundToInt(CheckX + FloatRelativeLoc.X), FGenericPlatformMath::RoundToInt(CheckY + FloatRelativeLoc.Y));
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
		if (CheckX > ExplosionRadius)
		{
			CheckY += 1;
			CheckX = -ExplosionRadius;
		}
	}
	for (auto& i : LocationsToBeDestroyed)
	{
		DestroyPixel(i, true, true, WorldLocation, ExplosionRadius);
	}

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

//Comment -Mabel
FPartData UPartGridComponent::GetPartDataAtGridLocation(FIntPoint Location)
{
	return PartGrid.FindRef(Location);
}

//Comment -Mabel
FPartData UPartGridComponent::GetPartDataAtRelativeLocation(FVector Location)
{
	return GetPartDataAtGridLocation(FVector2D(Location).RoundToVector().IntPoint());
}

//Comment -Mabel
FPartData UPartGridComponent::GetPartDataAtWorldLocation(FVector Location)
{
	return GetPartDataAtRelativeLocation(Ship->GetActorQuat().UnrotateVector(Location - Ship->GetActorLocation()));
}

//Comment -Mabel
void UPartGridComponent::DistrubuteHeat()
{
	TMap<FIntPoint, float> NewHeatMap = TMap<FIntPoint, float>();
	NewHeatMap.Reserve(PartGrid.Num());
	for (int j = 0; j < PartGrid.Num(); j++)
	{
		float NewHeat = 0;
		for (int i = 0; i < 4; i++)
		{
			FIntPoint TargetPoint = ((i % 2 == 1) ? FIntPoint((i > 1) ? 1 : -1, 0) : FIntPoint(0, (i > 1) ? 1 : -1));
			
			if (PartGrid.Contains(TargetPoint + PartGrid.LocationAtIndex(j)))
			{
				NewHeat += PartGrid.FindRef(TargetPoint + PartGrid.LocationAtIndex(j)).GetTemperature() * HeatPropagationFactor / (4);
			}
		}
		NewHeat = PartGrid.ValueAtIndex(j).GetTemperature() * (1-HeatPropagationFactor) + NewHeat;
		NewHeatMap.Emplace(PartGrid.LocationAtIndex(j), NewHeat > .05 ? NewHeat : 0);
	}

	TArray<FIntPoint> KeysToDestroy = TArray<FIntPoint>();
	for (int i = 0; i < PartGrid.Num(); i++)
	{
		if (NewHeatMap.FindRef(PartGrid.LocationAtIndex(i)) > PartGrid.ValueAtIndex(i).Part->GetHeatResistance())
		{
			KeysToDestroy.Emplace(PartGrid.LocationAtIndex(i));
		}
		else
		{
			PartGrid.ValueAtIndex(i).SetTemperature(NewHeatMap.FindRef(PartGrid.LocationAtIndex(i)));
		}
	}
	for (FIntPoint Val : KeysToDestroy)
	{
		DestroyPixel(Val);
	}
}

bool UPartGridComponent::DestroyPixel(FIntPoint Location, class UBasePart*& DamagedPart, bool CheckForBreaks, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius)
{
	//Check if pixel is valid
	if (PartGrid.Contains(Location))
	{
		//Remove from grid
		DamagedPart = PartGrid.FindRef(Location).Part;
	}
	return DestroyPixel(Location, CheckForBreaks, FromExplosion, ExplosionLocation, ExplosionRadius);
}

//Comment -Mabel
void UPartGridComponent::BuildShip(TArray<FSavePartInfo> Parts)
{
	TArray<FIntPoint> AllParts = PartGrid.GetKeyArray();

	for (auto& i : AllParts)
	{
		RemovePart(i, false);
	}
	for (int i = 0; i < Parts.Num(); i++)
	{
		//Debug
		//UE_LOG(LogTemp, Warning, TEXT("build ship part class %s"), *Parts[i].PartClass.Get()->GetDisplayNameText().ToString())
		AddPart(Parts[i].PartClass, Parts[i].PartLocation, Parts[i].PartRotation, false);
	}
}

//Comment -Mabel
void UPartGridComponent::SaveShip(FString ShipName)
{
	
	TArray<FPartData> Parts = PartGrid.GetValueArray();

	
	
	USaveGame* SaveGameInstance = UGameplayStatics::CreateSaveGameObject(USaveShip::StaticClass());

	for (int i = 0; i < Parts.Num(); i++)
	{
		Cast<USaveShip>(SaveGameInstance)->SavedShip.Add(FSavePartInfo(Parts[i].Part->GetClass(), Parts[i].Part->GetPartGridLocation(), Parts[i].Part->GetRelativeRotation()));
	}
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, ShipName, 0);

}

//Comment -Mabel
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
const FVector2D UPartGridComponent::CalcCenterOfMass()
{
	//Initalize Variables
	FVector2D Center = FVector2D(0,0);
	float Mass = CalcMass();
	//UE_LOG(LogTemp, Warning, TEXT("PRE cofmass?? x=%f, y=%f, Mass=%f"), Center.X, Center.Y, Mass);

	//Iterate though Pixels and adjust center of mass
	for (int i = 0; i < PartGrid.Num(); i++)
	{
		Center += FVector2D(PartGrid.LocationAtIndex(i)) * PartGrid.ValueAtIndex(i).Part->GetMass() / Mass;
	}
	//UE_LOG(LogTemp, Warning, TEXT("cofmass?? x=%f, y=%f"), Center.X, Center.Y);
	
	
	return Center;
}

//Comment -Mabel
const float UPartGridComponent::CalcMomentOfInertia()
{
	float ReturnValue = 0;
	FVector2D CenterOfMass = Ship->PhysicsComponent->GetCenterOfMass();
	for (int i = 0; i < PartGrid.Num(); i++)
	{
		float PartMass = PartGrid.ValueAtIndex(i).Part->GetMass();
		ReturnValue += (1 / 12) + PartMass * (FVector2D(PartGrid.ValueAtIndex(i).Part->GetPartRelativeLocation())).SizeSquared();
	}
	return ReturnValue;
}

//Calculates the mass of the PartGrid
const float UPartGridComponent::CalcMass()
{
	//Initialize Variable
	float Mass = 0;

	//Interate though Pixels and summate their mass
	for (int i = 0; i < PartGrid.Num(); i++)
	{
		Mass += PartGrid.ValueAtIndex(i).Part->GetMass();
	}
	//UE_LOG(LogTemp, Warning, TEXT("other mass = %f"), Mass);


	return Mass == 0 ? 1 : Mass;
}

//Gets the PartGrid Map
TGridMap<FPartData> UPartGridComponent::GetPartGrid()
{
	return PartGrid;
}

//Comment -Mabel
const float UPartGridComponent::GetPartGridScale()
{
	return GridScale;
}

//Comment -Mabel
const FArrayBounds UPartGridComponent::GetPartGridBounds()
{
	return GridBounds;
}

//Comment -Mabel
void UPartGridComponent::UpdateMaterials(FIntPoint Location, TSubclassOf<UBasePart> PartType)
{
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (FIntPoint(i + Location.X, j + Location.Y) != Location)
			{
				if (PartGrid.Contains(FIntPoint(i + Location.X, j + Location.Y)))
				{
					if (PartGrid.Contains(FIntPoint(Location)) && PartGrid.Find(FIntPoint(i + Location.X, j + Location.Y))->Part->GetClass() == PartType)
					{
						PartGrid.FindRef(FIntPoint(i + Location.X, j + Location.Y)).SetAdjacencyIndex(UFunctionLibrary::GetBitNumberFromLocation(FIntPoint(i * -1, j * -1)) + PartGrid.FindRef(FIntPoint(i + Location.X, j + Location.Y)).GetAdjacencyIndex());
					}
					else if (PartGrid.Find(FIntPoint(i + Location.X, j + Location.Y))->Part->GetClass() == PartType)
					{
						PartGrid.FindRef(FIntPoint(i + Location.X, j + Location.Y)).SetAdjacencyIndex(PartGrid.FindRef(FIntPoint(i + Location.X, j + Location.Y)).GetAdjacencyIndex() - UFunctionLibrary::GetBitNumberFromLocation(FIntPoint(i * -1, j * -1)));
					}
				}
			}
		}
	}
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

//Returns true if StartPoint and EndPoint are connected via PartGrid
bool UPartGridComponent::PointsConnected(FIntPoint StartPoint, FIntPoint EndPoint, bool TestForFunctionality)
{
	if (TestForFunctionality)
	{
		return PartGrid.PointsConnected(StartPoint, EndPoint, IsPixelFunctional);
	}
	else
	{
		return PartGrid.PointsConnected(StartPoint, EndPoint, AlwaysConnect<FPartData>);
	}
}

//Comment -Mabel
TArray<FIntPoint> UPartGridComponent::FindConnectedShape(TArray<FIntPoint> Shape, TGridMap<FPartData> ConnectedPartsMap, bool CheckFunctionality)
{

	//New shape will return the entire connected shape, indcluding the starting shape
	TArray<FIntPoint> NewShape = Shape;

	for (auto& i : Shape)
	{
		//If the new shape does NOT contain the checked location
		if (!NewShape.Contains(FIntPoint(i.X + 1, i.Y)))
		{
			//And the connected parts ARE at that location
			if (ConnectedPartsMap.Contains(FIntPoint(i.X + 1, i.Y)))
			{
				if (CheckFunctionality)
				{
					//And the pixel at that location is functional
					if (ConnectedPartsMap.Find(FIntPoint(i.X - 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X + 1, i.Y)))
					{
						//Add that location to the new shape, because it is connected
						NewShape.Emplace(FIntPoint(i.X + 1, i.Y));
					}
				}
				else
				{
					NewShape.Emplace(FIntPoint(i.X + 1, i.Y));
				}
			}
		}

		//Do the same thing done for X + 1 for X - 1
		if (!NewShape.Contains(FIntPoint(i.X - 1, i.Y)))
		{
			if (ConnectedPartsMap.Contains(FIntPoint(i.X - 1, i.Y)))
			{
				if (CheckFunctionality)
				{
					if (ConnectedPartsMap.Find(FIntPoint(i.X - 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X - 1, i.Y)))
					{
						NewShape.Emplace(FIntPoint(i.X - 1, i.Y));
					}
				}
				else
				{
					NewShape.Emplace(FIntPoint(i.X - 1, i.Y));
				}
			}
		}

		//Do the same thing done for X + 1 for Y + 1
		if (!NewShape.Contains(FIntPoint(i.X, i.Y + 1)))
		{
			if (ConnectedPartsMap.Contains(FIntPoint(i.X, i.Y + 1)))
			{
				if (CheckFunctionality)
				{
					if (ConnectedPartsMap.Find(FIntPoint(i.X - 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X, i.Y + 1)))
					{
						NewShape.Emplace(FIntPoint(i.X, i.Y + 1));
					}
				}
				else
				{
					NewShape.Emplace(FIntPoint(i.X, i.Y + 1));
				}
			}
		}

		//Do the same thing done for X + 1 for Y - 1
		if (!NewShape.Contains(FIntPoint(i.X, i.Y - 1)))
		{
			if (ConnectedPartsMap.Contains(FIntPoint(i.X, i.Y - 1)))
			{
				if (CheckFunctionality)
				{
					if (ConnectedPartsMap.Find(FIntPoint(i.X - 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X, i.Y - 1)))
					{
						NewShape.Emplace(FIntPoint(i.X, i.Y - 1));
					}
				}
				else
				{
					NewShape.Emplace(FIntPoint(i.X, i.Y - 1));
				}
			}
		}
	}

	//If the new shape has changed at all
	if (NewShape != Shape)
	{
		//Continue to check for connections by calling the function recursively.
		NewShape = FindConnectedShape(NewShape, ConnectedPartsMap, CheckFunctionality);
	}

	//Once everything has figured itself out, return the New Shape
	return NewShape;
}

//Comment -Mabel
bool UPartGridComponent::IsPixelFunctional(FPartData PixelValue, FIntPoint Loc)
{
	return PixelValue.Part->IsPixelFunctional(Loc);
}
