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

/*
* The resouce sytem appears to be not account for storage parts.
* The resouce sytem should either:
* Not handel quantities of resouces and just monitor production and consumption and let the storage parts handle if they can be drawn from or stored to
* or it should have a seperate variable for storage capacity and clamp the amount to that capacity.
* 
* The resouce sytem appears to not account for lost resouces do to taking damage.
* 
* The resouce sytem appears to not account for part resouce priortization.
* eg: Lets say that you have both a laser and a ion thruster atached to the same power grid but there is only enough production to power one of these parts
* and both parts are trying tor draw resoruces simultaneously; which part is gets the resources?
* - Liam Suggestion 
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

	//Comment not helpful. Comment should describe what a function does. - Liam Suggestion
	//Constructor
	UBaseResourceSystem();


	/*--Getter Functions--*\
	\*--------------------*/

	//Update enum to not require TEnumAsByte<>. - Liam Suggestion
	//Gets the resource type of the resource systen
	UFUNCTION(BlueprintPure)
	TEnumAsByte<EResourceType> GetType();

	//Comment not helpful. Comment should describe what a function does. - Liam Suggestion
	//GetWorld() functionality
	virtual class UWorld* GetWorld() const override;

	/*
	* Coment outdated. It is no longer a map.
	* 
	* Comment Unclear. Which UBaseParts?
	* - Liam Suggestion
	*/ 
	//Gets a FIntPoint to FPartData map from a list of UBaseParts. FIntPoints will be the pixel locations of the UBaseParts. 
	TGridMap<FPartData> GetMapFromConnectedParts();

	//Comment needed. What does the function do and what does it return? - Liam Suggestion
	UFUNCTION(BlueprintPure)
	float GetResourceAmount();

	/*--Setter Functions--*\
	\*--------------------*/

	//Comment needed. What does the function do? - Liam Suggestion
	UFUNCTION(BlueprintCallable)
	void SetType(TEnumAsByte<EResourceType> Type);


	/*--Resource Management Functions--*\
	\*---------------------------------*/

	//Comment phrasing weird. Consider rephrasing to "Increases the amount of resouces in the sytem by Amount." or something simalar - Liam Suggestion
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

	/*
	* Comment Issues
	* Do not mention private members of a class. Insead explain the effect of the function on the behavior of the class.
	* If a differnt function is perferable to this function than why does this function exist, and in what condition should you use RemovePart vs RemovePixel.
	* - Liam Suggestion
	*/
	//Removes a part from the variable storing all the parts. Call RemovePixel over this if possible.
	UFUNCTION()
	void RemovePart(UBasePart* RemovedPart);

	//"Call this during gameplay" is implied and unessary. - Liam Suggestion
	//Removes a pixel from the resource system. Checks the resource system for breaks in the system. Call this during gameplay.
	UFUNCTION(BlueprintCallable)
	void RemovePixel(FIntPoint Pixel);
	
	//Name/Comment inconsitancy. If this function adds parts than it should be called AddParts. - Liam Suggestion
	//Adds multiple parts to the resource system
	void AddSection(TGridMap<FPartData> AddedResourceGrid);

	/*--System and Break Management Functions--*\
	\*-----------------------------------------*/

	//Merge another system into this one. Essentially moves the parts from the other system to this system.
	UFUNCTION(BlueprintCallable)
	void MergeSystems(UBaseResourceSystem* MergedSystem);

	//Creates a new resource system with the parts inputted into this function
	void CreateNewSystem(TGridMap<FPartData> ResourceGrid, EResourceType Type);

	UFUNCTION()
	void DestroyResourceSystem();

	ABaseShip* GetOwningShip();

protected:

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*Instanced  Variables*\
	\*--------------------*/

	//Stores the correct amount of resource
	UPROPERTY()
	float ResourceAmount;

	//Stores all the parts that are connected to this system
	UPROPERTY()
	TArray<UBasePart*> ConnectedParts;
	
	//Stores all the locations that are connected to the resource system and what part corresponds to what location.
	TGridMap<FPartData> ResourceSystemGrid;

	//Stores the type of this system
	UPROPERTY()
	TEnumAsByte<EResourceType> SystemType;

	ABaseShip* OwningShip;
};
