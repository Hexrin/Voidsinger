// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartModule.h"
#include "Engine/DataAsset.h"
#include "PartData.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UPartData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<UPartModule*> Modules;
};
