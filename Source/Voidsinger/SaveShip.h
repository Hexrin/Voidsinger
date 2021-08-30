// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoidsingerTypes.h"
#include "GameFramework/SaveGame.h"
#include "SaveShip.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API USaveShip : public USaveGame
{
	GENERATED_BODY()

public:

	USaveShip();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSavePartInfo> SavedShip;

};
