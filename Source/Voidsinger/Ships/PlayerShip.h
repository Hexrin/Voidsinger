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
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Pixels;

	UPROPERTY(EditAnywhere)
	float CameraHeight;

	UPROPERTY(VisibleAnywhere)
	class UStarSystemData* CurrentStarSystem;

public:
	APlayerShip();
	
	UFUNCTION(BlueprintPure)
	TMap<TEnumAsByte<EResourceType>, float> GetTravelCost(class UStarSystemData* Target);

	UFUNCTION(BlueprintCallable)
	bool TravelToStarSystem(class UStarSystemData* Target);

	UFUNCTION(BlueprintPure)
	class UStarSystemData* GetCurrentStarSystem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;
	
};
