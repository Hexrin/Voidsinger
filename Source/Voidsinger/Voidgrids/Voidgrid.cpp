// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidgrid.h"
#include "Parts/Part.h"

//Sets default values for this voidgrid's properties
AVoidgrid::AVoidgrid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// \/ Initialize Mesh Component \/
	PixelMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh Component"));
	PixelMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PixelMeshComponent->bUseComplexAsSimpleCollision = true;
	PixelMeshComponent->bRenderCustomDepth = true;

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
	// /\ Initialize Mesh Component /\

}

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
		UPart* Part;
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
			SetPixelIntact(ShapeCompoentLocation, true);

			UpdatePixelMesh(ShapeCompoentLocation);
		}
	}
}

/**
 * Damages a pixel.
 *
 * @param Location - The location of the pixel to damage.
 */
void AVoidgrid::DamagePixel(GridLocationType Location)
{
	if (PixelMold.Contains(Location))
	{
		PixelType* PixelRef = PixelMold.Find(Location);
		if (PixelRef->IsIntact())
		{
			OnDamaged.Broadcast(Location);

			if (PixelRef->GetTargetPart() == UPart::GetNullPart())
			{
				TemporaryParts.Remove(PixelRef->GetCurrentPart());
				PixelMold.Remove(Location);
				MutablePixels.Remove(Location);
			}
			else
			{
				MutablePixels.Add(Location);
				PixelMold.Find(Location)->SetIntact(false);
			}
			UpdatePixelMesh(Location);
		}
	}
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
		if (!PixelMold.Find(Location)->IsIntact())
		{
			OnRepaired.Broadcast(Location);
			MutablePixels.Remove(Location);
			PixelMold.Find(Location)->SetIntact(true);
			UpdatePixelMesh(Location);
		}
		else
		{
			DamagePixel(Location);
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
void AVoidgrid::SetPixelIntact(GridLocationType Location, bool bNewIntact)
{
	if (PixelMold.Contains(Location))
	{
		if (PixelMold.Find(Location)->IsIntact() != bNewIntact)
		{
			if (!bNewIntact) 
			{
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
			}
			else
			{
				PixelMold.Find(Location)->SetIntact(bNewIntact);
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