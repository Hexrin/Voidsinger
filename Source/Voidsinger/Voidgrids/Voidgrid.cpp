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
				//DrawDebugDirectionalArrow(GetWorld(), OtherShip->GetActorLocation() + FVector(OtherRelativeHitLocation, 0), OtherShip->GetActorLocation() + FVector(OtherRelativeHitLocation, 0) + FVector(-1 * CollisionImpulseFactor * ImpactNormal, 0), 5, FColor::Red, false, 5, 0U, 0.3f);
				//UE_LOG(LogTemp, Warning, TEXT("%s Applyed an impules of %s at %s to %s"), *GetOwner()->GetName(), *(-1 * CollisionImpulseFactor * ImpactNormal).ToString(), *OtherRelativeHitLocation.ToString(), *Result.GetActor()->GetName());
			}
			else
			{
				CollsionForce = (-1 * (1 + CollisionElasticity) * GetVelocityOfPoint(RelativeHitLocation) | ImpactNormal) /
					(1 / Mass) + (FMath::Square(RelativeHitLocation ^ ImpactNormal) / MomentOfInertia);
			}

			AddImpulse(CollsionForce * ImpactNormal, FVector(RelativeHitLocation, 0));
			//DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation() + FVector(RelativeHitLocation, 0), GetOwner()->GetActorLocation() + FVector(RelativeHitLocation, 0) + FVector(CollisionImpulseFactor * ImpactNormal, 0), 5, FColor::Blue, false, 5, 0U, 0.3f);
			//UE_LOG(LogTemp, Warning, TEXT("%s Applyed an impules of %s at %s to itself when colideing with %s"), *GetOwner()->GetName(), *(CollisionImpulseFactor * ImpactNormal).ToString(), *RelativeHitLocation.ToString(), *Result.GetActor()->GetName());
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
		for (TPair<GridLocationType, PixelType> PixelData : PixelMold.GetGridPairs())
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
		Mass += DeltaMass;
		CenterOfMass += DeltaMass / Mass * MassLocation;
		PixelMeshComponent->SetRelativeLocation(FVector(-1 * CenterOfMass, 0));
		MomentOfInertia += (1 / 12) + DeltaMass * (MassLocation).SizeSquared();
		OnMassChanged.Broadcast(Mass, CenterOfMass, MomentOfInertia);
	}
}

/* /\ Physics /\ *\
\* ------------- */



/* ---------------- *\
\* \/ Pixel Mold \/ */

/**
 * Sets the pixel mold of the ship
 *
 * @param NewPixelMold - The value to assign to the pixel mold of the ship
 */
void AVoidgrid::SetPixelMold(TSet<FMinimalPartInstanceData> NewPixelMold)
{
	MinimalPixelMoldDataType DataOfPartsToCreate = NewPixelMold;

	//Remove Unneccesary Parts
	for (UPart* Part : Parts)
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
	for (UPart* Part : TemporaryParts)
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
		UPart* Part = UPart::CreatePart(this, FPartInstanceData(DataOfPartToCreate));
		Parts.Add(Part);

		for (GridLocationType ShapeComponent : DataOfPartToCreate.Data->Shape)
		{
			GridLocationType NewPixelLocation = Part->GetTransform().TransformGridLocation(ShapeComponent);
			PixelMold.Emplace(NewPixelLocation, FGridPixelData(Part));

			MutablePixels.Add(NewPixelLocation);
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
		UPart* Part = nullptr;
		if (!NewState.Mold.Contains(PartState.GetMinimalInstanceData()))
		{
			Part = UPart::CreatePart(this, PartState.GetMinimalInstanceData());
			TemporaryParts.Add(Part);
		}
		else
		{
			for (GridLocationType FistShapeCompoenent : PartState.GetData()->Shape)
			{
				Part = PixelMold.Find(PartState.GetTransform().TransformGridLocation(FistShapeCompoenent))->GetTargetPart();
				break;
			}
		}

		for (GridLocationType ShapeComponent : PartState.GetShape())
		{
			GridLocationType ShapeCompoentLocation = PartState.GetTransform().TransformGridLocation(ShapeComponent);
			if (!PixelMold.Contains(ShapeCompoentLocation))
			{
				PixelMold.Emplace(ShapeCompoentLocation, PixelType(Part, UPart::GetNullPart()));
			}
			SetPixelIntact(ShapeCompoentLocation, true, false);

			UpdatePixelMesh(ShapeCompoentLocation);
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
	TSet<FPartInstanceData> AllPartInstanceData = TSet<FPartInstanceData>();
	TSet<FMinimalPartInstanceData> AllMinimalPartInstanceData = TSet<FMinimalPartInstanceData>();
	for (UPart* Part : Parts)
	{
		AllPartInstanceData.Add(Part->GetPartInstanceData());
		AllMinimalPartInstanceData.Add(Part->GetMinimalPartInstanceData());
	}

	return FVoidgridState(AllMinimalPartInstanceData, AllPartInstanceData);
}

/**
 * Damages a pixel.
 *
 * @param Location - The location of the pixel to damage.
 */
void AVoidgrid::DamagePixel(GridLocationType Location)
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
	if (PixelMold.Contains(Location))
	{
		PixelType* PixelRef = PixelMold.Find(Location);
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
	if (PixelMold.Contains(Location))
	{
		if (PixelMold.Find(Location)->IsIntact() != bNewIntact)
		{
			if (!bNewIntact) 
			{
				float OldPixelMass = PixelMold.Find(Location)->GetCurrentPart()->GetPixelMass();
				if (PixelMold.Find(Location)->GetTargetPart() == UPart::GetNullPart())
				{
					TemporaryParts.Remove(PixelMold.Find(Location)->GetCurrentPart());
					MutablePixels.Remove(Location);
					PixelMold.Remove(Location);
				}
				else
				{
					MutablePixels.Add(Location);
					PixelMold.Find(Location)->SetIntact(bNewIntact);
				}
				OnPixelRemoved.Broadcast(Location, bApplyChangeEffect);
				UpdateMassProperties(-1 * OldPixelMass, FVector2D(Location));
			}
			else
			{
				PixelMold.Find(Location)->SetIntact(bNewIntact);
				MutablePixels.Remove(Location);
				OnPixelAdded.Broadcast(Location, bApplyChangeEffect);
				UpdateMassProperties(PixelMold.Find(Location)->GetCurrentPart()->GetPixelMass(), FVector2D(Location));
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
	if (PixelMold.Contains(Location) && NewTarget != PixelMold.Find(Location)->GetTargetPart())
	{
		if (NewTarget == PixelMold.Find(Location)->GetCurrentPart())
		{
			MutablePixels.Remove(Location);
			TemporaryParts.Remove(PixelMold.Find(Location)->GetCurrentPart());

			if(NewTarget == UPart::GetNullPart())
			{
				PixelMold.Remove(Location);
				UpdatePixelMesh(Location);
			}
			else
			{
				Parts.Add(PixelMold.Find(Location)->GetCurrentPart());
			}
		}
		else
		{
			MutablePixels.Add(Location);
			TemporaryParts.Add(PixelMold.Find(Location)->GetCurrentPart());
			Parts.Remove(PixelMold.Find(Location)->GetCurrentPart());
		}
	}
}

/**
 * Removes all parts from this voidgrid.
 * Does not call OnDamaged.
 */
void AVoidgrid::ClearVoidgrid()
{
	TMap<GridLocationType, PixelType> GridPairs = PixelMold.GetGridPairs();
	PixelMold.Empty();
	for (TPair<GridLocationType, PixelType> Pair : GridPairs)
	{
		UpdatePixelMesh(Pair.Key);
	}
	Parts.Empty();
	TemporaryParts.Empty();
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
	if (PixelMold.Contains(Location))
	{
		if (PixelMold.Find(Location)->IsIntact())
		{
			if (PixelMeshSegmentIndices.Contains(Location))
			{
				SetPixelMeshVisibility(Location, true);
			}
			else
			{
				AddPixelMesh(Location);
			}

			if (IsValid(PixelMold.Find(Location)->GetMaterial()))
			{
				PixelMeshComponent->SetMaterial(PixelMeshSegmentIndices.FindRef(Location), PixelMold.Find(Location)->GetMaterial());
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
	//If this pixel already has a mesh set the index to that mesh
	int32 SectionIndex = PixelMeshSegmentIndices.Contains(Location) ? PixelMeshSegmentIndices.FindRef(Location) : PixelMeshSegmentIndices.Num();

	PixelMeshComponent->CreateMeshSection(SectionIndex, GetPixelVertices(Location), PixelTriangles, TArray<FVector>(), PixelUVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	PixelMeshComponent->SetGenerateOverlapEvents(true);
	
	PixelMeshSegmentIndices.Emplace(Location, SectionIndex);
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