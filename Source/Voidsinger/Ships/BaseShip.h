// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Parts/PartGridComponent.h"
#include "ShipPhysicsComponent.h"
#include "ShipMovementComponent.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "ProceduralMeshComponent.h"
#include "BaseShip.generated.h"

class UBaseResourceSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLaserDelegate, float, Damage, float, Duration);

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

	//Adds a new Voidsong to the AvailableVoidsongs. Will be useful for gaining a new voidsong.
	UFUNCTION(BlueprintCallable)
	void AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong);

	//Plays a voidsong with a given activation sequence
	UFUNCTION()
	void PlayVoidsong(TArray<int> Sequence);

	//Creates Voidsong objects with the given classes of Voidsongs and adds them to the AvaialableVoidsongs. Will be useful for loading from a save game.
	UFUNCTION(BlueprintCallable)
	void LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs);

	//Saves the current part grid to the class defaults of the ClassCurrentlyEditing.
	UFUNCTION(BlueprintCallable)
	void SaveEditorShip();

	//Event dispatcher for laser.
	UFUNCTION(BlueprintCallable)
	void CallLaser(float Damage, float Duration);

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

	//For the Event Dispatcher
	UPROPERTY(BlueprintAssignable)
	FLaserDelegate OnLaserDelegate;

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

	UPROPERTY(BlueprintReadWrite)
	FVector TargetLookDirection;

	UPROPERTY(BlueprintReadWrite)
	bool bDecelerating{ true };
private:
	UPROPERTY()
	FVector2D TargetMoveDirection;


	/*---Mesh Control---*\
	\*------------------*/
public:
	UFUNCTION()
	void AddMeshAtLocation(FIntPoint Location);

	UFUNCTION()
	void RemoveMeshAtLocation(FIntPoint Location);

	UFUNCTION()
	void SetMeshRelativeLocation(FVector2D Location);
	void SetMeshRelativeLocation();

private:
	UFUNCTION()
	TSet<FVector> GetVerticesAroundLocation(FVector2D Location);

	UFUNCTION()
	void AddTrianglesForSquare(int32 UpperRight, int32 UpperLeft, int32 LowerRight, int32 LowerLeft, FIntPoint Location);

	UPROPERTY()
	TArray<FVector> Vertices;

	UPROPERTY()
	TArray<int32> Triangles;

	TArray<TArray<FVector2D>> UVs;

	FVector2D RelativeMeshLocation;

	TMap<FIntPoint, int32> TriangleIndices;
};
