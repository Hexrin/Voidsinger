////// Fill out your copyright notice in the Description page of Project Settings.
////
//#pragma once
////
//#include "CoreMinimal.h"
////#include "Components/ActorComponent.h"
////#include "Kismet/GameplayStatics.h"
////#include "Voidsinger/SaveShip.h"
////#include "DrawDebugHelpers.h"
//#include "Voidsinger/VoidsingerTypes.h"
////#include "Kismet/KismetMathLibrary.h"
////#include "UObject/ConstructorHelpers.h"
////#include "Misc/Char.h"
//#include "Part.h"
//#include "PartGridComponent.generated.h"
////
////class UBaseThrusterPart;
////class UCorePart;
////
////Comment the struct, what's this used for? -Mabel Suggestion
//USTRUCT(BlueprintType)
//struct VOIDSINGER_API FPartInfo
//{
//	GENERATED_BODY()
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	UPart* Part;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	UMaterialInstanceDynamic* DynamicMat;
//
//private:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	int32 AdjacencyIndex = 0;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	float Temperature = 0;
//
//public:
//
//	//Comment -Mabel Suggestion
//	FPartInfo()
//	{
//		FPartInfo(nullptr, 0, 0, nullptr);
//	}
//
//	//Comment -Mabel Suggestion
//	FPartInfo(UPart* PartRef, float Temp, int32 BitwiseIndex, UMaterialInterface* Material)
//	{
//		DynamicMat = UMaterialInstanceDynamic::Create(Material, Cast<UObject>(PartRef));
//		Part = PartRef;
//		SetTemperature(Temp);
//		SetAdjacencyIndex(BitwiseIndex);
//	}
//
//	//Comment -Mabel Suggestion
//	void SetTemperature(const float NewTemp)
//	{
//		Temperature = NewTemp;
//		DynamicMat->SetScalarParameterValue("Temperature", Temperature);
//	}
//
//	//Comment -Mabel Suggestion
//	const float GetTemperature()
//	{
//		return Temperature;
//	}
//
//	//Comment -Mabel Suggestion
//	void SetAdjacencyIndex(int32 NewAdjacencyIndex)
//	{
//		AdjacencyIndex = NewAdjacencyIndex;
//		DynamicMat->SetScalarParameterValue("Adjacency Index", AdjacencyIndex);
//	}
//
//	//Comment -Mabel Suggestion
//	const int32 GetAdjacencyIndex()
//	{
//		return AdjacencyIndex;
//	}
//};
//
////Comment the class maybe? -Mabel Suggestion
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
//class VOIDSINGER_API UPartGridComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//
//	// Sets default values for this component's properties
//	UPartGridComponent();
//
//protected:
//
//	// Called when the game starts
//	virtual void BeginPlay() override;
//
//public:	
//
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//	//---Ship Creation---
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	bool AddPart(TSubclassOf<UPart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace = false);
//	//Maybe the parameter shouldn't be called "PartialPartShape" becuase it might happen to be adding an entire part. It should probably be renamed to "PartShape" or "Shape" -Mabel Suggestion
//	bool AddPart(TSet<FIntPoint> PartialPartShape, TSubclassOf<UPart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace = false);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	bool RemovePart(FIntPoint Location, bool CheckForBreaks = true);
//
//	//---Destruction---
//
//	/*
//	* Function should be overloaded. (This is false due confusion about what the parameters do/are used for but I'm leaving it because its good info)
//	* Instead of having the FromExplosion parameter and then a bunch of other realted parameters,
//	* The function should be overloaded with two parameter sets that do differnt things
//	* 
//	* Encapulation Issue
//	* Destroy Pixel should have nothing to do with explosions. 
//	* 
//	* Confusing Parameters
//	* It seems like it will generate a new shape to be destoryed using the explosion info.
//	* - Liam Suggestion
//	*/
//	//Destroys a pixel at a given location.
//	//This might have too many parameters which is a sign of something spaghet but I added half of them so idk -Mabel Suggestion 
//	UFUNCTION(BlueprintCallable)
//	bool DestroyPixel(FIntPoint Location, class UPart*& DamagedPart, bool CheckForBreaks = true, bool FromExplosion = false, FVector ExplosionLocation = FVector(0, 0, 0), float ExplosionRadius = 0.0f);
//	bool DestroyPixel(FIntPoint Location, bool CheckForBreaks = true, bool FromExplosion = false, FVector ExplosionLocation = FVector(0, 0, 0), float ExplosionRadius = 0.0f);
//
//	/*
//	* Function should be overloaded. (This is false due confusion about what the parameters do/are used for but I'm leaving it because its good info)
//	* Instead of having the FromExplosion parameter and then a bunch of other realted parameters,
//	* The function should be overloaded with two parameter sets that do differnt things
//	* 
//	* OR
//	* 
//	* Create a one function for convering information about an explosion into a TArray<FIntPoint>.
//	* This function could likly be used in other functions
//	* 
//	* 
//	* Confusing parameters.
//	* It seems like it will generate a new shape to be removed using the explosion info.
//	* Explosion information should be a struct so that it can be constucted multiple ways and so it can be comprest to one parameter for beter readablilty.
//	* 
//	* Function name inaccurate.
//	* The function has not conectivity detection therefore it does not remove the *Disconnected* shape.
//	* Name does not communicate that it creates a new ship.
//	* Consider renaming to one of the following or similar:
//	* DisconectShape()
//	* CreateNewShipFromShape()
//	* - Liam Suggestion
//	*/
//	//Removes the given shape from the part grid and create a new base ship with them.
//	UFUNCTION()
//	void RemoveDisconnectedShape(TSet<FIntPoint> Shape, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void ApplyHeatAtLocation(FVector WorldLocation, float HeatToApply = 1);
//	void ApplyHeatAtLocation(FIntPoint RelativeLocation, float HeatToApply = 1);
//
//	/*
//	* You may want to move explosions to a new explosion manager class that would also handel ExplodeAtWorldLocation(), the FX for the explosion
//	* 
//	* Breaks encapsulation.
//	* This function should be private as you should never be able to explode only one ship.
//	* Explostions should only be triggerable via ExplodeAtWorldLocation().
//	* -Liam Suggestion
//	*/
//	//Explodes at the given location with the given radius.
//	UFUNCTION(BlueprintCallable)
//	void ExplodeAtLocation(FVector WorldLocation, float ExplosionRadius);
//
//	/*
//	* Should be static and in a math function library. 
//	* 
//	* Box infromation should be a struct so that it can be constucted multiple ways
//	* - Liam Suggestion
//	*/ 
//	//Checks if a 2d Rectangle contains the location given
//	UFUNCTION()
//	bool BoxContainsLocation(FVector2D TopLeft, FVector2D BottomRight, FVector2D Location);
//
//	/*
//	* Should be static and in a math function library.
//	* 
//	* Arbitrary return value. If you find yourself saying what each return value means then you are using the wrong return type.
//	* Consider cahngein the reurn type to one of the following:
//	* FIntPoint
//	* A custom enum
//	* A struct containing a custom enum that stores the relation to the X & Y Axisies
//	* 
//	* UE doesnt like int, use int32 or int64 instead. - Liam Suggestion	
//	* - Liam Suggestion
//	*/
//	//Returns the quadrant the location is in with respect to the origin. If the location is on an axis, then it will return 5 for positive Y, 6 for positive X, 7 for negative Y, and 8 for negative X. Remember that unreal is left-handed, so X is vertical and Y is horizontal. Returns 0 if the Location and origin are equal.
//	UFUNCTION()
//	int GetQuadrantFromLocation(FVector2D Location, FVector2D Origin);
//
//	/*
//	* Should be static and in a math function library.
//	* 
//	* Parameters wierd
//	* Box information should be a struct so that it can be constucted multiple ways so overloading becomes unnesseary
//	* Line information should be a struct so that it can be constucted multiple ways (preferably less unconventional ways) so overloading becomes unnesseary
//	* Just a XIntercept does not define a line.
//	* - Liam Suggestion
//	*/ 
//	//Returns true if the given line intersects the given 2d rectangle
//	UFUNCTION()
//	bool DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float SlopeRise, float SlopeRun, FVector2D origin);
//	bool DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float XIntercept);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	FPartInfo GetPartDataAtGridLocation(FIntPoint Location);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	FPartInfo GetPartDataAtRelativeLocation(FVector Location);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	FPartInfo GetPartDataAtWorldLocation(FVector Location);
//
//private:
//
//	//Comment -Mabel Suggestion
//	//"Distribute" is spelled incorrectly.
//	UFUNCTION()
//	void DistrubuteHeat();
//
//	//---Save Ship---
//public:
//	/*
//	* Name weird.
//	* Build implies an action by the player and conflucs with build mode
//	* Consider LoadPartGrid or SetPartGrid
//	* Update comment to match new name
//	* 
//	* Should this function be public or should it only be used in LoadSavedShip
//	* - Liam Suggestion
//	*/ 
//	//Builds a ship with the given parts
//	UFUNCTION(BlueprintCallable)
//	void BuildShip(TArray<FSavePartInfo> Parts);
//
//	//Saves a ship to a save game object
//	UFUNCTION(BlueprintCallable)
//	void SaveShip(FString ShipName);
//
//	//Loads a ship from a save game object
//	UFUNCTION(BlueprintCallable)
//	bool LoadSavedShip(FString ShipName);
//
//	//---Getters---
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	const FVector2D CalcCenterOfMass();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	const float CalcMomentOfInertia();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	const float CalcMass();
//
//	//Comment -Mabel Suggestion
//	//Why is this not blueprint pure but GetPartGridScale is?
//	//UFUNCTION(BlueprintPure)
//	TGridMap<FPartInfo> GetPartGrid();
//	
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	const float GetPartGridScale();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	const FIntBoxBounds GetPartGridBounds();
//
//	//Misc
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	bool PointsConnected(FIntPoint StartPoint, FIntPoint EndPoint, bool TestForFunctionality = false);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION()
//	void UpdateMaterials(FIntPoint Location, TSubclassOf<UPart> PartType);
//
//public:
//
//
//
//private:
//
//	//Comment -Mabel Suggestion
//	//GetOwner() or GetOuter() would accomplish the same thing here, so this variable may not be necessary. Also if we combine
//	//the part grid and base ship it won't be necessary. But if we do keep for some strange reason, it should probably be named
//	//"OwningBaseShip" or something like that, since "Ship" isn't super clear. -Mabel Suggestion
//	UPROPERTY()
//	ABaseShip* Ship;
//
//	//Comment -Mabel Suggestion
//	//It's a bit odd to have a variable named the same thing as the component. Makes it a bit confusing to say the least. -Mabel Suggestion
//	TGridMap<FPartInfo> PartGrid;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	FIntBoxBounds GridBounds;
//
//	//Comment -Mabel Suggestion
//	//You sure this should be edit anywhere and not edit defaults only?
//	UPROPERTY(EditAnywhere)
//	float GridScale; 
//
//	//Comment -Mabel Suggestion
//	//You sure this should be edit anywhere and not edit defaults only?
//	UPROPERTY(EditAnywhere)
//	FIntPoint GridHalfSize;
//	
//	//The path to the pixel mesh. 
//	UPROPERTY(EditAnywhere)
//	FString PathToPixelMesh = TEXT("/Game/3DAssets/PlaneWithCollision.PlaneWithCollision");
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	UStaticMesh* PixelMesh;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere)
//	float HeatTickRate;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta=(ClampMin="0", ClampMax="1"))
//	float HeatPropagationFactor;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
//	float HeatMeltTransferFactor;
//
//	//Comment -Mabel Suggestion
//	//TimeSinceHeatTick instead of Time*s*SinceHeatTick? 
//	UPROPERTY()
//	float TimesSinceHeatTick;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	UCorePart* CorePart;
//
//	//Comment -Mabel Suggestion
//	UFUNCTION()
//	bool const CanShapeFit(FIntPoint Loc, TSet<FIntPoint> DesiredShape);
//
//	/*Static Functions*\
//	\*----------------*/
//public:
//	//Function should be moved to TGridMap<> - Liam Suggestion
//	//A recursive function that will check the shape it's provided with for any parts that are not connected to each other
//	static TSet<FIntPoint> FindConnectedShape(TSet<FIntPoint> Shape, TGridMap<FPartInfo> ConnectedPartsMap, bool CheckFunctionality = false);
//
//	//Comment -Mabel Suggestion
//	static bool IsPixelFunctional(FPartInfo PixelValue, FIntPoint Loc);
//};
//>>>>>>> Demo-Version
