// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShip.h"
<<<<<<< HEAD
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/StarSystemData.h"
#include "Camera/CameraComponent.h"
=======
>>>>>>> origin/ZachM
#include "PlayerShip.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API APlayerShip : public ABaseShip
{
	GENERATED_BODY()
	

	protected:
	int Pixels;

<<<<<<< HEAD
	UPROPERTY(EditAnywhere)
	float CameraHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStarSystemData* CurrentStarSystem;

public:
=======
	public:
>>>>>>> origin/ZachM
	APlayerShip();
	
	UFUNCTION(BlueprintPure)
	FTravelCost GetTravelCost(class UStarSystemData* Target);

	UFUNCTION(BlueprintCallable)
	bool TravelToStarSystem(class UStarSystemData* Target);

	
};
