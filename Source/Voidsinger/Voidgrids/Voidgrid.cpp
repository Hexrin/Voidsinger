// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidgrid.h"
#include "BasePart.h"

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
void AVoidgrid::SetPixelMold(MinimalPixelMoldDataType NewPixelMold)
{
	MinimalPixelMoldDataType DataOfPartsToCreate = NewPixelMold;
	//Remove Unneccesary Parts
	for (TPair<GridLocationType, PixelType> PixelData : PixelMold.GetGridPairs())
	{
		FMinimalPartData PartData = FMinimalPartData(PixelData.Value.GetTargetPart()->StaticClass(), PixelData.Value.GetTargetPart()->GetTransform());
		if (!NewPixelMold.Contains(PartData))
		{
			PixelMold.Find(PixelData.Key)->SetTargetPart(UNullPart::Get());
			Parts.Remove(PixelData.Value.GetCurrentPart());
			TemporaryParts.Emplace(PixelData.Value.GetCurrentPart());
		}
		else
		{
			DataOfPartsToCreate.Remove(PartData);
		}
	}

	for (FMinimalPartData DataOfPartToCreate : DataOfPartsToCreate)
	{
		UBasePart* Part = UBasePart::CreatePart(this, FPartData(DataOfPartToCreate));
		Parts.Emplace(Part);

		for (GridLocationType ShapeComponent : Cast<UBasePart>(Part->StaticClass()->GetDefaultObject())->GetDefaultShape())
		{
			PixelMold.Emplace(Part->GetTransform().TransformGridLocation(ShapeComponent), PixelType(Part));
		}
	}
}

/**
 * Gets the minimal part data for all parts on this void grid.
 *
 * @return The minimal part data for all parts on this void grid.
 */
MinimalPixelMoldDataType AVoidgrid::GetMinimalMoldData()
{
	MinimalPixelMoldDataType AllPartsData = MinimalPixelMoldDataType();
	for (TPair<GridLocationType, PixelType> PixelData : PixelMold.GetGridPairs())
	{
		AllPartsData.Add(PixelData.Value.GetTargetPart()->GetMinimalData());
	}
	return AllPartsData;
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

			//Removes refernces to temporary parts
			if (TemporaryParts.Contains(PixelRef->GetCurrentPart()) && PixelRef->GetCurrentPart()->GetShape().Num() == 0)
			{
				TemporaryParts.Remove(PixelRef->GetCurrentPart());
			}

			if (PixelRef->GetTargetPart() == UNullPart::Get())
			{
				PixelMold.Remove(Location);
			}
			else
			{
				MutablePixels.Add(Location);
				PixelMold.Find(Location)->SetIntact(false);
			}
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
		OnRepaired.Broadcast(Location);
		MutablePixels.Remove(Location);
		PixelMold.Find(Location)->SetIntact(true);
	}
}


/* /\ Pixel Mold /\ *\
\* ---------------- */

/* ---------------- *\
\* \/ Pixel Mesh \/ */

/**
 * Creates a mesh segment for a pixel.
 *
 * @param Location - The location of the pixel to create a mesh segment for.
 */
void AVoidgrid::AddPixelMesh(GridLocationType Location)
{
	//If this pixel already has a mesh set the index to that mesh
	int32 SectionIndex = SectionIndex = PixelMeshSegmentIndices.Contains(Location) ? PixelMeshSegmentIndices.FindRef(Location) : PixelMeshSegmentIndices.Num();;

	PixelMeshComponent->CreateMeshSection(SectionIndex, GetPixelVertices(Location), PixelTriangles, TArray<FVector>(), PixelUVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	PixelMeshComponent->SetMaterial(SectionIndex, PixelMold.Find(Location)->GetMaterial());
	PixelMeshComponent->SetGenerateOverlapEvents(true);
	
	PixelMeshSegmentIndices.Emplace(Location, SectionIndex);
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
/* /\ Pixel Mesh /\ *\
\* ---------------- */