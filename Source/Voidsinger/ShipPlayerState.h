// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VoidsingerTypes.h"
#include "Parts/BasePart.h"
#include "ShipPlayerState.generated.h"

/**
 * 
 */

UCLASS()
class VOIDSINGER_API AShipPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	TGridMap<TSubclassOf<UBasePart>> ShipBlueprint;

	/*UFUNCTION()
	TArray<FPartData> GetParts();*/
};
