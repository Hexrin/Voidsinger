//// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoidsingerTypes.h"
#include "DrawDebugHelpers.h"
#include "FunctionLibrary.generated.h"

/* -------------------------- *\
\* \/ Forward Declarations \/ */

class UBaseVerbVoidsong;

/* /\ Forward Declarations /\ *\
\* -------------------------- */

/**
 * General function library
 */
UCLASS()
class VOIDSINGER_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/**
	 * Plays Verbs Voidsongs with the given information.
	 * 
	 * @param Verbs - The verbs that were played
	 * @param Instigator - The object that activated these Voidsongs
	 * @param Hit - The hit result information 
	 * @param Effectiveness - The effectiveness of the activation
	 */
	UFUNCTION(BlueprintCallable)
	static void ExecuteVerbVoidsongs(const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, UObject* Instigator, FHitResult Hit, float Effectiveness);

//	////Used for the creation widget. Avoid using this at runtime, it's quite a performance hit. 
//	//UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Child Classes", Keywords = "Get Child Classes"), Category = Class)
//	//static TArray<UClass*> GetClasses(UClass* ParentClass);
//
//	//Explode at a global location, affecting all part grids in the radius
//	//UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
//	//static void ExplodeAtWorldLocation(const UObject* WorldContextObject, FVector WorldLocation, float ExplosionRadius);
//		
//	//This will return a bit based on the location given. The location given should be relative to the pixel in question.
//	//So -1, -1 will be the bottom left, or 16.
//	UFUNCTION()
//	static int GetBitNumberFromLocation(FIntPoint Location);
//
//	//UFUNCTION(BlueprintPure)
//	//static TArray<UPart*> GetPartsHitFromWorldLocation(FVector Location, UPartGridComponent* PartGrid);
//
};
