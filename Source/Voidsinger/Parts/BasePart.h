// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/FunctionLibrary.h"
#include "Voidsinger/Ships/BaseShip.h"
#include "Voidsinger/Parts/PartGridComponent.h"
#include "BasePart.generated.h"

/**
 * 
 */

class UBaseResourceSystem;

UCLASS(BlueprintType, Blueprintable)
class VOIDSINGER_API UBasePart : public UObject, public FTickableGameObject
{

	GENERATED_BODY()





	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             FUNCTIONS             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//
	



	/*Initializer Funtions*\
	\*--------------------*/

public:

	//Constructor
	UBasePart();

	//Used to inialize variables. Called before part is placed onto the part grid
	UFUNCTION()
	void InitializeVariables(FIntPoint Loc, float Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType);

	//Used to initize funtionality once the part has been placed onto the part grid
	UFUNCTION()
	void InitializeFunctionality();
	
	//Used to get world. Also gives child BPs acces to the Kismet Systems Library
	UWorld* GetWorld() const;

	//Destroys the part. Does not call the blueprint funciton
	UFUNCTION()
	void DestroyPart();

protected:
	//Begin Play for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void BeginPlay();

	


	/*--------Tick--------*\
	\*--------------------*/
public:
	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Determines if this part will tick
	virtual bool IsTickable() const override;

	//Is a nesseary part of tick component
	virtual TStatId GetStatId() const override;

protected:
	//Event Tick for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void OnTick(float DeltaTime);

	




	/*--Getter Functions--*\
	\*--------------------*/

public:

	//Gets the desired shape of the part ignoring any damage the part may have taken
	UFUNCTION(BlueprintPure)
	const TArray<FIntPoint> GetDesiredShape();
	const TArray<FIntPoint> GetDesiredShape(float Rot);

	//Gets the curent shape of the part accounting for damage
	UFUNCTION(BlueprintPure)
	const TArray<FIntPoint> GetShape();

	//Gets the outer bounds of the part
	UFUNCTION(BlueprintPure)
	const FArrayBounds GetPartBounds();
	const FArrayBounds GetPartBounds(float Rot);

	//Gets the location of the origin of the part relative to the part grid
	UFUNCTION(BlueprintPure)
	const FIntPoint GetLocation();

	//Gets the rotaion of the part
	UFUNCTION(BlueprintPure)
	const float GetRotation();

	//Gets the mass of a single pixel for this part
	UFUNCTION(BlueprintPure)
	float GetMass();

	//Gets the part grid that this part is attached to
	UFUNCTION(BlueprintPure)
	const UPartGridComponent* GetPartGrid();

	//Gets the resource systems the part is a part of
	UFUNCTION(BlueprintPure)
	TArray<UBaseResourceSystem*> GetSystems();

	//Returns the resource system that the part is a part of with a given type, returns null if there is no system with that resource type
	UFUNCTION(BlueprintPure)
	UBaseResourceSystem* GetSystemByType(TEnumAsByte<EResourceType> Type);

	//Gets the locations and resource types of pixels of the part
	UFUNCTION(BlueprintPure)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> GetResourceTypes();

	//Gets the strength of the part
	UFUNCTION(BlueprintPure)
	int GetStrength();



	/*Condtional  Checkers*\
	\*--------------------*/
public:

	//returns true if there is at least one functional pixel
	UFUNCTION()
	bool IsFunctional();

	//returns true if pixel at loc is functional
	UFUNCTION()
	bool IsPixelFunctional(FIntPoint Loc);




	/*---Misc. Functions--*\
	\*--------------------*/
public:

	//Called when a part is completely destroyed
	UFUNCTION(BlueprintImplementableEvent, meta=(ShortToolTip = "Called when a part is completely destroyed"))
	void OnDestroyed();

	//Called when a part is damaged
	UFUNCTION(BlueprintImplementableEvent, meta = (ShortToolTip = "Called when a part is damaged"))
	void OnDamaged();

	//Called when a part is damaged in such a way that it will no longer be functional
	UFUNCTION(BlueprintImplementableEvent, meta = (ShortToolTip = "Called when a part is damaged in such a way that it will no longer be functional"))
	void OnCriticallyDamaged();

	//Remove a pixel form the actual shape of the part
	UFUNCTION(BlueprintCallable)
	void DestroyPixel(FIntPoint RelativeLoc);

	//Creates a new resource system that the part is a part of
	UFUNCTION(BlueprintCallable)
	void CreateNewSystem(TEnumAsByte<EResourceType> ResourceType);

	//Adds the part to the given resource system
	UFUNCTION(BlueprintCallable)
	void AddToSystem(UBaseResourceSystem* System);












	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//



	/*-Blueprint Defaults-*\
	\*--------------------*/
protected:

	//Stores the default shape of the part
	UPROPERTY(EditDefaultsOnly)
	TArray<FIntPoint> DesiredShape;

	//Stores the total mass of the part
	UPROPERTY(EditDefaultsOnly, NoClear)
	float TotalPartMass;

	//Stores the cost to place the part in Pixels
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Cost;

	//Stores the locations and resource types of all the pixels of this part, unless the pixel does not have a resource type
	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ResourceTypes;

	UPROPERTY(EditDefaultsOnly)
	int Strength;

	/*Function Return Vals*\
	\*--------------------*/

private:
	//Stores the curent shape of the part acounting for damage and rotation
	UPROPERTY()
	TArray<FIntPoint> ActualShape;

	//Stores the rotated version of DesiredShape 
	//UPROPERTY()
	//TArray<FIntPoint> RotatedShape;

	//Stores the bounds of the part
	UPROPERTY()
	FArrayBounds Bounds;

	//Stores whether or not the part is functional
	UPROPERTY()
	bool bFunctional;

	UPROPERTY()
	bool bIsBeingDestroyed;

	/*Instanced  Variables*\
	\*--------------------*/

	//Stores this parts rotation
	UPROPERTY()
	float Rotation;

	//Stores this parts location relative to the part grid
	UPROPERTY()
	FIntPoint Location;
	
	//Stores all the resource systems this part is a part of
	UPROPERTY()
	TArray<UBaseResourceSystem*> Systems;

	//Stores a reference to the part grid component
	UPROPERTY()
	UPartGridComponent* PartGridComponent;
	
};
