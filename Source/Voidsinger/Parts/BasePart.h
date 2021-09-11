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
enum EPartRotation;

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

	//Used to inialize variables
	UFUNCTION()
	void Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType);

protected:
	//Begin Play for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void BeginPlay();




	/*--------Tick--------*\
	\*--------------------*/
public:
	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Determins if this part ticks
	virtual bool IsTickable() const override;

	//Is a nesseary part of tick component
	virtual TStatId GetStatId() const override;

protected:
	//Event Tick for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void EventTick(float DeltaTime);

	




	/*--Getter Functions--*\
	\*--------------------*/
public:
	//Gets the desired shape of the part ignoring any damage the part may have taken
	UFUNCTION(BlueprintPure)
	const TArray<FIntPoint> GetDesiredShape();
	const TArray<FIntPoint> GetDesiredShape(TEnumAsByte<EPartRotation> Rot);

	//Gets the curent shape of the part accounting for damage
	UFUNCTION(BlueprintPure)
	const TArray<FIntPoint> GetShape();

	//Gets the outer bounds of the part
	UFUNCTION(BlueprintPure)
	const FArrayBounds GetPartBounds();
	const FArrayBounds GetPartBounds(TEnumAsByte<EPartRotation> Rot);

	//Gets the location of the origin of the part relative to the part grid
	UFUNCTION(BlueprintPure)
	const FIntPoint GetLocation();

	//Gets the rotaion of the part
	UFUNCTION(BlueprintPure)
	const TEnumAsByte<EPartRotation> GetRotation();

	//Gets the mass of a single pixel for this part
	UFUNCTION(BlueprintPure)
	float GetMass();

	//Gets the part grid that this part is attached to
	UFUNCTION(BlueprintPure)
	const UPartGridComponent* GetPartGrid();

	//----------idk what this do so Mabel should write this----------------
	UFUNCTION(BlueprintPure)
	TArray<UBaseResourceSystem*> GetSystems();

	//----------idk what this do so Mabel should write this----------------
	UFUNCTION(BlueprintPure)
	UBaseResourceSystem* GetSystemByType(TEnumAsByte<EResourceType> Type);

	//----------idk what this do so Mabel should write this----------------
	UFUNCTION(BlueprintPure)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> GetResourceTypes();




	/*Condtional  Checkers*\
	\*--------------------*/
public:
	//returns true if there is atleast one functional pixel
	UFUNCTION()
	bool IsFunctional();

	//returns true if pixel at loc is functional
	UFUNCTION()
	bool IsPixelFunctional(FIntPoint Loc);




	/*---Misc. Functions--*\
	\*--------------------*/
public:
	//Remove a pixel form the actual shape of the part
	UFUNCTION(BlueprintCallable)
	void DestroyPixel(FIntPoint RelativeLoc);

	//----------idk what this do so Mabel should write this----------------
	UFUNCTION(BlueprintCallable)
	void CreateNewSystem(TEnumAsByte<EResourceType> ResourceType);

	//----------idk what this do so Mabel should write this----------------
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




	/*Function Return Vals*\
	\*--------------------*/
private:
	//Stores the curent shape of the part acounting for damage and rotation
	UPROPERTY()
	TArray<FIntPoint> ActualShape;

	//Stores the rotated version of DesiredShape 
	UPROPERTY()
	TArray<FIntPoint> RotatedShape;

	//Stores the bounds of the part
	UPROPERTY()
	FArrayBounds Bounds;

	//Stores weather or not the part is functional
	UPROPERTY()
	bool bFunctional;


	/*Instanced  Variables* \
	\*--------------------*/
	//Stores this parts rotation
	UPROPERTY()
	TEnumAsByte<EPartRotation> Rotation;

	//Stores this parts location relative to the part grid
	UPROPERTY()
	FIntPoint Location;






	
	//----------idk what this do so Mabel should write this and decide what catagory this falls under----------------
	UPROPERTY()
	TArray<UBaseResourceSystem*> Systems;
	//----------idk what this do so Mabel should write this and decide what catagory this falls under----------------
	UPROPERTY()
	UPartGridComponent* PartGridComponent;
	//----------idk what this do so Mabel should write this and decide what catagory this falls under----------------
	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ResourceTypes;

	
};
