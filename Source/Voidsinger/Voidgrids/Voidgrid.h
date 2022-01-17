// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UObject/UObjectGlobals.h"
#include "GridMap.h"
#include "Parts/Part.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidgrid.generated.h"

//The type used for storing pixel data
typedef FGridPixelData PixelType;
//The type used for storing a ships Pixel Mold
typedef TGridMap<PixelType> PixelMoldType;
//Stores all information required to replicate a mold
typedef TSet<FMinimalPartInstanceData> MinimalPixelMoldDataType;

//Stores data about a pixel.
USTRUCT()
struct VOIDSINGER_API FGridPixelData
{
	GENERATED_BODY()
	
public:
	FGridPixelData(UPart* PartOfPixel, UPart* TargetPartOfPixel, bool bPixelIntact = false)
	{
		if (!IsValid(PartOfPixel))
		{
			PartOfPixel = UPart::GetNullPart();
		}

		if (!IsValid(TargetPartOfPixel))
		{
			TargetPartOfPixel = UPart::GetNullPart();
		}


		Material = UMaterialInstanceDynamic::Create(LoadObject<UMaterialInterface>(NULL, TEXT("/Game/2DAssets/Parts/Mat_BaseFreeformPart.Mat_BaseFreeformPart"), NULL, LOAD_None, NULL), TargetPartOfPixel->GetOuter());//Outer may cause memory leak. Im not sure

		SetTargetPart(TargetPartOfPixel);
		SetCurrentPart(PartOfPixel);
		SetIntact(bPixelIntact);
	}

	//Constructs a FGridPixelData using a part.
	FGridPixelData(UPart* TargetPartOfPixel = nullptr, bool bPixelIntact = false)
	{
		UPart* PartOfPixel = UPart::GetNullPart();

		if (!IsValid(TargetPartOfPixel))
		{
			TargetPartOfPixel = UPart::GetNullPart();
		}


		Material = UMaterialInstanceDynamic::Create(LoadObject<UMaterialInterface>(NULL, TEXT("/Game/2DAssets/Parts/Mat_BaseFreeformPart.Mat_BaseFreeformPart"), NULL, LOAD_None, NULL), TargetPartOfPixel->GetOuter());//Outer may cause memory leak. Im not sure

		SetTargetPart(TargetPartOfPixel);
		SetCurrentPart(PartOfPixel);
		SetIntact(bPixelIntact);
	}

	/**
	 * Gets whether this pixel is intact.
	 *
	 * @return Whether this pixel is intact.
	 */
	bool IsIntact()
	{
		return bIntact;
	}

	/**
	 * Sets whether this pixel is intact.
	 *
	 * @param bNewIntact - the new intactness of this pixel.
	 */
	void SetIntact(bool bNewIntact)
	{
		UE_LOG(LogTemp, Warning, TEXT("Intact to: %s"), bNewIntact ? *FString("True") : *FString("False"));
		if (bNewIntact != bIntact)
		{			
			Temperature = 0;
			SetCurrentPart(GetTargetPart());
		}

		bIntact = bNewIntact;
	}

	/**
	 * Sets the temperature of this pixel.
	 *
	 * @param NewTemperature - The new temperature of this pixel.
	 */
	void SetTemperature(float NewTemperature)
	{
		Temperature = bIntact ? NewTemperature : 0;
	}

	/**
	 * Add an amount to the temperature of this pixel.
	 *
	 * @param TemperatureToAdd - The amount to add to the temperature of this pixel.
	 * @return The new temperature of this pixel.
	 */
	float AddTemperature(float TemperatureToAdd)
	{
		return Temperature += bIntact ? TemperatureToAdd : 0;
	}

	/**
	 * Gets the part of this pixel.
	 *
	 * @return The part of this pixel.
	 */
	UPart* GetCurrentPart()
	{
		return CurrentPart;
	}

	/**
	 * Sets the part of this pixel.
	 * 
	 * @param NewPart - The new value of part.
	 * @return A refernce to this.
	 */
	void SetCurrentPart(UPart* NewPart)
	{
		if (!IsValid(NewPart))
		{
			NewPart = UPart::GetNullPart();
		}

		UE_LOG(LogTemp, Warning, TEXT("Set Current part to: %s"), *NewPart->GetName());
		if (CurrentPart != NewPart && IsValid(Material) && IsValid(NewPart) && IsValid(NewPart->GetData()) && IsValid(NewPart->GetData()->Texture))
		{
			Material->SetTextureParameterValue(TEXT("PartTexture"), NewPart->GetData()->Texture);
			UE_LOG(LogTemp, Warning, TEXT("Set texture to: %s"), *NewPart->GetData()->Texture->GetName());
		}
		CurrentPart = NewPart;
	}

	/**
	 * Gets the target part of this pixel.
	 *
	 * @return The target part of this pixel.
	 */
	UPart* GetTargetPart()
	{
		UE_LOG(LogTemp, Warning, TEXT("Get TargetPart =: %s"), *TargetPart->GetName());
		return TargetPart;
	}

	/**
	 * Sets the new target part of this pixel.
	 *
	 * @param NewPart - The new target part of this pixel.
	 * @return A refernce to this.
	 */
	void SetTargetPart(UPart* NewPart)
	{
		if (!IsValid(NewPart))
		{
			NewPart = UPart::GetNullPart();
		}

		UE_LOG(LogTemp, Warning, TEXT("Set TargetPart to: %s"), *NewPart->GetName());
		TargetPart = NewPart;
	}

	UMaterialInterface* GetMaterial()
	{
		return Material;
	}

	bool operator==(const FGridPixelData& Other) const
	{
		return CurrentPart == Other.CurrentPart && TargetPart == Other.TargetPart && bIntact == Other.bIntact && Temperature == Other.Temperature;
	}

	FGridPixelData& operator=(const FGridPixelData& Other)
	{
		CurrentPart = Other.CurrentPart;
		TargetPart = Other.TargetPart;
		bIntact = Other.bIntact;
		Temperature = Other.Temperature;
		Material = Other.Material;
		return *this;
	}

private:
	//Stores a pointer to the part in this pixel.
	UPROPERTY()
	UPart* CurrentPart = nullptr;

	//Stores a pointer to the target part of this pixel.
	UPROPERTY()
	UPart* TargetPart = nullptr;

	//Store whether or not this pixel is intact.
	bool bIntact = false;

	//Stores the temperature of this pixel.
	float Temperature = 0.f;

	//Stores a pointer to the material of this pixel.
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstanceDynamic* Material = nullptr;
};

//Hash function for FGridPixelData
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FGridPixelData& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FGridPixelData& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FGridPixelData));
	return Hash;
}
#endif

//Used for disbatching events requireing a grid locaiton
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridLocationDelegate, FIntPoint, GridLocaction);

UCLASS()
class VOIDSINGER_API AVoidgrid : public APawn
{
	GENERATED_BODY()

public:
	//Sets default values for this voidgrid's properties
	AVoidgrid();

	/* ---------------- *\ 
	\* \/ Pixel Mold \/ */

public:
	//Called when this is damaged.
	FGridLocationDelegate OnDamaged;
	//Called when this is repaired.
	FGridLocationDelegate OnRepaired;

	/**
	 * Sets the pixel mold of the ship
	 * 
	 * @param NewPixelMold - The value to assign to the pixel mold of the ship
	 */
	void SetPixelMold(MinimalPixelMoldDataType NewPixelMold);

	/**
	 * Gets the minimal part data for all parts on this void grid.
	 * 
	 * @return The minimal part data for all parts on this void grid.
	 */
	MinimalPixelMoldDataType GetMinimalMoldData();

	/**
	 * Damages a pixel.
	 * 
	 * @param Location - The location of the pixel to damage.
	 */
	void DamagePixel(GridLocationType Location);

	/**
	 * Repair a pixel.
	 *
	 * @param Location - The location of the pixel to repair.
	 */
	void RepairPixel(GridLocationType Location);

	/**
	 * Repairs a random pixel pixel.
	 */
	void RepairPixel();

private:
	//Stores the Pixel Mold of this.
	PixelMoldType PixelMold;

	//Stores the Locations of all damaged and temporary part Pixels.
	TSet<GridLocationType> MutablePixels;

	//Stores a referce to all permanent parts on this.
	UPROPERTY()
	TSet<UPart*> Parts;

	//Stores a referce to all parts marked for removal on this.
	UPROPERTY()
	TSet<UPart*> TemporaryParts;

	/* /\ Pixel Mold /\ *\
	\* ---------------- */

	/* ---------------- *\
	\* \/ Pixel Mesh \/ */

public:
	//A procedural mesh component for physicaly representing all pixels on this.
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* PixelMeshComponent;

private:
	/**
	 * Updates a mesh segment for a pixel.
	 *
	 * @param Location - The location of the pixel to update the mesh of.
	 */
	void UpdatePixelMesh(GridLocationType Location);

	/**
	 * Creates a mesh segment for a pixel.
	 * 
	 * @param Location - The location of the pixel to create a mesh segment for.
	 */
	void AddPixelMesh(GridLocationType Location);

	/**
	 * Removes the mesh segment for a pixel.
	 *
	 * @param Location - The location of the pixel to remove the mesh segment of.
	 */
	void RemovePixelMesh(GridLocationType Location);

	/**
	 * Sets the visible a mesh segment for a pixel.
	 *
	 * @param Location - The location of the pixel set the visablilty of.
	 * @param bNewVisibility - The visablity to set the pixel mesh to.
	 */
	void SetPixelMeshVisibility(GridLocationType Location, bool bNewVisibility);

	/**
	 * Generates the vertices of a pixel mesh
	 * 
	 * @param Location - The location of the pixel to generate vertices for.
	 * @return An array of vertices that can be used to generate a mesh for a pixel
	 */
	TArray<FVector> GetPixelVertices(GridLocationType Location);

	/**
	 * Generates 2 triangles that conect the four specified vertices.
	 * 
	 * @param UpperRight - The vertex index of the upper right corner of the square.
	 * @param UpperLeft - The vertex index of the upper left corner of the square.
	 * @param LowerRight - The vertex index of the lower right corner of the square.
	 * @param LowerLeft - The vertex index of the lower left corner of the square.
	 * @return An array of vertices that can be used to generate triangles that will form a square between the specified vertices.
	 */
	UFUNCTION()
	TArray<int32> CreateTrianglesForPixelMeshFace(int32 UpperRight = 0, int32 UpperLeft = 1, int32 LowerRight = 2, int32 LowerLeft =3);

	//Stores the mesh segment incisces coresponding to each pixel
	TMap<GridLocationType, int32> PixelMeshSegmentIndices;

	//Stores the default UVs of a single pixel mesh
	UPROPERTY()
	TArray<FVector2D> PixelUVs;

	//Stores the default triangles of a single pixel mesh
	UPROPERTY()
	TArray<int32> PixelTriangles;

	/* /\ Pixel Mesh /\ *\
	\* ---------------- */

	/* ------------- *\
	\* \/ Faction \/ */

public:

	/**
	 * Gets the faction of the Voidgrid.
	 * 
	 * @return The faction of the Voidgrid.
	 */
	UFUNCTION(BlueprintPure)
	virtual EFaction GetFaction() const;

	/* /\ Faction /\ *\
	\* ------------- */
};
