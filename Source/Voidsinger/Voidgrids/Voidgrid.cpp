// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidgrid.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ThrustManager.h"
#include "DrawDebugHelpers.h"
#include "Parts/Part.h"

//Sets default values for this voidgrid's properties
AVoidgrid::AVoidgrid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initilize Root Component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CenterOfMass"));

	// \/ Initialize Mesh Component \/ /
	PixelMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh Component"));
	PixelMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PixelMeshComponent->bUseComplexAsSimpleCollision = true;
	PixelMeshComponent->bRenderCustomDepth = true;
	PixelMeshComponent->SetCollisionObjectType(VoidgridCollsionChanel);

	PixelUVs = TArray<FVector2D>();
	PixelUVs.Emplace(FVector2D(1, 1)); //Bottom Right
	PixelUVs.Emplace(FVector2D(1, 1)); //Bottom Right
	PixelUVs.Emplace(FVector2D(1, 0)); //Top Right
	PixelUVs.Emplace(FVector2D(1, 0)); //Top Right
	PixelUVs.Emplace(FVector2D(0, 1)); //Bottom Left
	PixelUVs.Emplace(FVector2D(0, 1)); //Bottom Left
	PixelUVs.Emplace(FVector2D(0, 0)); //Top Left
	PixelUVs.Emplace(FVector2D(0, 0)); //Top Left

	PixelTriangles = TArray<int32>();
	PixelTriangles += CreateTrianglesForPixelMeshFace(0, 2, 4, 6); //Top Face
	PixelTriangles += CreateTrianglesForPixelMeshFace(2, 0, 3, 1); //Side Face for collision
	PixelTriangles += CreateTrianglesForPixelMeshFace(0, 4, 1, 5); //Side Face for collision
	PixelTriangles += CreateTrianglesForPixelMeshFace(4, 6, 5, 7); //Side Face for collision
	PixelTriangles += CreateTrianglesForPixelMeshFace(6, 2, 7, 3); //Side Face for collision
	// /\ Initialize Mesh Component /\ /

	//Initialize Thruster Manager
	if (IsValid(ThrustManagerClass))
	{
		ThrustManager = NewObject<UThrustManager>(this, ThrustManagerClass.Get());
	}
}

/*
 * Used to update location, thrust control, heat spread, and resources.
 * 
 * @param DeltaTime - The time between ticks
 */
void AVoidgrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTransform(DeltaTime);

	DeltaHeatTime += DeltaTime;
	
	//Find how many heat ticks have passed and call SpreadHeat that number of times
	for (int EachHeatTickPassed = 0; EachHeatTickPassed < DeltaHeatTime / HeatTick; DeltaHeatTime -= HeatTick)
	{
		SpreadHeat();
		DeltaHeatTime = 0;
	}	
	DeltaHeatTime += DeltaTime;

	HandleResourceRequests();
}

/* ------------- *\
\* \/ Physics \/ */

// \/ Add Impulse \/ 
/**
 * Pushes this voidgrid in the direction of Impulse with the force of |Impulse|.
 *
 * @param RelativeImpulse - The impluse to apply to this voidgrid in relative space.
 * @param GridImpulseLocation - The location on the part grid to apply the impulse at.
 */
void AVoidgrid::AddImpulse(FVector2D RelativeImpulse, GridLocationType GridImpulseLocation)
{
	FVector2D WorldImpulse = FVector2D(GetActorRotation().RotateVector(FVector(RelativeImpulse, 0)));
	//Clamp new velocity within MaxLinearVelocity
	//                            | -------- Get New Velocity -------- |
	LinearVelocity = FMath::Clamp(LinearVelocity + (WorldImpulse / Mass), FVector2D(-1 * MaxLinearVelocity), FVector2D(MaxLinearVelocity));

	FVector2D RelativeImpulseLocation = FVector2D(GridImpulseLocation) + FVector2D(PixelMeshComponent->GetRelativeLocation());
	//Clamp new velocity within MaxAngualarVelocity
	//                             | --------------------------------------------------------------------- Get New Velocity --------------------------------------------------------------------- |
	AngularVelocity = FMath::Clamp(AngularVelocity + FVector2D::CrossProduct(RelativeImpulseLocation, WorldImpulse) / MomentOfInertia, -1 * MaxAngularVelocity, MaxAngularVelocity);
}

/**
 * Pushes this voidgrid in the direction of Impulse with the force of |Impulse|.
 *
 * @param Impulse - The impluse to apply to this voidgrid in world space.
 * @param WorldImpulseLocation - The location in world space to apply the impulse at.
 */
void AVoidgrid::AddImpulse(FVector2D Impulse, FVector WorldImpulseLocation)
{
	//Clamp new velocity within MaxLinearVelocity
	//                            | -------- Get New Velocity -------- |
	LinearVelocity = FMath::Clamp(LinearVelocity + (Impulse / Mass), FVector2D(-1 * MaxLinearVelocity), FVector2D(MaxLinearVelocity));

	FVector2D RelativeImpulseLocation = FVector2D(GetActorTransform().InverseTransformPosition(WorldImpulseLocation));
	//Clamp new velocity within MaxAngualarVelocity
	//                             | --------------------------------------------------------------------- Get New Velocity --------------------------------------------------------------------- |
	AngularVelocity = FMath::Clamp(AngularVelocity + FVector2D::CrossProduct(RelativeImpulseLocation, Impulse) / MomentOfInertia, -1 * MaxAngularVelocity, MaxAngularVelocity);
}
// /\ Add Impulse /\ 

/**
 * Gets the instantaneous linear velocity of a point on this Voidgrid in world space.
 *
 * @param Location - The location of the point to get the velocity of.
 */
FVector2D AVoidgrid::GetVelocityOfPoint(FVector2D Location)
{
	return LinearVelocity + FVector2D(-1, 1) * Location * AngularVelocity;
}

/**
* Updates the voidgrids location and rotation by its velocity. Also sweeps for collisions and computes new velocities
*/
void AVoidgrid::UpdateTransform(float DeltaTime)
{
	//                                     | -------------- Get Delta Roatation -------------- | | ------------------ Get Delta Location ------------------ |
	FTransform DeltaTransform = FTransform(FQuat(FVector(0, 0, 1), AngularVelocity * DeltaTime), FVector(LinearVelocity * DeltaTime, 0), FVector::ZeroVector);

	FHitResult SweepHitResult = FHitResult();

	// \/ Detect collsion and update velocity acordingly \/ //
	if (SweepShip(DeltaTransform, SweepHitResult))
	{
		FVector2D RelativeHitLocation = FVector2D(SweepHitResult.Location - GetActorLocation());

		FVector2D ImpactNormal = FVector2D(SweepHitResult.ImpactNormal);

		//Prevent collisions from occuring between already overlaping ships. Done by checking to see if the impact direction is in a oposite direction to the normal.
		if ((GetVelocityOfPoint(RelativeHitLocation) | ImpactNormal) < 0)
		{
			AVoidgrid* OtherVoidgrid = Cast<AVoidgrid>(SweepHitResult.GetActor());

			float CollsionForce;

			if (IsValid(OtherVoidgrid))
			{
				FVector2D OtherRelativeHitLocation = FVector2D(SweepHitResult.Location - OtherVoidgrid->GetActorLocation());

				CollsionForce = (-1 * (1 + CollisionElasticity) * (GetVelocityOfPoint(RelativeHitLocation) | ImpactNormal)) /
					(1 / Mass + 1 / OtherVoidgrid->Mass + FMath::Square(RelativeHitLocation ^ ImpactNormal) / MomentOfInertia + FMath::Square(OtherRelativeHitLocation ^ ImpactNormal) / OtherVoidgrid->MomentOfInertia);

				OtherVoidgrid->AddImpulse(-1 * CollsionForce * ImpactNormal, FVector(OtherRelativeHitLocation, 0));
			}
			else
			{
				CollsionForce = (-1 * (1 + CollisionElasticity) * GetVelocityOfPoint(RelativeHitLocation) | ImpactNormal) /
					(1 / Mass) + (FMath::Square(RelativeHitLocation ^ ImpactNormal) / MomentOfInertia);
			}

			AddImpulse(CollsionForce * ImpactNormal, FVector(RelativeHitLocation, 0));
			//DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation() + FVector(RelativeHitLocation, 0), GetOwner()->GetActorLocation() + FVector(RelativeHitLocation, 0) + FVector(CollisionImpulseFactor * ImpactNormal, 0), 5, FColor::Blue, false, 5, 0U, 0.3f);
			////UE_LOG(LogTemp, Warning, TEXT("%s Applyed an impules of %s at %s to itself when colideing with %s"), *GetOwner()->GetName(), *(CollisionImpulseFactor * ImpactNormal).ToString(), *RelativeHitLocation.ToString(), *Result.GetActor()->GetName());
		}
	}
	// /\ Detect collsion and update velocity acordingly /\ //

	AddActorWorldTransform(DeltaTransform);
}

/**
 * Checks for collisions along this ships path to a new transfrom.
 * 
 * @param DeltaTransform - The change in transform needed to get to the new transform.
 * @param Hit - The first blocking hit from this voidgrid colliding with another voidgrid.
 * @return Whether or not this voidgrid collided with another voidgrid.
 */
bool AVoidgrid::SweepShip(const FTransform & DeltaTransform, FHitResult & Hit)
{
	//Return Values
	bool ReturnValue = false;
	Hit = FHitResult();

	
	FTransform StartingTransform = GetActorTransform();
	FTransform TargetTransfrom = GetActorTransform() + DeltaTransform;

	//Set up convex sweep querry
	FVector BoundsExtent = PixelMeshComponent->Bounds.BoxExtent;
	FVector BoundsCenterLocation = PixelMeshComponent->GetRelativeLocation();

	FCollisionQueryParams QueryParams = FCollisionQueryParams().DefaultQueryParam;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	FCollisionObjectQueryParams ObjectQueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);

	//Sweeps the convex bounds of this shape to determin if a hit is possible.
	if (GetWorld()->SweepSingleByObjectType(Hit, StartingTransform.GetTranslation() + BoundsCenterLocation, TargetTransfrom.GetTranslation() + BoundsCenterLocation, TargetTransfrom.GetRotation(), ObjectQueryParams, FCollisionShape::MakeBox(BoundsExtent), QueryParams))
	{
		//Iterates though all pixels and sweeps them for a collsion.
		for (TPair<GridLocationType, PixelType> PixelData : LocationsToPixelState.GetGridPairs())
		{
			FVector PixelRelativeLocation = FVector(FVector2D(PixelData.Key), 0);
			FVector PixelStartingWorldLocation = StartingTransform.TransformVector(FVector(PixelRelativeLocation));
			FVector PixelTargetWorldLocation = TargetTransfrom.TransformVector(FVector(PixelRelativeLocation));
			FHitResult ThisHit = FHitResult();

			//Sweep the pixel's shape
			if (GetWorld()->SweepSingleByObjectType(ThisHit, PixelStartingWorldLocation, PixelTargetWorldLocation, TargetTransfrom.GetRotation(), ObjectQueryParams, FCollisionShape::MakeBox(FVector(0.5f)), QueryParams))
			{
				if (Hit.bBlockingHit && Hit.Time != 0 && ThisHit.Time < Hit.Time)
				{
					ReturnValue = true;

					Hit = ThisHit;
				}
			}
		}
	}


	return ReturnValue;
}

/**
* Updates Mass, CenterOfMass, MomentOfInertia
*/
void AVoidgrid::UpdateMassProperties(float DeltaMass, FVector2D MassLocation)
{
	if (DeltaMass != 0)
	{
		CenterOfMass = DeltaMass / (Mass + DeltaMass)  * MassLocation + Mass / (Mass + DeltaMass) * CenterOfMass;
		Mass += DeltaMass;
		AddActorLocalOffset(PixelMeshComponent->GetRelativeLocation() + FVector(CenterOfMass, 0));
		PixelMeshComponent->SetRelativeLocation(FVector(-1 * CenterOfMass, 0));
		MomentOfInertia += (1 / 12) + DeltaMass * (MassLocation).SizeSquared();
		OnMassChanged.Broadcast(Mass, CenterOfMass, MomentOfInertia);
	}
}

/* /\ Physics /\ *\
\* ------------- */

/* ----------------- *\
\* \/ Temperature \/ */

// \/ Add temperature \/ /

/**
 * Applys the temperature given at the world location given
 *
 * @param Location - The world location to apply the temperature
 * @param Temperature - The temperature to add
 */
void AVoidgrid::AddTemperatureAtLocation(FVector WorldLocation, float Temperature)
{
	FIntPoint RelativeLocation = TransformWorldToGrid(WorldLocation);
	AddTemperatureAtLocation(RelativeLocation, Temperature);
}

/**
 * Applys the temperature given at the location given on this Voidgrid
 *
 * @param Location - The location to apply the temperature
 * @param Temperature - The temperature to add
 */
void AVoidgrid::AddTemperatureAtLocation(FIntPoint Location, float Temperature)
{
	if (LocationsToPixelState.Contains(Location))
	{
		LocationsToPixelState.Find(Location)->AddTemperature(Temperature);
	}
}

// /\ Add temperature /\ /

/**
 * Spreads the heat on the Voidgrid
 */
void AVoidgrid::SpreadHeat()
{
	
	// \/ Calculate the new heat map \/ //

	TMap<FIntPoint, float> NewHeatMap = TMap<FIntPoint, float>();
	NewHeatMap.Reserve(LocationsToPixelState.Num());

	//Iterate through the locations pending heat transfer and transfer their heat
	for (TPair<FIntPoint, float> EachLocationToTemperaturePendingHeatTransfer : LocationsToTemperaturesPendingHeatTransfer)
	{
		for (int EachLocationAround = 0; EachLocationAround < 4; EachLocationAround++)
		{
		FIntPoint LocationAround;

		switch (EachLocationAround)
		{
		case 0:
			LocationAround = FIntPoint(1, 0);
			break;
		case 1:
			LocationAround = FIntPoint(0, 1);
			break;
		case 2:
			LocationAround = FIntPoint(-1, 0);
			break;
		case 3:
			LocationAround = FIntPoint(0, -1);
			break;
		}

		NewHeatMap.Emplace(EachLocationToTemperaturePendingHeatTransfer.Key + LocationAround, (EachLocationToTemperaturePendingHeatTransfer.Value * TemperaturePropagationFactor) / 4.0f);
		}
	}

	LocationsToTemperaturesPendingHeatTransfer.Empty();

	//Iterate through each point on the grid map to spread heat to each pixel
	for (TPair<FIntPoint, PixelType> EachPixel : LocationsToPixelState.GetGridPairs())
	{
		//Heat added to this pixel
		float AddedHeat = 0;

		// Spread heat to each point around the current location
		for (int EachPointAroundPixel = 0; EachPointAroundPixel < 4; EachPointAroundPixel++)
		{
			FIntPoint TargetPoint = ((EachPointAroundPixel % 2 == 1) ? FIntPoint((EachPointAroundPixel > 1) ? 1 : -1, 0) : FIntPoint(0, (EachPointAroundPixel > 1) ? 1 : -1));

			if (LocationsToPixelState.Contains(TargetPoint + EachPixel.Key))
			{

				//Get the temperature of the adjacent pixel
				float OtherPixelTemperature = LocationsToPixelState.Find(TargetPoint + EachPixel.Key)->GetTemperature();

				//The heat added to this pixel from the other pixel is the other pixel's temperature multiplied by the heat propagation faction (how much heat it will spread to other pixels). It
				//is divided by four because a pixel will spread heat to four other pixels.
				AddedHeat += (OtherPixelTemperature * TemperaturePropagationFactor) / (4);
			}
		}

		//The heat remaining when this pixel spreads heat to the surrounding pixels
		float RemainingHeat = EachPixel.Value.GetTemperature() * (1 - TemperaturePropagationFactor);

		//									     	|-- If the new heat map contains this location then add that temperature in too --|
		float NewHeat = RemainingHeat + AddedHeat + (NewHeatMap.Contains(EachPixel.Key) ? NewHeatMap.FindRef(EachPixel.Key) : 0);

		//If the amount of heat is within the negligable temperature amount, then it's negligable. 
		NewHeatMap.Emplace(EachPixel.Key, ((NewHeat > NegligableTemperatureAmount) || (NewHeat < -NegligableTemperatureAmount)) ? NewHeat : 0);

	}

	// /\ Calculate the new heat map /\ //

	// \/ Set the temperature of each pixel and find whether it should be melted or frozen \/ //

	for (TPair<FIntPoint, PixelType> EachPixel : LocationsToPixelState.GetGridPairs())
	{
		if (NewHeatMap.Contains(EachPixel.Key) && EachPixel.Value.IsIntact())
		{
			LocationsToPixelState.Find(EachPixel.Key)->SetTemperature(NewHeatMap.FindRef(EachPixel.Key));

			//Melt pixel
			if (NewHeatMap.FindRef(EachPixel.Key) > EachPixel.Value.GetCurrentPart()->GetData()->HeatResistance)
			{
				RemovePixel(EachPixel.Key);
			}
			else
			{
				//Freeze pixel
				if (NewHeatMap.FindRef(EachPixel.Key) < -1 * EachPixel.Value.GetCurrentPart()->GetData()->HeatResistance)
				{
					LocationsToPixelState.Find(EachPixel.Key)->GetCurrentPart()->SetPixelFrozen(EachPixel.Key, true);
				}
				//Unfreeze pixel
				else
				{
					LocationsToPixelState.Find(EachPixel.Key)->GetCurrentPart()->SetPixelFrozen(EachPixel.Key, false);
				}
			}
		}
	}

	// /\ Set the temperature of each pixel and find whether it should be melted or frozen /\ /
}

/* /\ Temperature /\ *\
\* ----------------- */

/* ---------------- *\
\* \/ Pixel Mold \/ */

/**
 * Gets the grid loction of a world loction.
 *
 * @param WorldLocation - The world location to transform.
 * @return The grid loction of WorldLocation;
 */
UFUNCTION(BlueprintPure)
FIntPoint AVoidgrid::TransformWorldToGrid(FVector WorldLocation) const
{
	return (FVector2D(GetTransform().InverseTransformPosition(WorldLocation)) + CenterOfMass).IntPoint();
}

/**
 * Gets the world location of a grid loction.
 *
 * @param GridLoction - The grid location to transform.
 * @return The world loction of GridLoction;
 */
UFUNCTION(BlueprintPure)
FVector AVoidgrid::TransformGridToWorld(FIntPoint GridLocation) const
{
	return GetTransform().TransformPosition(FVector(FVector2D(GridLocation) - CenterOfMass, 0));
}

/**
 * Sets the pixel mold of the ship
 *
 * @param NewPixelMold - The value to assign to the pixel mold of the ship
 */
void AVoidgrid::SetPixelMold(TSet<FMinimalPartInstanceData> NewPixelMold)
{
	TargetParts = NewPixelMold;
	TSet<FMinimalPartInstanceData> DataOfPartsToCreate = NewPixelMold;

	//Remove Unneccesary Parts
	TSet<UPart*> PartsCopy = Parts;
	for (UPart* Part : PartsCopy)
	{
		FMinimalPartInstanceData PartData = Part->GetMinimalPartInstanceData();

		if (NewPixelMold.Contains(PartData))
		{
			DataOfPartsToCreate.Remove(PartData);
		}
		else
		{
			for (GridLocationType PartPixelLocation : Part->GetTransform().TransformPartShape(Part->GetDefaultShape()))
			{
				SetPixelTarget(PartPixelLocation, UPart::GetNullPart());
			}
		}
	}

	//Re-add temprary parts if inlcuded in a new mold
	TSet<UPart*> TemporaryPartsCopy = TemporaryParts;
	for (UPart* Part : TemporaryPartsCopy)
	{
		FMinimalPartInstanceData PartData = Part->GetMinimalPartInstanceData();

		if (NewPixelMold.Contains(PartData))
		{
			DataOfPartsToCreate.Remove(PartData);

			for (GridLocationType PartPixelLocation : Part->GetTransform().TransformPartShape(Part->GetDefaultShape()))
			{
				SetPixelTarget(PartPixelLocation, Part);
			}
		}			
	}

	//Create new parts
	for (FMinimalPartInstanceData DataOfPartToCreate : DataOfPartsToCreate)
	{
		if (IsValid(DataOfPartToCreate.Data))
		{
			UPart* Part = UPart::CreatePart(this, FPartInstanceData(DataOfPartToCreate));
			Parts.Add(Part);

			for (GridLocationType ShapeComponent : DataOfPartToCreate.Data->Shape)
			{
				SetPixelTarget(Part->GetTransform().TransformGridLocation(ShapeComponent), Part);
			}
		}
	}
}

/**
 * Gets the minimal part data for all parts on this voidgrid.
 *
 * @return The minimal part data for all parts on this voidgrid.
 */
MinimalPixelMoldDataType AVoidgrid::GetPixelMold()
{
	MinimalPixelMoldDataType AllPartsData = MinimalPixelMoldDataType();
	for (UPart* Part : Parts)
	{
		AllPartsData.Add(Part->GetMinimalPartInstanceData());
	}
	return AllPartsData;
}

/**
 * Sets the state of this voidgrid.
 *
 * @param NewState - The state to make this voidgrid match.
 */
void AVoidgrid::SetState(FVoidgridState NewState)
{
	ClearVoidgrid();
	SetPixelMold(NewState.Mold);

	for (FPartInstanceData PartState : NewState.State)
	{
		if (!NewState.Mold.Contains(PartState.GetMinimalInstanceData()))
		{
			if (!PartState.GetShape().IsEmpty())
			{
				UPart* CurrentPart = UPart::CreatePart(this, PartState.GetMinimalInstanceData());
				TemporaryParts.Add(CurrentPart);

				for (GridLocationType ShapeComponentLocation : PartState.GetShape())
				{
					GridLocationType NewPixelLocation = CurrentPart->GetTransform().TransformGridLocation(ShapeComponentLocation);
					if (!LocationsToPixelState.Contains(NewPixelLocation))
					{
						LocationsToPixelState.Emplace(NewPixelLocation, FGridPixelData(CurrentPart, UPart::GetNullPart()));
					}
					else
					{
						LocationsToPixelState.Find(NewPixelLocation)->SetCurrentPart(CurrentPart);
					}

					MutablePixels.Add(NewPixelLocation);
					SetPixelIntact(NewPixelLocation, true, false);
				}
			}
		}
		else
		{
			for (GridLocationType ShapeComponentLocation : PartState.GetShape())
			{
				SetPixelIntact(PartState.GetTransform().TransformGridLocation(ShapeComponentLocation), true, false);
			}
		}
	}
}

/**
 * Gets the state of this voidgrid.
 *
 * @return The state of this voidgrid.
 */
FVoidgridState AVoidgrid::GetState()
{
	TSet<FPartInstanceData> State = TSet<FPartInstanceData>();
	TSet<FMinimalPartInstanceData> Mold = TargetParts;
	for (UPart* Part : Parts)
	{
		Mold.Add(Part->GetMinimalPartInstanceData());
		if (Part->GetPartInstanceData().GetShape().Num() > 0)
		{
			State.Add(Part->GetPartInstanceData());
		}
	}

	for (UPart* Part : TemporaryParts)
	{
		if (Part->GetPartInstanceData().GetShape().Num() > 0)
		{
			State.Add(Part->GetPartInstanceData());
		}
	}
	

	return FVoidgridState(Mold, State);
}

/**
 * Damages a pixel.
 *
 * @param Location - The location of the pixel to damage.
 */
void AVoidgrid::RemovePixel(GridLocationType Location)
{
	LocationsToTemperaturesPendingHeatTransfer.Emplace(Location, LocationsToPixelState.Find(Location)->GetTemperature());

	SetPixelIntact(Location, false);
}

/**
 * Repair a pixel.
 *
 * @param Location - The location of the pixel to repair.
 */
void AVoidgrid::RepairPixel(GridLocationType Location)
{
	if (LocationsToPixelState.Contains(Location))
	{
		PixelType* PixelRef = LocationsToPixelState.Find(Location);
		if (!PixelRef->IsIntact())
		{
			SetPixelIntact(Location, true);
		}
		else if(MutablePixels.Contains(Location))
		{
			SetPixelIntact(Location, false, false);
		}
	}
}

/**
 * Repairs a random pixel pixel.
 */
void AVoidgrid::RepairPixel()
{
	if (MutablePixels.Num() != 0)
	{
		RepairPixel(MutablePixels.Array()[FMath::RandRange(0, MutablePixels.Num() - 1)]);
	}
}

/**
 * Set pixel intact
 *
 * @param Location - The location of the pixel to edit.
 * @param bNewIntact - The new integrity of the pixel.
 */
void AVoidgrid::SetPixelIntact(GridLocationType Location, bool bNewIntact, bool bApplyChangeEffect)
{
	if (LocationsToPixelState.Contains(Location))
	{
		if (LocationsToPixelState.Find(Location)->IsIntact() != bNewIntact)
		{
			if (!bNewIntact) 
			{
				UPart* OldPart = LocationsToPixelState.Find(Location)->GetCurrentPart();
				float OldPixelMass = OldPart->GetPixelMass();
				if (LocationsToPixelState.Find(Location)->GetTargetPart() == UPart::GetNullPart())
				{
					MutablePixels.Remove(Location);

					LocationsToPixelState.Remove(Location);
				}
				else
				{
					MutablePixels.Add(Location);
					LocationsToPixelState.Find(Location)->SetIntact(bNewIntact);
				}

				UpdateMassProperties(-1 * OldPixelMass, FVector2D(Location));
				OnPixelRemoved.Broadcast(Location, bApplyChangeEffect, OldPart);

				if (OldPart->GetShape().Num() <= 1)
				{
					TemporaryParts.Remove(OldPart);
				}
			}
			else
			{
				if (LocationsToPixelState.Find(Location)->GetCurrentPart() == UPart::GetNullPart())
				{
					UE_LOG(LogTemp, Error, TEXT("Set Null part to intact at %s"), *Location.ToString());
				}

				LocationsToPixelState.Find(Location)->SetIntact(bNewIntact);
				if (LocationsToPixelState.Find(Location)->IsTargetPart())
				{
					MutablePixels.Remove(Location);
				}

				OnPixelAdded.Broadcast(Location, bApplyChangeEffect, LocationsToPixelState.Find(Location)->GetCurrentPart());
				UpdateMassProperties(LocationsToPixelState.Find(Location)->GetCurrentPart()->GetPixelMass(), FVector2D(Location));
			}
			UpdatePixelMesh(Location);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Set intact to %i when already in that state at %s"), bNewIntact ? 1 : 0, *Location.ToString());
		}
	}
}

/**
 * Set pixel target
 *
 * @param Location - The location of the pixel to edit.
 * @param NewTarget - The new target of the pixel.
 */
void AVoidgrid::SetPixelTarget(GridLocationType Location, UPart* NewTarget)
{
	if (LocationsToPixelState.Contains(Location))
	{
		//Set target part if target part has changed
		if (NewTarget != LocationsToPixelState.Find(Location)->GetTargetPart())
		{
			//Unmark pixel as temporary  if pixel is already the target part.
			if (NewTarget == LocationsToPixelState.Find(Location)->GetCurrentPart() && NewTarget != UPart::GetNullPart())
			{
				if (LocationsToPixelState.Find(Location)->IsIntact())
				{
					MutablePixels.Remove(Location);
				}
				else
				{
					MutablePixels.Add(Location);
				}

				Parts.Add(LocationsToPixelState.Find(Location)->GetCurrentPart());
				TemporaryParts.Remove(LocationsToPixelState.Find(Location)->GetCurrentPart());

				LocationsToPixelState.Find(Location)->SetTargetPart(NewTarget);
			}
			//Mark pixel as temporary if pixel needs to change to become the new part.
			else if(NewTarget != UPart::GetNullPart() || LocationsToPixelState.Find(Location)->IsIntact())
			{
				MutablePixels.Add(Location);
				TemporaryParts.Add(LocationsToPixelState.Find(Location)->GetCurrentPart());
				Parts.Remove(LocationsToPixelState.Find(Location)->GetCurrentPart());
				LocationsToPixelState.Find(Location)->SetTargetPart(NewTarget);
			}
			//Edge case for removeing uneeded NullPart pixels
			else
			{
				if (LocationsToPixelState.Find(Location)->GetCurrentPart()->GetShape().IsEmpty())
				{
					TemporaryParts.Remove(LocationsToPixelState.Find(Location)->GetCurrentPart());
				}

				MutablePixels.Remove(Location);
				Parts.Remove(LocationsToPixelState.Find(Location)->GetCurrentPart());
				LocationsToPixelState.Remove(Location);
			}
		}
	}
	//Create new pixel data if pixel does not exist.
	else if (NewTarget != UPart::GetNullPart())
	{
		LocationsToPixelState.Emplace(Location, FGridPixelData(NewTarget));
		MutablePixels.Add(Location);
	}
}

/**
 * Removes all parts from this voidgrid.
 * Does not call OnDamaged.
 */
void AVoidgrid::ClearVoidgrid()
{
	TMap<GridLocationType, PixelType> GridPairs = LocationsToPixelState.GetGridPairs();
	for (TPair<GridLocationType, PixelType> Pair : GridPairs)
	{
		SetPixelIntact(Pair.Key, false, false);
	}
}

/* /\ Pixel Mold /\ *\
\* ---------------- */

/* ---------------- *\
\* \/ Pixel Mesh \/ */

/**
 * Updates a mesh segment for a pixel.
 *
 * @param Location - The location of the pixel to update the mesh of.
 */
void AVoidgrid::UpdatePixelMesh(GridLocationType Location)
{
	if (LocationsToPixelState.Contains(Location))
	{
		if (LocationsToPixelState.Find(Location)->IsIntact())
		{
			if (PixelMeshSegmentIndices.Contains(Location))
			{
				SetPixelMeshVisibility(Location, true);
			}
			else
			{
				AddPixelMesh(Location);
			}

			if ((LocationsToPixelState.Find(Location)->GetMaterial()->IsValidLowLevel()))
			{
				PixelMeshComponent->SetMaterial(PixelMeshSegmentIndices.FindRef(Location), LocationsToPixelState.Find(Location)->GetMaterial());
			}
		}
		else
		{
			SetPixelMeshVisibility(Location, false);
		}
	}
	else
	{
		RemovePixelMesh(Location);
	}
}

/**
 * Creates a mesh segment for a pixel.
 *
 * @param Location - The location of the pixel to create a mesh segment for.
 */
void AVoidgrid::AddPixelMesh(GridLocationType Location)
{
	//If this pixel already has a mesh set the index to that 
	int32 SectionIndex = PixelMeshSegmentIndices.Contains(Location) ? PixelMeshSegmentIndices.FindRef(Location) : MeshSectionCounter;

	PixelMeshComponent->CreateMeshSection(SectionIndex, GetPixelVertices(Location), PixelTriangles, TArray<FVector>(), PixelUVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	PixelMeshComponent->SetGenerateOverlapEvents(true);
	
	PixelMeshSegmentIndices.Emplace(Location, SectionIndex);
	MeshSectionCounter++;
}

/**
 * Removes the mesh segment for a pixel.
 *
 * @param Location - The location of the pixel to remove the mesh segment of.
 */
void AVoidgrid::RemovePixelMesh(GridLocationType Location)
{
	PixelMeshComponent->ClearMeshSection(PixelMeshSegmentIndices.FindRef(Location));
	PixelMeshSegmentIndices.Remove(Location);
}

/**
 * Sets the visible a mesh segment for a pixel.
 *
 * @param Location - The location of the pixel set the visablilty of.
 * @param bNewVisibility - The visablity to set the pixel mesh to.
 */
void AVoidgrid::SetPixelMeshVisibility(GridLocationType Location, bool bNewVisibility)
{
	PixelMeshComponent->SetMeshSectionVisible(PixelMeshSegmentIndices.FindRef(Location), bNewVisibility);
}

/**
 * Generates the vertices of a pixel mesh
 *
 * @param Location - The location of the pixel to generate vertices for.
 * @return An array of vertices that can be used to generate a mesh for a pixel
 */
TArray<FVector> AVoidgrid::GetPixelVertices(GridLocationType Location)
{
	TArray<FVector> PixelVertices = TArray<FVector>();
	for (int i = 0; i < 4; i++)
	{
		FVector2D VertexLocation = FVector2D(Location) + FVector2D(GetActorScale3D().X / (i < 2 ? 2 : -2), GetActorScale3D().X / ((i % 2 == 0) ? 2 : -2));
		PixelVertices.Emplace(FVector(VertexLocation, 0.1));
		PixelVertices.Emplace(FVector(VertexLocation, -0.1));
	}
	
	return PixelVertices;
}

/**
 * Generates 2 triangles that conect the four specified vertices.
 *
 * @param UpperRight - The vertex index of the upper right corner of the square.
 * @param UpperLeft - The vertex index of the upper left corner of the square.
 * @param LowerRight - The vertex index of the lower right corner of the square.
 * @param LowerLeft - The vertex index of the lower left corner of the square.
 * @return An array of vertices that can be used to generate triangles that will form a square between the specified vertices.
 */
TArray<int32> AVoidgrid::CreateTrianglesForPixelMeshFace(int32 UpperRight, int32 UpperLeft, int32 LowerRight, int32 LowerLeft)
{
	TArray<int32> Triangles = TArray<int32>();
	Triangles.Emplace(UpperRight);
	Triangles.Emplace(UpperLeft);
	Triangles.Emplace(LowerLeft);
	
	Triangles.Emplace(LowerLeft);
	Triangles.Emplace(LowerRight);
	Triangles.Emplace(UpperRight);
	return Triangles;
}
/* /\ Pixel Mesh /\ *\
\* ---------------- */

/* ------------- *\
\* \/ Faction \/ */


/**
 * Gets the faction of the Voidgrid.
 *
 * @return The faction of the Voidgrid.
 */
EFaction AVoidgrid::GetFaction() const
{
	return EFaction::Inanimate;
}

/* /\ Faction /\ *\
\* ------------- */

/* ------------------------- *\
\* \/ Resource Management \/ */

/**
 * Gets the resources stored on this Voidgrid
 *
 * @return - The resources
 */
const TMap<EResourceType, float> AVoidgrid::GetResources() const
{
	return Resources;
}

/**
 * Adds resources to the Voidgrid
 *
 * @param AddedResources - The resources added and how much of each is added
 */
void AVoidgrid::AddResources(TMap<EResourceType, float> AddedResources)
{
	for (TPair<EResourceType, float> EachAddedResource : AddedResources)
	{
		if (EachAddedResource.Value < 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Attempted to add a negative amount of resource. Use 'UseResources' instead."));
		}
		else
		{

			//Stores the storage capacity of this resource type
			float Capacity = ResourceTypesToStorageCapacities.FindRef(EachAddedResource.Key);
			//Stores the amount of this resource that this voidgrid already has
			float CurrentAmount = Resources.Contains(EachAddedResource.Key) ? Resources.FindRef(EachAddedResource.Key) : 0;
			//Stores the amount of resource that is actually added
			float AddedAmount = CurrentAmount + EachAddedResource.Value > Capacity ? Capacity : Resources.FindRef(EachAddedResource.Key) + EachAddedResource.Value;
				
			//Emplace will override the previous key value pair.
			Resources.Emplace(EachAddedResource.Key, AddedAmount);
		}
	}

}

/* -------------- *\
\* \/ Requests \/ */

/**
 * Adds a resource request to the list of resource requests sorted by priority
 *
 * @param ResourceRequest - The new resource request
 */
void AVoidgrid::AddResourceRequest(FResourceRequest ResourceRequest)
{

	//Stores the lower index of the range where ResourceRequest should be
	int LowerIndex = 0;

	//Stores the upper index of the range where ResourceRequest should be
	int UpperIndex = ResourceRequests.Num() - 1;

	//Stores the middle index between the lower index and the upper index
	int MiddleIndex = (LowerIndex + UpperIndex) / 2;

	//Start the binary search for the index the request should be in the ResourceRequests array 
	while (true)
	{
		if (LowerIndex > UpperIndex)
		{
			//When the lower index is greater than the number of resource requests, that means the new request needs to be at the end of the array
			if (!(LowerIndex > ResourceRequests.Num()))
			{
				ResourceRequests.EmplaceAt(MiddleIndex, ResourceRequest);
				break;
			}
			else
			{
				ResourceRequests.Emplace(ResourceRequest);
				break;
			}
		}

		MiddleIndex = (LowerIndex + UpperIndex) / 2;

		if (ResourceRequests[MiddleIndex].Priority == ResourceRequest.Priority)
		{
			ResourceRequests.EmplaceAt(MiddleIndex, ResourceRequest);
			break;
		}
		else if (ResourceRequest.Priority < ResourceRequests[MiddleIndex].Priority)
		{
			UpperIndex = MiddleIndex - 1;
		}
		else
		{
			LowerIndex = MiddleIndex + 1;
		}
	}
}

/**
 * Handles all resource requests made this tick by using the resources specified. 
 */
void AVoidgrid::HandleResourceRequests()
{
	for (FResourceRequest EachResourceRequest : ResourceRequests)
	{
		if (UseResources(EachResourceRequest.ResourceTypesToAmountUsed))
		{
			EachResourceRequest.OnResourceRequestCompleted.Broadcast();
		}
	}

	ResourceRequests.Empty();
}

/**
 * Uses resources on the Voidgrid. Will not use up resources if not all the resources can be used.
 *
 * @param UsedResources - The resources used and how much of each is used
 *
 * @return - Whether the resources were successfully used or not
 */
const bool AVoidgrid::UseResources(TMap<EResourceType, float> UsedResources)
{

	// \/ Check if all resources can be used \/ //
	for (TPair<EResourceType, float> EachUsedResource : UsedResources)
	{
		if ((!Resources.Contains(EachUsedResource.Key)) || (Resources.FindRef(EachUsedResource.Key) < EachUsedResource.Value))
		{
			//Return if not all resources can be used.
			return false;
		}
	}
	// /\ Check if all resources can be used /\ //

	// \/ Use the resources \/ //
	for (TPair<EResourceType, float> EachUsedResource : UsedResources)
	{
		//Emplace will override the previous key value pair
		Resources.Emplace(EachUsedResource.Key, Resources.FindRef(EachUsedResource.Key) - EachUsedResource.Value);
	}
	// /\ Use the resources /\ //

	return true;
}

/* /\ Requests /\ *\
\* -------------- */

/* -------------- *\
\* \/ Capacity \/ */

/**
 * Gets the storage capacities for each resource on this Voidgrid
 *
 * @return - The storage capacities
 */
const TMap<EResourceType, float> AVoidgrid::GetResourceStorageCapacities() const
{
	return ResourceTypesToStorageCapacities;
}

/**
 * Adds storage capacity for each resource type specified, and increase them by the amount specified
 *
 * @param ResourceTypesToAmountToIncreaseCapacities - The resource types to the amount of increased capacity
 */
void AVoidgrid::AddResourceStorageCapacity(TMap<EResourceType, float> ResourceTypesToAmountToIncreaseCapacities)
{
	for (TPair<EResourceType, float> EachResourceTypeToAmountToIncreaseCapacity : ResourceTypesToAmountToIncreaseCapacities)
	{
		//Stores the capacity added to this resource type
		float AddedCapacity = EachResourceTypeToAmountToIncreaseCapacity.Value;
		//Stores what the new capacity of this resource type will actually be
		float NewStorageCapacity = ResourceTypesToStorageCapacities.Contains(EachResourceTypeToAmountToIncreaseCapacity.Key) ? ResourceTypesToStorageCapacities.FindRef(EachResourceTypeToAmountToIncreaseCapacity.Key) + AddedCapacity : AddedCapacity;
		ResourceTypesToStorageCapacities.Emplace(EachResourceTypeToAmountToIncreaseCapacity.Key, NewStorageCapacity);
	}
}

/**
 * Removes storage capacity for each resource type specified, and decreases them by the amount specified
 *
 * @param ResourceTypesToAmountToDecreaseCapacities - The resource types to the amount of decreased capacity
 */
void AVoidgrid::RemoveResourceStorageCapacity(TMap<EResourceType, float> ResourceTypesToAmountToDecreaseCapacities)
{
	for (TPair<EResourceType, float> EachResourceTypeToAmountToDecreaseCapacity : ResourceTypesToAmountToDecreaseCapacities)
	{
		if (ResourceTypesToStorageCapacities.Contains(EachResourceTypeToAmountToDecreaseCapacity.Key))
		{
			//Stores the current capacity of this resource type
			float CurrentCapacity = ResourceTypesToStorageCapacities.FindRef(EachResourceTypeToAmountToDecreaseCapacity.Key);
			//Stores the current capacity minus the amount of capacity that was removed
			float CurrentCapacityMinusDecreasedAmount = CurrentCapacity - EachResourceTypeToAmountToDecreaseCapacity.Value;
			//Stores the new storage capacity
			float NewStorageCapacity = CurrentCapacityMinusDecreasedAmount < 0 ? 0 : CurrentCapacityMinusDecreasedAmount;

			ResourceTypesToStorageCapacities.Emplace(EachResourceTypeToAmountToDecreaseCapacity.Key, NewStorageCapacity);
		}
	}
}

/* /\ Capacity /\ *\
\* -------------- */

/* ----------- *\
\* \/ Rates \/ */

/**
 * Gets the production rates of each resource type
 *
 * @return A map of each resource type to how much of each is being produced
 */
const TMap<EResourceType, float> AVoidgrid::GetResourceProductionRates() const
{
	return ResourceTypesToProductionRates;
}

/**
 * Gets the consumption rates of each resource type
 *
 * @return A map of each resource type to how much of each is being consumed
 */
const TMap<EResourceType, float> AVoidgrid::GetResourceConsumptionRates() const
{
	return ResourceTypesToConsumptionRates;
}

/**
 * Calculates the resources created and consumed over a given time period
 *
 * @param OutResourceTypesToProductionRates - The production rates of each resource type
 * @param OutResourceTypesToConsumptionRates - The consumption rates of each resource type
 * @param ResourcesProduced - The resources produced over the given time period
 * @param ResourcesConsumed - The resources consumed over the given time period
 * @param Time - The time period over which resources were created and used
 */
void AVoidgrid::CalculateResourceRates(TMap<EResourceType, float>& OutResourceTypesToProductionRates, TMap<EResourceType, float>& OutResourceTypesToConsumptionRates, TMap<EResourceType, float> ResourcesProduced, TMap<EResourceType, float> ResourcesConsumed, float Time)
{
	OutResourceTypesToProductionRates.Empty();
	OutResourceTypesToConsumptionRates.Empty();

	for (TPair<EResourceType, float> EachResourceProduced : ResourcesProduced)
	{
		OutResourceTypesToProductionRates.Emplace(EachResourceProduced.Key, EachResourceProduced.Value / Time);
	}

	for (TPair<EResourceType, float> EachResourceConsumed : ResourcesConsumed)
	{
		OutResourceTypesToConsumptionRates.Emplace(EachResourceConsumed.Key, EachResourceConsumed.Value / Time);
	}
}

/* /\ Rates /\ *\
\* ----------- */

/* /\ Resource Management /\ *\
\* ------------------------- */