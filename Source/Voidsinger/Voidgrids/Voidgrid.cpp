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

//Used to update location and thrust control.
void AVoidgrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTransform(DeltaTime);

	//Find how many heat ticks have passed and call SpreadHeat that number of times
	for (int EachHeatTickPassed = 0; EachHeatTickPassed < DeltaHeatTime / HeatTick; DeltaHeatTime -= HeatTick)
	{
		SpreadHeat();
	}
	


	DeltaHeatTime += DeltaTime;

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
	FIntPoint RelativeLocation = FVector2D(WorldLocation - GetActorLocation()).IntPoint();
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
	
	// \/ Calculate the new heat map \/ /

	TMap<FIntPoint, float> NewHeatMap = TMap<FIntPoint, float>();
	NewHeatMap.Reserve(LocationsToPixelState.Num());

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
				AddedHeat += (OtherPixelTemperature * HeatPropagationFactor) / (4);
			}
		}

		//The heat remaining when this pixel spreads heat to the surrounding pixels
		float RemainingHeat = EachPixel.Value.GetTemperature() * (1 - HeatPropagationFactor);

		float NewHeat = RemainingHeat + AddedHeat;

		//If the amount of heat is below .05, then it's negligable. 
		NewHeatMap.Emplace(EachPixel.Key, NewHeat > .05 ? NewHeat : 0);

	}

	// /\ Calculate the new heat map /\ /

	// \/ Set the temperature of each pixel and find whether it should be melted or frozen \/ /

	for (TPair<FIntPoint, PixelType> EachPixel : LocationsToPixelState.GetGridPairs())
	{
		if (NewHeatMap.Contains(EachPixel.Key) && EachPixel.Value.IsIntact())
		{
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
					EachPixel.Value.GetCurrentPart()->SetPixelFrozen(EachPixel.Key, true);
				}
				//Unfreeze pixel
				else
				{
					EachPixel.Value.GetCurrentPart()->SetPixelFrozen(EachPixel.Key, false);
				}

				EachPixel.Value.SetTemperature(NewHeatMap.FindRef(EachPixel.Key));
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
 * Gets the grid location of a world location.
 *
 * @param WorldLocation - The world location to transform.
 * @return The grid location of WorldLocation;
 */
FVector2D AVoidgrid::TransformWorldToGrid(FVector WorldLocation) const
{
	return (FVector2D(GetTransform().InverseTransformPosition(WorldLocation)) + CenterOfMass);
}

/**
 * Gets the world location of a grid location.
 *
 * @param GridLocation - The grid location to transform.
 * @return The world location of GridLocation;
 */
FVector AVoidgrid::TransformGridToWorld(FVector2D GridLocation) const
{
	return GetTransform().TransformPosition(FVector(GridLocation - CenterOfMass, 0));
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

/* --------------- *\
\* \/ Explosion \/ */

/**
 * Causes an explosion at a world location. This will remove all pixels within the explosion radius, but pixel strength may reduce the radius.
 *
 * @param WorldContext - An object used to get the world that the explosion will occur in.
 * @param WorldLocation - The location of the center of the explosion.
 * @param Radius - The distance from the center within which pixels will be destroyed.
 */
void AVoidgrid::ExplodeVoidgrids(UObject* WorldContext,  FVector WorldLocation, float Radius)
{

	//Debugggg -Mabel Suggestion
	DrawDebugCircle(WorldContext->GetWorld(), FTransform(FRotator(90, 0, 0), WorldLocation + FVector(0, 0, 0.5), FVector(1)).ToMatrixWithScale(), Radius, 50, FColor::White, false, 2, 0U, .05);

	//Get all actors in radius
	TArray<FOverlapResult> Results = TArray<FOverlapResult>();
	WorldContext->GetWorld()->OverlapMultiByChannel(Results, WorldLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(Radius));

	FlushPersistentDebugLines(WorldContext->GetWorld());
	//Start explosion at WorldLocation for each voidgrid found
	for (FOverlapResult EachResult : Results)
	{
		if (AVoidgrid* OtherVoidgrid = Cast<AVoidgrid>(EachResult.GetActor()))
		{
			FIntPoint GridRelativeExplosionLocation = OtherVoidgrid->TransformWorldToGrid(WorldLocation).IntPoint();
			OtherVoidgrid->StartExplosionAtPixel(GridRelativeExplosionLocation, GridRelativeExplosionLocation, Radius);
		}
	}
}

/**
 * Recursive function that will explode all pixels shadowed by the pixel at grid location.
 *
 * @param GridLocation - The pixel to remove.
 * @param GridRelativeExplosionLocation -  The location of the center of the explosion relative to the pixel grid.
 * @param Radius - The radius of the explosion.
 * @param Arc - The arc to apply the explosion in. Only pixels inside the arc will be destroyed.
 */
void AVoidgrid::StartExplosionAtPixel(FIntPoint PixelLocation, FIntPoint GridRelativeExplosionLocation, float Radius, FVectorArc Arc)
{
	//Array of all adjacent pixel offests
	//Wouldn't this go back and check the previous location you were at? Or am I missing something -Mabel Suggestion
	static TArray<FIntPoint> AdjacentPixelOffests{ TArray<FIntPoint>() };
	if (AdjacentPixelOffests.IsEmpty())
	{
		AdjacentPixelOffests.Emplace(FIntPoint( 0, 1));
		AdjacentPixelOffests.Emplace(FIntPoint( 1, 0));
		AdjacentPixelOffests.Emplace(FIntPoint( 0,-1));
		AdjacentPixelOffests.Emplace(FIntPoint(-1, 0));
	}

	//The location of the pixel relative to the explosion's center.
	FIntPoint ExplosionRelativeLocation = PixelLocation - GridRelativeExplosionLocation;

	
	//Shrink radius based on part strength.
	if (LocationsToPixelState.Contains(PixelLocation) && LocationsToPixelState.Find(PixelLocation)->IsIntact())
	{
		Radius -= LocationsToPixelState.Find(PixelLocation)->GetCurrentPart()->GetData()->Strength - 1;
	}

	//If in new explosion radius
	if (ExplosionRelativeLocation.SizeSquared() < FMath::Square(Radius))
	{
		FVector DebugOffset = FVector(0, 0, .5 * FMath::FRand() + .1);
		// \/ For each adjacent pixel start an explosion if in Arc \/ //
		for (FIntPoint EachAdjacentPixelOffest : AdjacentPixelOffests)
		{
			//The pixel location of the next pixel to destroy.
			FIntPoint AdjacentPixelLocation = PixelLocation + EachAdjacentPixelOffest;
			//The location relative to the center of the explosion of the next pixel to destroy.
			FVector2D AdjacentPixelExplosionRelativeLocation = AdjacentPixelLocation - GridRelativeExplosionLocation;
			// If in the correct direction for this quadrant.
			if (AdjacentPixelExplosionRelativeLocation.SizeSquared() > ExplosionRelativeLocation.SizeSquared())
			{
				//The lower arc bound of an arc that contains only the AdjacentPixel.
				FVector2D PixelLowerArcBound = FVector2D();
				//The upper arc bound of an arc that contains only the AdjacentPixel.
				FVector2D PixelUpperArcBound = FVector2D();
				//The sign of X & Y in a tri-bit format
				int32 NextExplosionSignedDirection = (FMath::Sign(AdjacentPixelExplosionRelativeLocation.X) + 1) + 3 * (FMath::Sign(AdjacentPixelExplosionRelativeLocation.Y) + 1);
				// \/ Sets PixelArcBounds based on sign \/ //
				switch (NextExplosionSignedDirection)
				{
					// X < 0 && Y < 0
				case 0:
					PixelLowerArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(0.5, -0.5);
					PixelUpperArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(-0.5, 0.5);
					break;

					// X = 0 && Y < 0
				case 1:
					PixelLowerArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(0.5, 0.5);
					PixelUpperArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(-0.5, 0.5);
					break;

					// X > 0 && Y < 0
				case 2:
					PixelLowerArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(0.5, 0.5);
					PixelUpperArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(-0.5, -0.5);
					break;

					// X < 0 && Y = 0
				case 3:
					PixelLowerArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(0.5, -0.5);
					PixelUpperArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(0.5, 0.5);
					break;

					// X = 0 && Y = 0
				case 4:
					break;

					// X > 0 && Y = 0
				case 5:
					PixelLowerArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(-0.5, 0.5);
					PixelUpperArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(-0.5, -0.5);
					break;

					// X < 0 && Y > 0
				case 6:
					PixelLowerArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(-0.5, -0.5);
					PixelUpperArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(0.5, 0.5);
					break;

					// X = 0 && Y > 0
				case 7:
					PixelLowerArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(-0.5, -0.5);
					PixelUpperArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(0.5, -0.5);
					break;

					// X > 0 && Y > 0
				case 8:
					PixelLowerArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(-0.5, 0.5);
					PixelUpperArcBound = AdjacentPixelExplosionRelativeLocation + FVector2D(0.5, -0.5);
					break;

				default:
					ensureMsgf(false, TEXT("NextExplosionSignedDirection Invalid"));
					break;
				}
				// /\ Sets PixelArcBounds based on sign /\ //

				//  | ------------------------------------ In radius ------------------------- |    | ----------------------------- In arc ------------------------------- |
				if (AdjacentPixelExplosionRelativeLocation.SizeSquared() < FMath::Square(Radius) && Arc.DoesLinePassThoughArc(PixelLowerArcBound, PixelUpperArcBound, false))
				{
					FVectorArc NewArc = Arc;
					NewArc.ShrinkArcBounds(PixelLowerArcBound, PixelUpperArcBound);
					StartExplosionAtPixel(AdjacentPixelLocation, GridRelativeExplosionLocation, Radius, NewArc);
				//}
					DrawDebugDirectionalArrow(GetWorld(), DebugOffset + TransformGridToWorld(PixelLocation), DebugOffset + TransformGridToWorld(AdjacentPixelLocation), .02, FColor::Green, true);
				}
				else if (AdjacentPixelExplosionRelativeLocation.SizeSquared() < FMath::Square(Radius))
				{
					DrawDebugDirectionalArrow(GetWorld(), DebugOffset + TransformGridToWorld(PixelLocation), DebugOffset + TransformGridToWorld(AdjacentPixelLocation), .02, FColor::Red, true);
				}
				else
				{
					DrawDebugDirectionalArrow(GetWorld(), DebugOffset + TransformGridToWorld(PixelLocation), DebugOffset + TransformGridToWorld(AdjacentPixelLocation), .02, FColor::Black, true);
				}
			}
		}
		// /\ For each adjacent pixel start an explosion if in Arc /\ //

		RemovePixel(PixelLocation);
	}
}

/* /\ Explosion /\ *\
\* --------------- */

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
 * @param Location - The location of the pixel set the visibility of.
 * @param bNewVisibility - The visibility to set the pixel mesh to.
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

//Notes, delete later

////Comment -Mabel Suggestion
//void UPartGridComponent::DistrubuteHeat()
//{
//	TMap<FIntPoint, float> NewHeatMap = TMap<FIntPoint, float>();
//	NewHeatMap.Reserve(PartGrid.Num());
//
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" -Mabel Suggestion
//	for (int j = 0; j < PartGrid.Num(); j++)
//	{
//		float NewHeat = 0;
//
//		//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" -Mabel Suggestion
//		for (int i = 0; i < 4; i++)
//		{
//			FIntPoint TargetPoint = ((i % 2 == 1) ? FIntPoint((i > 1) ? 1 : -1, 0) : FIntPoint(0, (i > 1) ? 1 : -1));
//
//			if (PartGrid.Contains(TargetPoint + PartGrid.LocationAtIndex(j)))
//			{
//				//4 is borderline magic number. I understand why you used it but still -Mabel Suggestion
//				NewHeat += PartGrid.FindRef(TargetPoint + PartGrid.LocationAtIndex(j)).GetTemperature() * HeatPropagationFactor / (4);
//			}
//		}
//
//		//Math is occuring that needs to be commented. Why is the pixels current temperature mutiplied by 1 - the heat propagation factor? -Mabel Suggestion
//		NewHeat = PartGrid.ValueAtIndex(j).GetTemperature() * (1 - HeatPropagationFactor) + NewHeat;
//
//
//		//Why 0.5? comment plz (also, is it.... magic number?) -Mabel Suggestion
//		NewHeatMap.Emplace(PartGrid.LocationAtIndex(j), NewHeat > .05 ? NewHeat : 0);
//	}
//
//	TArray<FIntPoint> KeysToDestroy = TArray<FIntPoint>();
//
//	//You iterate through the part grid not once, but twice in this function. Big oof if I'm being honest. Honestly, it'd be better to just have heat not spread
//	//if this is the way that we're doing it. This is so bad for large ships. 
//	//I wonder if each part could handle it's own heat and distribute to the places around it instead of doing this on the part grid? I don't know how much it would help but 
//	//it might help a little bit. Not sure, that might be just as laggy. -Mabel Suggestion
//	for (int i = 0; i < PartGrid.Num(); i++)
//	{
//		if (NewHeatMap.FindRef(PartGrid.LocationAtIndex(i)) > PartGrid.ValueAtIndex(i).Part->GetHeatResistance())
//		{
//			KeysToDestroy.Emplace(PartGrid.LocationAtIndex(i));
//		}
//		else
//		{
//			PartGrid.ValueAtIndex(i).SetTemperature(NewHeatMap.FindRef(PartGrid.LocationAtIndex(i)));
//		}
//	}
//
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" 
//	//"Val" is just as bad as i. Just saying. -Mabel Suggestion
//	for (FIntPoint Val : KeysToDestroy)
//	{
//		DestroyPixel(Val);
//	}
//}