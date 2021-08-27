// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/BasePart.h"
#include "UObject/NoExportTypes.h"


USTRUCT(BlueprintType)
struct VOIDSINGER_API FPartData
{
	GENERATED_BODY()

		//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//class UBasePart* Part;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Temperature;
};

UENUM(BlueprintType)
enum EDifficulty
{
	Easy			UMETA(DisplayName = "Easy"),
	Medium			UMETA(DisplayName = "Medium"),
	Hard			UMETA(DisplayName = "Hard"),
	VeryHard		UMETA(DisplayName = "Very Hard"),
};

UENUM(BlueprintType)
enum EFluidType
{
	Fuel			UMETA(DisplayName = "Fuel"),
	Hydrogen		UMETA(DisplayName = "Hydrogen"),
	InertGas		UMETA(DisplayName = "InertGas"),
	Coolent			UMETA(DisplayName = "Coolent"),
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FTravelCost
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EFluidType, float> FluidCosts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ElectricityCost;

};