// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/FunctionLibrary.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PartGridComponent.h"
#include "BaseResourceSystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VOIDSINGER_API UBaseResourceSystem : public UObject
{
	GENERATED_BODY()
	
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             FUNCTIONS             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

public:

	/*Initializer Funtions*\
	\*--------------------*/

	//Constructor
	UBaseResourceSystem();


	/*--Getter Functions--*\
	\*--------------------*/

	//Gets the resource type of the resource systen
	UFUNCTION(BlueprintPure)
	TEnumAsByte<EResourceType> GetType();

	//GetWorld() functionality
	virtual class UWorld* GetWorld() const override;

	//Gets a FIntPoint to FPartData map from a list of UBaseParts. FIntPoints will be the pixel locations of the UBaseParts. 
	TGridMap<FPartData> GetMapFromConnectedParts();

	UFUNCTION(BlueprintPure)
	float GetResourceAmount();

	/*--Setter Functions--*\
	\*--------------------*/

	UFUNCTION(BlueprintCallable)
	void SetType(TEnumAsByte<EResourceType> Type);


	/*--Resource Management Functions--*\
	\*---------------------------------*/

	//Add an amount of resource
	UFUNCTION(BlueprintCallable)
	void AddResources(float Amount);

	//remove/use an amount of resource, bool returns if the amount was sucessfully drawn
	UFUNCTION(BlueprintCallable)
	bool DrawResources(float Amount);


	/*--Part Management Functions--*\
	\*-----------------------------*/

	//Adds a new part to the resource system
	UFUNCTION(BlueprintCallable)
	void AddPart(UBasePart* AddedPart);

	//Removes a part from the variable storing all the parts. Call RemovePixel over this if possible.
	UFUNCTION()
	void RemovePart(UBasePart* RemovedPart);

	//Removes a pixel from the resource system. Checks the resource system for breaks in the system. Call this during gameplay.
	UFUNCTION(BlueprintCallable)
	void RemovePixel(FIntPoint Pixel);

	//Adds multiple parts to the resource system
	UFUNCTION(BlueprintCallable)
	void AddSection(TArray<UBasePart*> AddedParts);

	//Removes multiple parts from the variable storing all the parts. Avoid this function if possible.
	UFUNCTION()
	void RemoveSection(TArray<UBasePart*> RemovedParts);


	/*--System and Break Management Functions--*\
	\*-----------------------------------------*/

	//Merge another system into this one. Essentially moves the parts from the other system to this system.
	UFUNCTION(BlueprintCallable)
	void MergeSystems(UBaseResourceSystem* MergedSystem);

	//Creates a new resource system with the parts inputted into this function
	UFUNCTION(BlueprintCallable)
	void CreateNewSystem(TArray<UBasePart*> RemovedParts);

	//Checks 2 shapes to see if they are adjacent. Deprecated for now.
	//UFUNCTION(BlueprintCallable)
	//bool AreShapesAdjacent(TArray<FIntPoint> Shape1, TArray<FIntPoint> Shape2);

protected:

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*Instanced  Variables*\
	\*--------------------*/

	//Stores the currect amount of resource
	UPROPERTY()
	float ResourceAmount;

	//Stores all the parts that are connected to this system
	UPROPERTY()
	TArray<UBasePart*> ConnectedParts;
	
	//Stores the type of this system
	UPROPERTY()
	TEnumAsByte<EResourceType> SystemType;

};
