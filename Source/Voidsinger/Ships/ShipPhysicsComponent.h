// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h" //remove this once debuging is compleate
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

	UFUNCTION(BlueprintPure)
	FVector2D GetVelocity();

	UFUNCTION(BlueprintPure)
	float GetAngularVelocity();
private:

	//declare private variables
	//UPROPERTY()
	ABaseShip* Ship;

	UPROPERTY()
	FVector2D LinearVelocity;

	UPROPERTY()
	FVector2D DeltaVelocity;

	UPROPERTY()
	FVector2D CenterOfThrust;

	UPROPERTY()
	float AngularVelocity;

};