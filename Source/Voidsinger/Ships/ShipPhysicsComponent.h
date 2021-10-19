// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Voidsinger/FunctionLibrary.h"
#include "ShipPhysicsComponent.generated.h"

//Forward Declaration read these: https://forums.unrealengine.com/t/circular-dependency-detected-for-filename/45749/2 & https://answers.unrealengine.com/questions/62540/calling-a-function-in-my-gamemode-from-an-actor-cy.html
class ABaseShip;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class VOIDSINGER_API UShipPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShipPhysicsComponent();

protected:

	virtual void BeginPlay() override;

public:	

	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//declare functions
	UFUNCTION(BlueprintCallable)
	void AddForce(FVector2D ForceLocation, FVector2D Force);

	UFUNCTION(BlueprintCallable)
	void AddImpulse(FVector2D Impulse, FVector2D RelativeImpulseLocation);

	UFUNCTION(BlueprintPure)
	FVector2D GetVelocity();

	UFUNCTION(BlueprintCallable)
	void SetLinearVelocity(FVector2D NewVelocity);

	UFUNCTION(BlueprintCallable)
	void SetAngularVelocity(float NewVelocity);

	UFUNCTION()
	void UpdateMassCalculations();

	UFUNCTION(BlueprintPure)
	bool SweepShip(const FTransform& NewTransform, FHitResult& Hit);

	UFUNCTION(BlueprintPure)
	float GetAngularVelocity();

	UFUNCTION(BlueprintPure)
	float GetMass();

	UFUNCTION(BlueprintPure)
	float GetMomentOfInertia();

	UFUNCTION(BlueprintPure)
	FVector2D GetCenterOfMass();

	UFUNCTION(BlueprintPure)
	FVector2D GetVelocityOfPoint(FVector2D RelativePointLocation);
private:

	//declare private variables
	//UPROPERTY()
	ABaseShip* Ship;

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MaxLinearVelocity{ 100000 };

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MaxAngularVelocity{ 10000 };

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MinLinearVelocity{ 0.05 };

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MinAngularVelocity{ 0.001 };

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float CollisionElasticity{ 0 };

private:
	UPROPERTY()
	float Mass;

	UPROPERTY()
	FVector2D CenterOfMass;

	UPROPERTY()
	float MomentOfInertia;

	UPROPERTY()
	float AngularVelocity;

	UPROPERTY()
	FVector2D LinearVelocity;

	UPROPERTY()
	float AngularAcceleration;

	UPROPERTY()
	FVector2D LinearAcceleration;

	UPROPERTY()
	FColor DebugColor;

	
};
