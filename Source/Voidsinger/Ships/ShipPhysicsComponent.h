//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "Voidsinger/FunctionLibrary.h"
//#include "ShipPhysicsComponent.generated.h"
//
//class ABaseShip;
//
//
////Comment the class? -Mabel Suggestion
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
//class VOIDSINGER_API UShipPhysicsComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:	
//
//	//Comment -Mabel Suggestion
//	UShipPhysicsComponent();
//
//protected:
//
//	//Comment -Mabel Suggestion
//	virtual void BeginPlay() override;
//
//public:	
//
//	//Comment -Mabel Suggestion
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//
//	//declare functions
//	//Ah yes. declare functions. Amazing comment -Mabel Suggestion
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void AddForce(FVector2D ForceLocation, FVector2D Force);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void AddImpulse(FVector2D Impulse, FVector2D RelativeImpulseLocation);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	FVector2D GetVelocity();
//
//	//Comment -Mabel Suggestion
//	//Should this be public?
//	UFUNCTION(BlueprintCallable)
//	void SetLinearVelocity(FVector2D NewVelocity);
//
//	//Comment -Mabel Suggestion
//	//Should this be public?
//	UFUNCTION(BlueprintCallable)
//	void SetAngularVelocity(float NewVelocity);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION()
//	void UpdateMassCalculations();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	bool SweepShip(const FTransform& NewTransform, FHitResult& Hit);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	float GetAngularVelocity();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	float GetMass();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	float GetMomentOfInertia();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	FVector2D GetCenterOfMass();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	FVector2D GetVelocityOfPoint(FVector2D RelativePointLocation);
//
//private:
//
//	//declare private variables
//	// Declare private variables. 10/10 comment -Mabel Suggestion
//	 
//	//Comment -Mabel Suggestion
//	//UPROPERTY()
//	ABaseShip* Ship;
//
//public:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float MaxLinearVelocity{ 1000000 };
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float MaxAngularVelocity{ 10000 };
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float MinLinearVelocity{ 0.25 };
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float MinAngularVelocity{ 0.025 };
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
//	float CollisionElasticity{ 0 };
//
//private:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	float Mass;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	FVector2D CenterOfMass;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	float MomentOfInertia;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	float AngularVelocity;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	FVector2D LinearVelocity;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	float AngularAcceleration;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	FVector2D LinearAcceleration;
//
//	//Debug
//	//UPROPERTY()
//	//FColor DebugColor;
//
//	
//};
