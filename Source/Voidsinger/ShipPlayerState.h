// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Parts/PartGridComponent.h"
#include "ShipPlayerState.generated.h"

/**
 * 
 */
USTRUCT()
stuct VOIDSINGER_API FShipBlueprintInfo

UCLASS()
class VOIDSINGER_API AShipPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FPartGrid ShipBlueprint;

	UFUNCTION()
	TArray<FPartData> GetParts();
};
