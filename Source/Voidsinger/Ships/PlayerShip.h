// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShip.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/StarSystemData.h"
#include "Camera/CameraComponent.h"
#include "PlayerShip.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API APlayerShip : public ABaseShip
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	int Pixels;

	UPROPERTY(EditAnywhere)
	float CameraHeight;

public:
	APlayerShip();
	
	UFUNCTION(BlueprintPure)
	FTravelCost GetTravelCost(StarSystemData Target);
};
