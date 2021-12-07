// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/VoidGameMode.h"
#include "Voidsinger/Voidgrids/GridMap.h"
#include "Voidsinger/FunctionLibrary.h"
#include "BasePart.generated.h"

/**
 *  Comment what the class is -Mabel Suggestion
 */

class UBaseResourceSystem;
class UBaseThrusterPart;
//class UPartGridComponent;
struct FPartData;

USTRUCT(BlueprintType)
struct VOIDSINGER_API FPartTransform
{
	GENERATED_BODY()

	//Stores the Location in IntPoint form for accessablity in blueprints. Do not use in C++.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Location;

	//Stores the location of the part.
	GridLocationType GridLocation;

	//Stores the rotation of the part.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rotation;

	//Constucts a part transform with the given location and a rotation snaped to 90 degree intervals.
	FPartTransform(GridLocationType Loc = GridLocationType(0, 0), float Rot = 0.f)
	{
		Location = Loc;
		GridLocation = Loc;
		Rotation = FMath::Fmod(FMath::GridSnap(Rot, 90.f), 90.f);
	}
};

typedef TSet<GridLocationType> PartShapeType;

UCLASS(BlueprintType, Blueprintable)
class VOIDSINGER_API UBasePart : public UObject, public FTickableGameObject, public IActivateInterface
{

	GENERATED_BODY()


	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             FUNCTIONS             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*Initializer Functions*\
	\*--------------------*/

public:

	//Constructor
	//If you're gonna get on me about "constructor" being a bad comment then i'm gonna get on you about that -Mabel Suggestion
	UBasePart();

	//The functions "InitializeVariables" and "InitializeFunctionality" feel a bit off. Granted, I'm not totally sure what's
	// off about them, but something feels off about those. It feels like they break encapsulation somehow. -Mabel Suggestion
	
	//Used to initialize variables. Called before part is placed onto the part grid
	//Should this be in the constructor then? Hmm except you can't have arguments to the constructor. Might be worth looking into
	//the c++ equivilent of "expose on spawn". 
	//Why is this needed? wouldn't you only need the location/rotation/etc when you're placing the part anyway, in which case
	// you can use whatever the "place part" function is? or is this the "place part" function? If this is only
	//for internal use, then this should be a private function. -Mabel Suggestion
	UFUNCTION()
	void InitializeVariables(FIntPoint Loc, float Rot, /*UPartGridComponent* PartGrid,*/ TSubclassOf<UBasePart> PartType);

	//Used to initize funtionality once the part has been placed onto the part grid
	//This is called from part grid then, after it's placed? asking for clarification -Mabel Suggestion
	UFUNCTION()
	void InitializeFunctionality();
	
	//Used to get world. Also gives child BPs acces to the Kismet Systems Library
	//"Comment not helpful. What does the GetWorld() functionality mean? - Liam Suggestion" -Mabel Suggestion
	UWorld* GetWorld() const;

	//Destroys the part. Does not call OnDestroyed()
	//Why would this not call "OnDestroyed"? It doesn't make sense to me that a function literally called "DestroyPart"
	// wouldn't call the thing that says "hey I was destroyed". 
	// Also, what other way is there for destroying parts that *would* call "OnDestroyed"? I'm confused. maybe this function
	// should be called "DestroyBecauseMerged" or something (horrible name, think of something better than that, but y'know)
	// SilentDestroyPart? DeletePart? (I'm kind of a fan of DeletePart) 
	// The name should suggest that it wouldn't call OnDestroyed() instead of heavily implying that it does call OnDestroyed().
	//-Mabel Suggestion
	UFUNCTION()
	void DestroyPart();

protected:

	//Begin Play for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void BeginPlay();


private:

	//Comment -Mabel Suggestion
	bool bHasFunctionalityBeenInitialized{ false };

	//Comment not helpful. What does it do? Which interface is it overriding - Liam Suggestion
	//Interface
	virtual void Activate() override;


	/*--------Tick--------*\
	\*--------------------*/

public:

	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Determines if this part will tick
	virtual bool IsTickable() const override;

	//Is a nesseary part of tick component
	virtual TStatId GetStatId() const override;

	/*
	* Comment not helpful.
	* What does it do?
	* When is it called?
	* 
	* Inconsitant name.
	* All other blueprint implementable events do not have blueprint in thier name.
	* - Liam Suggestion
	*/ 
	//Blueprint activate
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintActivate();

protected:

	//Event Tick for use in blueprint
	//Make blueprint display name "Tick" so it shows up as event tick? -Mabel Suggestion
	UFUNCTION(BlueprintImplementableEvent)
	void OnTick(float DeltaTime);

	/*--Getter Functions--*\
	\*--------------------*/

public:

	//Gets the desired shape of the part ignoring any damage the part may have taken
	//UFUNCTION(BlueprintPure)
	const PartShapeType GetDesiredShape();
	const PartShapeType GetDesiredShape(float Rot);

	//Gets the current shape of the part accounting for damage
	//UFUNCTION(BlueprintPure)
	const PartShapeType GetShape();

	//Gets the outer bounds of the part
	//Should the name of this function have the word "Part" in it? Isn't it implied that it would be the part bounds? -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const FArrayBounds GetPartBounds();
	const FArrayBounds GetPartBounds(float Rot);

	//Gets the location of the origin of the part relative to the part grid
	// Isn't it implied that it would be the origin of the part? -Mabel Suggestion
	
	//If this were being consistent with the other function names, it would be called "GetPartPartGridLocation". I don't think
	//these functions need the word "part" in their name. It should already be obvious that you are getting the part's
	//location because these functions are defined on the part. -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const FIntPoint GetPartGridLocation();

	//Gets the location of the origin of the part in world space
	UFUNCTION(BlueprintPure)
	const FVector GetPartWorldLocation();

	//Gets the location of the origin of the part in the local space of the ship it's a part of
	//Consider rephrasing: "Gets the location of the part relative to the ship" -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const FVector GetPartRelativeLocation();

	//Gets the rotaion of the part relative to the part grid
	//These rotation getters are not consistent with the location getters because they don't have the word "part" in them. 
	//Either the location getters or these rotation getters should be renamed for consistency. -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const float GetRelativeRotation();

	//Gets the rotation of the part in world space
	UFUNCTION(BlueprintPure)
	const float GetWorldRotation();

	//Gets the mass of a single pixel for this part
	//This is not intuitive because it's easy to assume that this would be the mass of the entire part. This should be renamed
	//to "GetPixelMass" or something along those lines. -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	float GetMass();

	//Gets the part grid that this part is attached to
	//This can be achieved with "GetOuter" and isn't a necessary function -Mabel Suggestion
	/*UFUNCTION(BlueprintPure)
	UPartGridComponent* GetPartGrid();*/

	//Gets the ship it's a part of
	//This could be achieved with "GetOuter()->GetOuter()" currently, though after our refactor it will probably just be
	//"GetOuter()", so this isn't a necessary function either -Mabel Suggestion
	//UFUNCTION(BlueprintPure)
	//const ABaseShip* GetShip();

	//Gets the resource systems the part is a part of
	UFUNCTION(BlueprintPure)
	TArray<UBaseResourceSystem*> GetSystems();

	//Comment technicaly false. It finds the *first* resource system ... - Liam suggestion
	//Returns the resource system that the part is a part of with a given type, returns null if there is no system with that resource type
	UFUNCTION(BlueprintPure)
	UBaseResourceSystem* GetSystemByType(TEnumAsByte<EResourceType> Type);

	//Confuseing name. The function gets more than just the resource types. Consider renaming to GetResourceInterfaceLocations()? - Liam Suggestion
	//Gets the locations and resource types of pixels of the part
	UFUNCTION(BlueprintPure)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> GetResourceTypes();

	//Gets the strength of the part
	UFUNCTION(BlueprintPure)
	const int GetStrength();

	//Gets the heat resistance of the part
	UFUNCTION(BlueprintPure)
	const int GetHeatResistance();

	//Gets the material used for this part's pixel
	//This is from half-complete code if i remember correctly. In any case, each pixel will have a different material/however we
	// end up doing it
	//so this function might not make sense -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	UMaterialInterface* GetPixelMaterial();

	/*Condtional  Checkers*\
	\*--------------------*/
public:

	//returns true if there is at least one functional pixel
	//This should be changed to if the part is well, functional. I would say that most parts would probably not be functional when
	//only one pixel is functional -Mabel Suggestion
	UFUNCTION()
	bool IsFunctional();

	//returns true if pixel at loc is functional on Part
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

	/*
	* Needs comment. What does it do? 
	* 
	* Confusing Name.
	* How is it differnt from AddToSystem?
	* Consider renaming to InitializeSytems()
	* - Liam Suggestion
	*/
	//What about ConnectToSurroundingSystems? This function doesn't "initialize" any systems. It just connects to already
	//existing systems -Mabel Suggestion
	UFUNCTION()
	void ConnectToSystems();

	//Creates a new resource system that the part is a part of
	UFUNCTION(BlueprintCallable)
	void CreateNewSystem(TEnumAsByte<EResourceType> ResourceType);

	//Adds the part to the given resource system
	UFUNCTION(BlueprintCallable)
	void AddToSystem(UBaseResourceSystem* System);

	/*
	* Function name bad?
	* DelegateCalled too broad a term.
	* What delagate is being called?
	* What is the function doing?
	* When is it being called?
	* 
	* Comment needs improving.
	* What does the function do?
	* How are the parameters used?
	* - Liam Suggestion
	*/
	//Function called when FVoidsongDelegate is broadcasted
	UFUNCTION(BlueprintCallable)
	void OnDelegateCalled(const TArray<TEnumAsByte<EFactions>>& Factions, const TArray<TSubclassOf<UObject>>& NounClasses, const TArray<UBaseVoidsong*>& AvailableVoidsongs);

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*-Blueprint Defaults-*\
	\*--------------------*/

protected:

	//Stores the default shape of the part 
	//Either this comment or the variable should be renamed. either "Stores the desired shape of the part" or "DefaultShape" -Mabel Suggestion
	//UPROPERTY(EditDefaultsOnly)
	PartShapeType DesiredShape;

	//Comment -Mabel Suggestion
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* PixelMaterial;

	//Stores the total mass of the part
	UPROPERTY(EditDefaultsOnly, NoClear, BlueprintReadOnly)
	float TotalPartMass;

	//Stores the cost to place the part in Pixels
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Cost;

	//Stores the locations and resource types of all the pixels of this part, unless the pixel does not have a resource type
	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ResourceTypes;

	//Strength of the part
	//Why is this comment calling "PartName" strength of the part -Mabel Suggestion
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PartName;

	//Strength of the part
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Strength;

	//Heat resistance of the part
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int HeatResistance;

	//Strength of the part
	//Why is this comment calling "PartIcon" strength of the part - Mabel Suggestion
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* PartIcon;

	//What percentage of pixels that have to be part of the part in order for the part to be functional
	//This should be used in the aforementioned "IsFunctional" function (if it already is then update the comment of "IsFunctional"
	//Also, this should probably be renamed to "RequiredPercentFunctional" or something along those lines. "PercentFunctional"
	//implies that this is the *current* PercentFunctional, not the required percentage to be functional.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PercentFunctional = 50;

	/*Function Return Vals*\
	\*--------------------*/

	//Stores the current shape of the part accounting for damage and rotation
	//UPROPERTY()
	PartShapeType ActualShape;

private:
	

	//Stores the rotated version of DesiredShape 
	// Why is this commented out? delete if it's not needed anymore -Mabel Suggestion
	//UPROPERTY()
	//TArray<FIntPoint> RotatedShape;

	//Stores the bounds of the part
	UPROPERTY()
	FArrayBounds Bounds;

	//Stores whether or not the part is functional
	UPROPERTY()
	bool bFunctional;

	//UPROPERTY()
	//Comment -Mabel Suggestion
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
	/*UPROPERTY()
	UPartGridComponent* PartGridComponent;*/

};
