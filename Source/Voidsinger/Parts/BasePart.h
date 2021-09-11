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
	



	//Initializer Funtions\\
	|----------------------|
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




	//--------Tick--------\\
	|----------------------|
public:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

protected:
	//Event Tick for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void EventTick(float DeltaTime);

	




	//--Getter Functions--\\
	|----------------------|
public:
	//Gets the desired shape of the part ignoring any damage the part may have taken
	UFUNCTION(BlueprintPure)
	const TArray<FIntPoint> GetDesiredShape();
	const TArray<FIntPoint> GetDesiredShape(TEnumAsByte<EPartRotation> Rot);

	//Gets the outer bounds of the part
	UFUNCTION(BlueprintPure)
	const FArrayBounds GetPartBounds();
	const FArrayBounds GetPartBounds(TEnumAsByte<EPartRotation> Rot);

	//Gets the curent shape of the part accounting for damage
	UFUNCTION(BlueprintPure)
	const TArray<FIntPoint> GetShape();

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




	//Condtional  Checkers\\
	|----------------------|
public:
	//returns true if there is atleast one functional pixel
	UFUNCTION()
	bool IsFunctional();

	//returns true if pixel at loc is functional
	UFUNCTION()
	bool IsPixelFunctional(FIntPoint Loc);


	//---Misc. Functions--\\
	|----------------------|
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


	
protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FIntPoint> DesiredShape;

	UPROPERTY(EditDefaultsOnly, NoClear)
	float TotalMass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Cost;


private:

	UPROPERTY()
	TArray<FIntPoint> ActualShape;

	UPROPERTY()
	TArray<FIntPoint> RotatedShape;

	UPROPERTY()
	FArrayBounds Bounds;

	UPROPERTY()
	TEnumAsByte<EPartRotation> Rotation;

	UPROPERTY()
	FIntPoint Location;

	UPROPERTY()
	bool Functional;

	UPROPERTY()
	TArray<UBaseResourceSystem*> Systems;
	
	UPROPERTY()
	UPartGridComponent* PartGridComponent;

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ResourceTypes;

	
};
