// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Voidsinger/FunctionLibrary.h"
#include "ShipPhysicsComponent.generated.h"

class ABaseShip;


//Comment the class? -Mabel
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class VOIDSINGER_API UShipPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	//Comment -Mabel
	UShipPhysicsComponent();

protected:

	//Comment -Mabel
	virtual void BeginPlay() override;

public:	

	//Comment -Mabel
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//declare functions
	//Ah yes. declare functions. Amazing comment -Mabel

	//Comment -Mabel
	UFUNCTION(BlueprintCallable)
	void AddForce(FVector2D ForceLocation, FVector2D Force);

	//Comment -Mabel
	UFUNCTION(BlueprintCallable)
	void AddImpulse(FVector2D Impulse, FVector2D RelativeImpulseLocation);

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	FVector2D GetVelocity();

	//Comment -Mabel
	//Should this be public?
	UFUNCTION(BlueprintCallable)
	void SetLinearVelocity(FVector2D NewVelocity);

	//Comment -Mabel
	//Should this be public?
	UFUNCTION(BlueprintCallable)
	void SetAngularVelocity(float NewVelocity);

	//Comment -Mabel
	UFUNCTION()
	void UpdateMassCalculations();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	bool SweepShip(const FTransform& NewTransform, FHitResult& Hit);

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	float GetAngularVelocity();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	float GetMass();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	float GetMomentOfInertia();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	FVector2D GetCenterOfMass();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	FVector2D GetVelocityOfPoint(FVector2D RelativePointLocation);

private:

	//declare private variables
	// Declare private variables. 10/10 comment -Mabel
	 
	//Comment -Mabel
	//UPROPERTY()
	ABaseShip* Ship;

public:

	//Comment -Mabel
	//Magic number
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MaxLinearVelocity{ 1000000 };

	//Comment -Mabel
	//Magic number
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MaxAngularVelocity{ 10000 };

	//Comment -Mabel
	//Magic number
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MinLinearVelocity{ 0.25 };

	//Comment -Mabel
	//Magic numbers!!!
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MinAngularVelocity{ 0.025 };

	//Comment -Mabel
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float CollisionElasticity{ 0 };

private:

	//Comment -Mabel
	UPROPERTY()
	float Mass;

	//Comment -Mabel
	UPROPERTY()
	FVector2D CenterOfMass;

	//Comment -Mabel
	UPROPERTY()
	float MomentOfInertia;

	//Comment -Mabel
	UPROPERTY()
	float AngularVelocity;

	//Comment -Mabel
	UPROPERTY()
	FVector2D LinearVelocity;

	//Comment -Mabel
	UPROPERTY()
	float AngularAcceleration;

	//Comment -Mabel
	UPROPERTY()
	FVector2D LinearAcceleration;

	//Debug
	//UPROPERTY()
	//FColor DebugColor;

	
};
