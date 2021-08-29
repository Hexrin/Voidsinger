// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Parts/PartGridComponent.h"
#include "BaseShip.generated.h"

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

	//UFUNCTION(BlueprintPure)
	float const GetFluidCapacity(TEnumAsByte<EFluidType> Fluid);

	//UFUNCTION(BlueprintPure)
	float const GetFluidAmount(TEnumAsByte<EFluidType> Fluid);

	UFUNCTION(BlueprintPure)
	FVector2D GetCenterOfMass();

	UFUNCTION(BlueprintPure)
	float GetMass();

private:
	UPROPERTY(VisibleAnywhere)
	class UPartGridComponent* PartGrid;
};
