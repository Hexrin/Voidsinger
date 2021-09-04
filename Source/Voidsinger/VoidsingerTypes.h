// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VoidsingerTypes.generated.h"

class UBasePart;

UENUM(BlueprintType)
enum EDifficulty
{
	Easy			UMETA(DisplayName = "Easy"),
	Medium			UMETA(DisplayName = "Medium"),
	Hard			UMETA(DisplayName = "Hard"),
	VeryHard		UMETA(DisplayName = "Very Hard"),
};

UENUM(BlueprintType)
enum EResourceType
{
	Fuel			UMETA(DisplayName = "Fuel"),
	Hydrogen		UMETA(DisplayName = "Hydrogen"),
	InertGas		UMETA(DisplayName = "InertGas"),
	Coolent			UMETA(DisplayName = "Coolent"),
	Electricity		UMETA(DisplayName = "Electricity")
};


USTRUCT(BlueprintType)
struct VOIDSINGER_API FPartData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBasePart* Part;

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Temperature = 0;
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FArrayBounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint LowerBounds = FIntPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint UpperBounds = FIntPoint();
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FTravelCost
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<EResourceType>, float> FluidCosts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ElectricityCost;

	FTravelCost(TMap<TEnumAsByte<EResourceType>, float> FCosts = TMap<TEnumAsByte<EResourceType>, float>(), float ECost = 0)
	{
		FluidCosts = FCosts;
		ElectricityCost = ECost;
	}


};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FIntPointArray
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIntPoint> IntPointArray;

	FIntPointArray()
	{
		FIntPointArray(TArray<FIntPoint>());
	}

	FIntPointArray(TArray<FIntPoint> Array)
	{
		IntPointArray = Array;
	}

};
UENUM(BlueprintType)
enum EPartRotation
{
	Degrees0		UMETA(DisplayName = "0 Degrees"),
	Degrees90		UMETA(DisplayName = "90 Degrees"),
	Degrees180		UMETA(DisplayName = "180 Degrees"),
	Degrees270		UMETA(DisplayName = "270 Degrees"),
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FSavePartInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasePart> PartClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint PartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EPartRotation> PartRotation;

	FSavePartInfo()
	{
		FSavePartInfo(nullptr, FIntPoint(), EPartRotation::Degrees0);
	}

	FSavePartInfo(TSubclassOf<UBasePart> Class, FIntPoint Location, TEnumAsByte<EPartRotation> Rotation)
	{

		PartClass = Class;
		PartLocation = Location;
		PartRotation = Rotation;
	}
};