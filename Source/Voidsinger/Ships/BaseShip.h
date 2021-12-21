// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Parts/PartGridComponent.h"
#include "ShipPhysicsComponent.h"
#include "ShipMovementComponent.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Voidsinger/Voidsongs/BaseNounVoidsong.h"
#include "Voidsinger/Voidsongs/BaseVerbVoidsong.h"
#include "Voidsinger/Voidsongs/BaseWhoVoidsong.h"
#include "ProceduralMeshComponent.h"
#include "BaseShip.generated.h"

class UBaseResourceSystem;

//Outdated delagate should be deleted - Liam Suggestion
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLaserDelegate, float, DamageMultiplier, float, DurationMultiplier);

//A delegate for brodcasting events that need a refernce to a part.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPartDataEventDelagate, UBasePart*, Part);

//Delegates should be commented. What type of functions should they be bound to? - Liam 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddVoidsongDelegate, UBaseVoidsong*, AddedVoidsong);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActivatePartsDelegate, const TArray<TSubclassOf<UObject>>&, NounClasses, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnsetVerbsDelegate);

UCLASS()
class VOIDSINGER_API ABaseShip : public APawn
{
	GENERATED_BODY()

public:

	/*-Initializers-*\
	\*--------------*/

	// Sets default values for this pawn's properties
	ABaseShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*-Tick-*\
	\*------*/

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*-Resource Managment-*\
	\*--------------------*/

	/*
	* Outdated name.
	* Should be GetResourceCapacity(TEnumAsByte<EResourceType> ResourceType)
	* Comment should be updated to match.
	* 
	* Update enum to not require TEnumAsByte<>.
	* 
	* Comment Confusing.
	* It gets storage capacity not capacity
	* It gets the storage capacity of all systems on the ship combined.
	* 
	* - Liam Suggestion
	*/
	//Gets the maximum fluid capacity of the given fluid.
	UFUNCTION(BlueprintPure)
	float const GetFluidCapacity(TEnumAsByte<EResourceType> Fluid);

	/*
	* Outdated name.
	* Should be GetFluidAmount(TEnumAsByte<EResourceType> ResourceType)
	* Comment should be updated to match.
	* 
	* Update enum to not require TEnumAsByte<>
	* 
	* Comment Confusing.
	* What does amount mean in this context?
	* It gets the storage capacity of all systems on the ship combined.
	* - Liam Suggestion
	*/ 
	//Gets the amount of a given fluid.
	UFUNCTION(BlueprintPure)
	float const GetFluidAmount(TEnumAsByte<EResourceType> Fluid);
	
	//Adds a resource system to the systems on the ship.
	UFUNCTION()
	void AddResourceSystem(UBaseResourceSystem* System);

	//Removes a resource system from the systems on the ship.
	UFUNCTION()
	void RemoveResourceSystem(UBaseResourceSystem* System);

	//Gets all the resource systems on the ship.
	UFUNCTION(BlueprintPure)
	TArray<UBaseResourceSystem*> GetResourceSystems();

	/*-Voidsong Control-*\
	\*------------------*/

	//Adds a new Voidsong to the AvailableVoidsongs. Will be useful for gaining a new voidsong.
	UFUNCTION(BlueprintCallable)
	void AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong);

	//UE doesnt like int, use int32 or int64 instead. - Liam Suggestion	
	//Plays a voidsong with a given activation sequence
	UFUNCTION()
	void PlaySequence(TArray<int> Sequence);

	/*
	* UE doesnt like int, use int32 or int64 instead.
	* 
	* Confusing Fucntion Name.
	* What does it mean to decide a voidsong? 
	* What is the return value?
	* 
	* Confusing Comment.
	* Comment is vague. What does "figues out" and "played" mean?
	* What is this function returning?
	* What are each of the parameters used for/ what is supposed to be input?
	* 
	* Strange Parameter Types.
	* Factions should be a TArray<UBaseFactionVoidsong*>& type for consistancy
	* Nouns should be a TArray<UBaseNounVoidsong*>& type for consistancy
	* - Liam Suggestion
	*/
	//Figures out what Voidsongs are played.
	UFUNCTION()
	float DecideVoidsongsPlayed(TArray<int> Sequence, TArray<TEnumAsByte<EFactions>>& Factions, TArray<TSubclassOf<UObject>>& Nouns, TArray<UBaseVerbVoidsong*>& Verbs);

	/*
	* Confusing Fucntion Name.
	* What is a duration delay?
	* Judgeing by the name I'd think this function would return the duration of a delay for a voidsong.
	* Consider Renaming to EndVoidsong() or similar
	*
	* Confusing Comment.
	* The domment should not say when the function is used but what it does.
	* - Liam Suggestion
	*/
	//This function is for the delay of the Voidsong duration
	UFUNCTION()
	void DurationDelay();

	//Consider swapping the TArray for a TSet as duplicate Voidsongs should not be possible - Liam Suggestion
	//Creates Voidsong objects with the given classes of Voidsongs and adds them to the AvaialableVoidsongs. Will be useful for loading from a save game.
	UFUNCTION(BlueprintCallable)
	void LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs);

	/*
	* Noun classes dosnt make sense in this context.
	* Either rename the parameter to PartClasses or, 
	* rename the function to something realted to Noun Voidsongs, maybe BrodcastPlayNouns.
	* 
	* Souldn't NounClasses be a TArray<TSubclassOf<UBasePart>>&? Especially if the function is called activate *parts*.
	* - Liam Suggestion
	*/
	//Event dispatcher for activating the parts on the ship.
	UFUNCTION(BlueprintCallable)
	void BroadcastActivateParts(const TArray<TSubclassOf<UObject>>& NounClasses, float Duration);

	/*-Ship Creation-*\
	\*---------------*/
	/*
	* The functionality of a function should be entierly self condained and thus should not rely on a varable being set to a certain value.
	* ClassCurrentlyEditing should be a parameter of this function.
	* - Liam Suggestion
	*/
	//Saves the current part grid to the class defaults of the ClassCurrentlyEditing.
	UFUNCTION(BlueprintCallable)
	void SaveEditorShip();

	/*-Misc-*\
	\-------*/

	UPROPERTY(BlueprintAssignable)
	FPartDataEventDelagate OnDamaged;

	UPROPERTY(BlueprintReadOnly)
	int32 PixelValue { 0 };

	//Gets the faction of the ship
	UFUNCTION(BlueprintPure)
	TEnumAsByte<EFactions> GetFaction();

	//Outdated disbatcher should be deleted - Liam Suggestion
	//Event dispatcher for laser.
	UFUNCTION(BlueprintCallable)
	void CallLaser(float DamageMultiplier, float DurationMultiplier);

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*-Resource Managment-*\
	\*--------------------*/

	//An array of all the resource systems on the base ship
	UPROPERTY()
	TArray<UBaseResourceSystem*> ResourceSystems;

	/*-Voidsong Managment-*\
	\*--------------------*/

	/*
	* Comment phrasing wierd
	* Consider "Stores the voidsongs that this ship can play" or something similar.
	* 
	* If suggestion on lines 11-18 of BaseVoidsong.h are followed then
	* this should be TSet<TSubclassOf<UBaseVoidsong>>
	* - Liam Suggestion
	*/
	//Array of the Voidsongs that are available to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBaseVoidsong*> AvailableVoidsongs;

	/*
	* Comment unclear
	* Reader does not need to know where this is called but what it stores.
	* Since it stores a delegate the reader should know when the delagate is broadcast
	* - Liam Suggestion
	*/
	//For the add voidsong event dispatcher
	UPROPERTY(BlueprintAssignable)
	FAddVoidsongDelegate OnAddVoidsongDelegate;

	/*
	* Comment unclear
	* Reader does not need to know what the functiosn bound to this do but what it stores.
	* Since it stores a delegate the reader should know when the delagate is broadcast
	* 
	* Why does this delegate exist?
	* It is only called on BroadcastActivateParts() which itself is never called
	* - Liam Suggestion
	*/
	//For firing parts on the ship
	UPROPERTY(BlueprintAssignable)
	FActivatePartsDelegate OnActivatePartsDelegate;

private:
	//bool property should start with b - Liam Suggestion
	//Whether the ship can activate another Voidsong
	UPROPERTY()
	bool CanActivateVoidsong = true;

public:

	/*-Ship Creation-*\
	\*---------------*/

	/*
	* Comment/name Unclear
	* What does it mean to create a ship?
	* Does setting this to false prevent ships from being spawned?
	* Does it have to do with saving ship designs? If so then the name or comment should mention saving.
	* - Liam Suggestion
	*/
	//This allows you to create ships. Should set this to false once we create all the ships.
	UPROPERTY(BlueprintReadWrite)
	bool bCanCreateShips = true;

	/*
	* The functionality of a function should be entierly self condained and thus should not rely on a varable being set to a certain value.
	* ClassCurrentlyEditing should be a parameter of SaveEditorShips().
	* - Liam Suggestion
	*/
	//The class that is currently being edited. This is the class that the SaveEditorShips() will change.
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ABaseShip> ClassCurrentlyEditing;


	/*
	* Delegate needs comment.
	* Reader needs to know what it stores.
	* Since it stores a delegate the reader should know when the delagate is broadcast.
	* - Liam Suggestion
	*/
	UPROPERTY(BlueprintAssignable)
	FUnsetVerbsDelegate OnUnsetVerbsDelegate;

	/*-Liam, you haven't commented things in a while-*\
	\*-----------------------------------------------*/

	//Comment -Mabel Suggestion
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* MeshComponent;

	//Comment -Mabel Suggestion
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPartGridComponent* PartGrid;

	//Comment -Mabel Suggestion
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UShipPhysicsComponent* PhysicsComponent;

	//Comment -Mabel Suggestion
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UShipMovementComponent* MovementComponent;

	//Outdated disbatcher should be deleted - Liam Suggestion
	//For the laser Event Dispatcher
	UPROPERTY(BlueprintAssignable)
	FLaserDelegate OnLaserDelegate;

	/*
	* Comment needed*.
	* Reader needs to know what it stores.
	* 
	* *Low priority as it should be replaced by upcomming blueprint system.
	* - Liam suggestion
	*/ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSavePartInfo> DefaultParts;


	/*-Movement Control-*\
	\*------------------*/
public:

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintCallable)
	void SetTargetMoveDirection(FVector2D Vector);

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	FVector2D GetTargetMoveDirection();

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintCallable)
	void SetTargetMoveSpeed(float Vector);

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	float GetTargetMoveSpeed();

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintCallable)
	void SetTargetLookDirection(FVector Vector);

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const FVector GetTargetLookDirection();
	
private:

	//Comment -Mabel Suggestion
	UPROPERTY()
	FVector TargetLookDirection;

	//Comment -Mabel Suggestion
	UPROPERTY()
	FVector2D TargetMoveDirection;

	//Comment -Mabel Suggestion
	UPROPERTY()
	bool bCurrentRotationDeccelerationStatus{ true };

	//Comment -Mabel Suggestion
	UPROPERTY()
	float TargetMoveSpeed{ 0 };

	/*---Mesh Control---*\
	\*------------------*/
public:

	//Comment -Mabel Suggestion
	UFUNCTION()
	void AddMeshAtLocation(FIntPoint Location);

	//Comment -Mabel Suggestion
	UFUNCTION()
	void RemoveMeshAtLocation(FIntPoint Location);

	//Comment -Mabel Suggestion
	UFUNCTION()
	void SetMeshRelativeLocation(FVector2D Location);

	//Comment -Mabel Suggestion
	UFUNCTION()
	void SetMeshMaterialAtLocation(FIntPoint Location, UMaterialInterface* Material);

private:

	//Comment -Mabel Suggestion
	//Function name unclear. Mentions vertices, but where exactly is it getting the vertices from? Sure, there may be meshes
	//around, but what meshes would it get the vertices from? Do you have like a variable storing vertices? 
	//As an aside, I'm wondering if mesh handling should be a separate class. I don't really know for sure. 
	//If only the mesh component ever cares about these functions, then we may actually want to make this part of a
	//separate component. And that may actually make sense. Think about it. -Mabel Suggestion
	UFUNCTION()
	TArray<FVector> GetVerticesAroundLocation(FVector2D Location);

	//Comment -Mabel Suggestion
	//Because I happen to know that you were making meshes, I know what this means. but otherwise this function name is 
	//confusing. What does creating triangles for a square have to do with a base ship? -Mabel Reed
	UFUNCTION()
	TArray<int32> CreateTrianglesForSquare(int32 UpperRight = 0, int32 UpperLeft = 1, int32 LowerRight = 2, int32 LowerLeft =3);

	//Comment -Mabel Suggestion
	UPROPERTY()
	TMap<FIntPoint, int32> MeshData;

	//Comment -Mabel Suggestion
	UPROPERTY()
	TArray<FVector2D> UV;

public:

	/*-Misc Variables-*\
	\*----------------*/

	//The faction of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EFactions> Faction;

};
