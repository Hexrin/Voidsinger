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



class UThrustManager;


//The type used for storing pixel data
typedef FGridPixelData PixelType;
//The type used for storing a ships Pixel Mold
typedef TGridMap<PixelType> PixelMoldType;
//Stores all information required to replicate a mold
typedef TSet<FMinimalPartInstanceData> MinimalPixelMoldDataType;

/* \/ =============== \/ *\
|  \/ Grid Pixel Data \/  |
\* \/ =============== \/ */

/**
 * Stores data about a pixel.
 */
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
		SetTargetPart(TargetPartOfPixel);
		SetCurrentPart(PartOfPixel);
		SetIntact(bPixelIntact);
	}

	//Constructs a FGridPixelData using a part.
	FGridPixelData(UPart* TargetPartOfPixel, bool bPixelIntact = false)
	{
		if (!IsValid(TargetPartOfPixel))
		{
			TargetPartOfPixel = UPart::GetNullPart();
		}
		UPart* PartOfPixel = TargetPartOfPixel;

		SetTargetPart(TargetPartOfPixel);
		SetCurrentPart(PartOfPixel);
		SetIntact(bPixelIntact);
	}

	//Constructs a FGridPixelData using a part.
	FGridPixelData(bool bPixelIntact = false)
	{
		SetTargetPart(UPart::GetNullPart());
		SetCurrentPart(UPart::GetNullPart());
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
		if (bNewIntact != bIntact)
		{			
			Temperature = 0;
		}

		if (!bNewIntact && GetTargetPart() != UPart::GetNullPart())
		{
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
	 * Gets the temperature of this pixel.
	 * 
	 * @return - The temperature of this pixel
	 */
	float GetTemperature()
	{
		return Temperature;
	}

	/**
	 * Gets whether or not this is curently the target part.
	 */
	bool IsTargetPart()
	{
		return bIntact && CurrentPart == TargetPart;
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

		if (NewPart != UPart::GetNullPart() && CurrentPart != NewPart && IsValid(NewPart) && IsValid(NewPart->GetData()) && IsValid(NewPart->GetData()->Texture))
		{
			if (!Material->IsValidLowLevel())
			{
				Material = UMaterialInstanceDynamic::Create(LoadObject<UMaterialInterface>(NULL, TEXT("/Game/2DAssets/Parts/Mat_BaseFreeformPart.Mat_BaseFreeformPart"), NULL, LOAD_None, NULL), NewPart->GetOuter(), FName(TEXT("Pixel_Mat")));
			}
			Material->SetTextureParameterValue(TEXT("PartTexture"), NewPart->GetData()->Texture);
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

/* /\ =============== /\ *\
|  /\ Grid Pixel Data /\  |
\* /\ =============== /\ */





/* \/ ============== \/ *\
|  \/ Voidgrid State \/  |
\* \/ ============== \/ */

/**
 * Stores all data required to replicate a voidgrid.
 */
USTRUCT()
struct VOIDSINGER_API FVoidgridState
{
	GENERATED_BODY()

public:
	//Stores the mold of the voidgrid
	UPROPERTY()
	TSet<FMinimalPartInstanceData> Mold = MinimalPixelMoldDataType();

	//Stores the state of the voidgrid's pixels.
	UPROPERTY()
	TSet<FPartInstanceData> State = TSet<FPartInstanceData>();

	/**
	 * Initilizes a voidgrid state using its mold and pixel state.
	 */
	FVoidgridState(MinimalPixelMoldDataType VoidgridMold = MinimalPixelMoldDataType(), TSet<FPartInstanceData> VoidgridPixelState = TSet<FPartInstanceData>())
	{
		Mold = VoidgridMold;
		State = VoidgridPixelState;
	}
};


//Hash function for FVoidgridState
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FVoidgridState& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FVoidgridState& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FVoidgridState));
	return Hash;
}
#endif


/* /\ =============== /\ *\
|  /\ Grid Pixel Data /\  |
\* /\ =============== /\ */





/* \/ ======== \/ *\
|  \/ Voidgrid \/  |
\* \/ ========= \/ */

//Used for dispatching events requireing a grid locaiton.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGridLocationDelegate, FIntPoint, GridLocaction, bool, bApplyChangeEffect, UPart*, Part);
//Used for dispatching events requireing mass information.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMassDelegate, float, Mass, FVector2D, CenterOfMass, float, MomentOfInertia);
//Used for dispatching simple events with no data.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGeneralDelegate);

UCLASS(Config = "Physics")
class VOIDSINGER_API AVoidgrid : public APawn
{
	GENERATED_BODY()

public:
	friend UThrustManager;

	//Sets default values for this voidgrid's properties
	AVoidgrid();

	//Used to update location and thrust control.
	virtual void Tick(float DeltaTime) override;

	/* ------------- *\
	\* \/ Physics \/ */

	FMassDelegate OnMassChanged;

	/**
	 * Pushes this voidgrid in the direction of Impulse with the force of |Impulse|.
	 *
	 * @param RelativeImpulse - The impluse to apply to this voidgrid in relative space.
	 * @param GridImpulseLocation - The location on the part grid to apply the impulse at.
	 */
	UFUNCTION(BlueprintCallable)
	void AddImpulse(FVector2D Impulse, FVector WorldImpulseLocation);
	/**
	 * Pushes this voidgrid in the direction of Impulse with the force of |Impulse|.
	 *
	 * @param Impulse - The impluse to apply to this voidgrid in world space.
	 * @param WorldImpulseLocation - The location in world space to apply the impulse at.
	 */
	void AddImpulse(FVector2D RelativeImpulse, GridLocationType GridImpulseLocation = GridLocationType::ZeroValue);

	/**
	 * Gets the instantaneous linear velocity of a point on this Voidgrid
	 *
	 * @param Location - The location of the point to get the velocity of.
	 */
	UFUNCTION(BlueprintPure)
	FVector2D GetVelocityOfPoint(FVector2D Location);

private:
	/**
	 * Updates the voidgrids location and rotation by its velocity. Also sweeps for collisions and computes new velocities
	 */
	UFUNCTION()
	void UpdateTransform(float DeltaTime);

	/**
	 * Checks for collisions along this ships path to a new transfrom.
	 *
	 * @param DeltaTransform - The change in transform needed to get to the new transform.
	 * @param Hit - The first blocking hit from this voidgrid colliding with another voidgrid.
	 * @return Whether or not this voidgrid collided with another voidgrid.
	 */
	bool SweepShip(const FTransform& DeltaTransform, FHitResult& Hit);

	/**
	 * Updates Mass, CenterOfMass, MomentOfInertia because of a change in mass.
	 * 
	 * @param DeltaMass - The change in mass of this.
	 * @param MassLocation - The location of the mass that changned on this.
	 */
	UFUNCTION()
	void UpdateMassProperties(float DeltaMass, FVector2D MassLocation);

	//Stores the linear velocity of this voidgrid. Measured in GridSquares/Second.
	UPROPERTY(VisibleInstanceOnly)
	FVector2D LinearVelocity;

	//Stores the angular velocity of this voidgrid. Mesured in Radians/Second.
	UPROPERTY(VisibleInstanceOnly)
	float AngularVelocity;

	//Stores the mass of this voidgrid.
	UPROPERTY()
	float Mass{ 0 };

	//Stores the mass of this voidgrid.
	UPROPERTY()
	FVector2D CenterOfMass{ FVector2D::ZeroVector };

	//Stores the moment of inertia of this voidgrid.
	UPROPERTY(Config)
	float MomentOfInertia;

	//Stores the maximum linear velocity of this voidgrid.
	UPROPERTY(Config)
	float MaxLinearVelocity{ 10000000 };

	//Stores the maximum linear velocity of this voidgrid.
	UPROPERTY(Config)
	float MaxAngularVelocity{ PI * 6 };

	//Stores the percent of energy conserved when coliding
	UPROPERTY(Config, meta=(ClampMin = "0", ClampMax = "1"))
	float CollisionElasticity{ 1 };

	//Stores the default Collision Channel used to test for collisions with other voidgrids.
	UPROPERTY()
	TEnumAsByte<ECollisionChannel> VoidgridCollsionChanel{ ECollisionChannel::ECC_PhysicsBody };

	/* /\ Physics /\ *\
	\* ------------- */

	/* ----------------- *\
	\* \/ Temperature \/ */

public:

	// \/ Add temperature \/ /

	/**
	 * Applys the temperature given at the world location given
	 * 
	 * @param Location - The world location to apply the temperature
	 * @param Temperature - The temperature to add
	 */
	UFUNCTION(BlueprintCallable)
	void AddTemperatureAtLocation(FVector WorldLocation, float Temperature);

	/**
	 * Applys the temperature given at the location given on this Voidgrid
	 * 
	 * @param Location - The location to apply the temperature
	 * @param Temperature - The temperature to add
	 */
	void AddTemperatureAtLocation(FIntPoint Location, float Temperature);

	// /\ Add temperature /\ /

private:

	/**
	 * Spreads the heat on the Voidgrid
	 */
	UFUNCTION()
	void SpreadHeat();

protected:

	//The time between heat ticks
	UPROPERTY(EditDefaultsOnly)
	float HeatTick = 1;

	//The percent of heat that a pixel will spread to all surrounding pixels
	UPROPERTY(EditDefaultsOnly)
	float HeatPropagationFactor = 0.5;

private:

	//The time since the last heat tick
	UPROPERTY()
	float DeltaHeatTime;

	//The locations that need their heat transfered to surrounding location and how much heat the location had
	UPROPERTY()
	TMap<FIntPoint, float> LocationsToTemperaturesPendingHeatTransfer;
	
	/* /\ Temperature /\ *\
	\* ----------------- */

	/* ---------------- *\ 
	\* \/ Pixel Mold \/ */

public:
	//Called when this is damaged.
	FGridLocationDelegate OnPixelRemoved;
	//Called when this is repaired.
	FGridLocationDelegate OnPixelAdded;

	/**
	 * Gets the grid loction of a world loction.
	 *
	 * @param WorldLocation - The world location to transform.
	 * @return The grid loction of WorldLocation;
	 */
	UFUNCTION(BlueprintPure)
	FIntPoint TransformWorldToGrid(FVector WorldLocation) const;

	/**
	 * Gets the world location of a grid loction.
	 *
	 * @param GridLoction - The grid location to transform.
	 * @return The world loction of GridLoction;
	 */
	UFUNCTION(BlueprintPure)
	FVector TransformGridToWorld(FIntPoint GridLocation) const;

	/**
	 * Sets the pixel mold of the voidgrid
	 * 
	 * @param NewPixelMold - The value to assign to the pixel mold of the voidgrid
	 */
	UFUNCTION(BlueprintCallable)
	void SetPixelMold(TSet<FMinimalPartInstanceData> NewPixelMold);

	/**
	 * Gets the minimal part data for all parts on this voidgrid.
	 * 
	 * @return The minimal part data for all parts on this voidgrid.
	 */
	UFUNCTION(BlueprintPure)
	TSet<FMinimalPartInstanceData> GetPixelMold();

	/**
	 * Sets the state of this voidgrid.
	 *
	 * @param NewState - The state to make this voidgrid match.
	 */
	void SetState(FVoidgridState NewState);

	/**
	 * Gets the state of this voidgrid.
	 *
	 * @return The state of this voidgrid.
	 */
	FVoidgridState GetState();

	/**
	 * Removes a pixel.
	 * 
	 * @param Location - The location of the pixel to damage.
	 */
	void RemovePixel(GridLocationType Location);

	/**
	 * Repair a pixel.
	 *
	 * @param Location - The location of the pixel to repair.
	 */
	void RepairPixel(GridLocationType Location);

	/**
	 * Repairs a random pixel.
	 */
	void RepairPixel();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UThrustManager> ThrustManagerClass;

	UPROPERTY()
	UThrustManager* ThrustManager;

private:
	/**
	 * Set pixel intact
	 *
	 * @param Location - The location of the pixel to edit.
	 * @param bNewIntact - The new integrity of the pixel.
	 */
	void SetPixelIntact(GridLocationType Location, bool bNewIntact, bool bApplyChangeEffect = true);

	/**
	 * Set pixel target
	 *
	 * @param Location - The location of the pixel to edit.
	 * @param NewTarget - The new target of the pixel.
	 */
	void SetPixelTarget(GridLocationType Location, UPart* NewTarget);
		
	/**
	 * Removes all parts from this voidgrid.
	 * Does not call OnDamaged.
	 */
	void ClearVoidgrid();

	//Stores the Pixel Mold of this.
	PixelMoldType LocationsToPixelState;

	//Stores the target parts and shape of this.
	TSet<FMinimalPartInstanceData> TargetParts;

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

	//Stores number of mesh sections that have been created.
	UPROPERTY()
	int32 MeshSectionCounter { 0 };

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

/* /\ ========= /\ *\
|  /\ Voidgrid /\  |
\* /\ ========= /\ */

/* \/ ========= \/ *\
|  \/ AVoidgrid \/  |
\* \/ ========= \/ */

