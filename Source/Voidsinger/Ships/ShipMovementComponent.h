//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "ShipMovementComponent.generated.h"
//
//class UBaseThrusterPart;
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class VOIDSINGER_API UShipMovementComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//	// Sets default values for this component's properties
//	UShipMovementComponent();
//
//protected:
//	// Called when the game starts
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//	//Comment -Mabel Suggestion
//	//Should this be blueprint callable? 
//	UFUNCTION(BlueprintCallable)
//	void UpdateThrusters();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void RotateShip(bool Clockwise, float Throttle);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void Move(FVector2D Direction, float Throttle);
//	
//	//Comment -Mabel Suggestion
//	//Tolerance is spelled wrong.
//	//Does this need to be public?
//	UFUNCTION(BlueprintPure)
//	const float GetLookDirectionErrorTollerance();
//
//	//Comment -Mabel Suggestion
//	//Tolerance is spelled wrong.
//	//Does this need to be public?
//	UFUNCTION(BlueprintPure)
//	const float GetMoveSpeedErrorTollerance();
//
//	//Comment -Mabel Suggestion
//	//Does this need to be public?
//	UFUNCTION(BlueprintPure)
//	const float GetRotationDirectionUpdateThreshold();
//
//	//Comment -Mabel Suggestion
//	//Does this need to be public?
//	UFUNCTION(BlueprintPure)
//	const float GetRotationDecelerationTargetDuration();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	TSet<UBaseThrusterPart*> GetThrustersForDirection(FVector2D Direction);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	TSet<UBaseThrusterPart*> GetThrustersForRotation(bool Clockwise);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	const float GetMaximumAccelerationInDirection(FVector2D Direction, float AtThrottle = 1);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	const float GetMaximumAccelerationInRotation(bool Clockwise, float AtThrottle = 1);
//
//protected:
//	//In radians
//	//Comment better -Mabel Suggestion
//	//Tolerance is spelled wrong. -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float LookDirectionErrorTollerance{ 0.02 };
//
//	//Comment -Mabel Suggestion
//	//Tolerance is spelled wrong. -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float MoveSpeedErrorTollerance{ 0.5 };
//
//	//The threshold at which ships alternate between accelerating and decelerating what trying to look in a direction
//	//? did you mean when? Also, I don't understand what this comment means. -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float RotationDirectionUpdateThreshold{ 0.1 };
//
//	//The time which the algorithm allots for decelerating
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float RotationDecelerationTargetDuration{ 0.25 };
//
//private:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	TSet<UBaseThrusterPart*> Thrusters;
//
//	//Comment -Mabel Suggestion
//	//Why are these last 2 variables and not return values? -Mabel Suggestion
//	TMap<bool, TSet<UBaseThrusterPart*>> ThrustersForRotation;
//
//	//Comment -Mabel Suggestion
//	TMap<FVector2D, TSet<UBaseThrusterPart*>> ThrustersForDirection;
//};
