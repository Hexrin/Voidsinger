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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLaserDelegate, float, DamageMultiplier, float, DurationMultiplier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddVoidsongDelegate, UBaseVoidsong*, AddedVoidsong);

UCLASS()
class VOIDSINGER_API ABaseShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	float const GetFluidCapacity(TEnumAsByte<EResourceType> Fluid);

	UFUNCTION(BlueprintPure)
	float const GetFluidAmount(TEnumAsByte<EResourceType> Fluid);

	UFUNCTION()
	void AddResourceSystem(UBaseResourceSystem* System);

	UFUNCTION()
	void RemoveResourceSystem(UBaseResourceSystem* System);

	//Gets the faction of the ship
	UFUNCTION(BlueprintPure)
	TEnumAsByte<EFactions> GetFaction();

	//Adds a new Voidsong to the AvailableVoidsongs. Will be useful for gaining a new voidsong.
	UFUNCTION(BlueprintCallable)
	void AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong);

	//Plays a voidsong with a given activation sequence
	UFUNCTION()
	void PlaySequence(TArray<int> Sequence);

	//Figures out what Voidsongs are played.
	UFUNCTION()
	float DecideVoidsongsPlayed(TArray<int> Sequence, TArray<TEnumAsByte<EFactions>>& Factions, TArray<TSubclassOf<UObject>>& Nouns, TArray<UBaseVerbVoidsong*>& Verbs);

	//This function is for the delay of the Voidsong duration
	UFUNCTION()
	void DurationDelay();

	//Creates Voidsong objects with the given classes of Voidsongs and adds them to the AvaialableVoidsongs. Will be useful for loading from a save game.
	UFUNCTION(BlueprintCallable)
	void LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs);

	//Saves the current part grid to the class defaults of the ClassCurrentlyEditing.
	UFUNCTION(BlueprintCallable)
	void SaveEditorShip();

	//Event dispatcher for laser.
	UFUNCTION(BlueprintCallable)
	void CallLaser(float DamageMultiplier, float DurationMultiplier);

	UFUNCTION(BlueprintPure)
	TArray<UBaseResourceSystem*> GetResourceSystems();

	UPROPERTY()
	TArray<UBaseResourceSystem*> ResourceSystems;

	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPartGridComponent* PartGrid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UShipPhysicsComponent* PhysicsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UShipMovementComponent* MovementComponent;

	//Array of the voidsongs that are available to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBaseVoidsong*> AvailableVoidsongs;

	//For the laser Event Dispatcher
	UPROPERTY(BlueprintAssignable)
	FLaserDelegate OnLaserDelegate;

	//For the add voidsong event dispatcher
	UPROPERTY(BlueprintAssignable)
	FAddVoidsongDelegate OnAddVoidsongDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSavePartInfo> DefaultParts;

//Creating ship stuff

	//This allows you to create ships. Should set this to false once we create all the ships.
	UPROPERTY(BlueprintReadWrite)
	bool bCanCreateShips = true;

	//The class that is currently being edited. This is the class that the SaveEditorShips() will change.
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ABaseShip> ClassCurrentlyEditing;


	/*-Movement Control-*\
	\*------------------*/
public:
	UFUNCTION(BlueprintCallable)
	void SetTargetMoveDirection(FVector2D Vector);

	UFUNCTION(BlueprintPure)
	FVector2D GetTargetMoveDirection();

	UFUNCTION(BlueprintCallable)
	void SetTargetMoveSpeed(float Vector);

	UFUNCTION(BlueprintPure)
	float GetTargetMoveSpeed();

	UFUNCTION(BlueprintCallable)
	void SetTargetLookDirection(FVector Vector);

	UFUNCTION(BlueprintPure)
	const FVector GetTargetLookDirection();
	
private:
	UPROPERTY()
	FVector TargetLookDirection;

	UPROPERTY()
	FVector2D TargetMoveDirection;

	UPROPERTY()
	bool bCurrentRotationDeccelerationStatus{ true };

	UPROPERTY()
	float TargetMoveSpeed{ 0 };

	/*---Mesh Control---*\
	\*------------------*/
public:
	UFUNCTION()
	void AddMeshAtLocation(FIntPoint Location);

	UFUNCTION()
	void RemoveMeshAtLocation(FIntPoint Location);

	UFUNCTION()
	void SetMeshRelativeLocation(FVector2D Location);

	UFUNCTION()
	void SetMeshMaterialAtLocation(FIntPoint Location, UMaterialInterface* Material);

private:

	UFUNCTION()
	TArray<FVector> GetVerticesAroundLocation(FVector2D Location);

	UFUNCTION()
	TArray<int32> CreateTrianglesForSquare(int32 UpperRight = 0, int32 UpperLeft = 1, int32 LowerRight = 2, int32 LowerLeft =3);

	UPROPERTY()
	TMap<FIntPoint, int32> MeshData;

	UPROPERTY()
	TArray<FVector2D> UV;

public:

	//The faction of the ship
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EFactions> Faction;

private:

	//Whether the ship can activate another Voidsong
	UPROPERTY()
	bool CanActivateVoidsong = true;

};
