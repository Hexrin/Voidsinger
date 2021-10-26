// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
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
	Electricity		UMETA(DisplayName = "Electricity"),
	Ammo			UMETA(DisplayName = "Ammo"),
};

UENUM(BlueprintType)
enum EVoidsongVerbs
{
	Activate		UMETA(DisplayName = "Activate"),
	Deactivate		UMETA(DisplayName = "Deactivate"),
	GitGud			UMETA(DisplayName = "GitGud"),
	BecomNube		UMETA(DisplayName = "BecomNube"),
	Hot				UMETA(DisplayName = "Hot"),
	Cold			UMETA(DisplayName = "Cold"),
	Absorb			UMETA(DisplayName = "Absorb"),
	Disperse		UMETA(DisplayName = "Disperse"),

};

//Faction types. Enemy will probably be changed but it works for now.
UENUM(BlueprintType)
enum EFactions
{
	Player			UMETA(DisplayName = "Player"),
	Enemy			UMETA(DisplayName = "Enemy"),
	Inanimate		UMETA(DisplayName = "Inanimate")
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FArrayBounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint LowerBounds = FIntPoint(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint UpperBounds = FIntPoint(0);

	FArrayBounds()
	{
		LowerBounds = FIntPoint(0, 0);
		UpperBounds = FIntPoint(0, 0);
	}

	FArrayBounds(FIntPoint Lower, FIntPoint Upper)
	{
		LowerBounds = Lower;
		UpperBounds = Upper;
	}

	FArrayBounds(int32 LowerX, int32 LowerY, int32 UpperX, int32 UpperY)
	{
		LowerBounds = FIntPoint(LowerX, LowerY);
		UpperBounds = FIntPoint(UpperX, UpperY);
	}
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FIntPointArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIntPoint> IntPointArray;

	FIntPointArray()
	{
		FIntPointArray(TArray<FIntPoint>(0,0));
	}


	FIntPointArray(TArray<FIntPoint> Array)
	{
		IntPointArray = Array;
	}
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FUBasePartArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBasePart*> UBasePartArray;

	FUBasePartArray()
	{
		FUBasePartArray(TArray<UBasePart*>(0,0));
	}


	FUBasePartArray(TArray<UBasePart*> Array)
	{
		UBasePartArray = Array;
	}
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
	float PartRotation;

	FSavePartInfo()
	{
		FSavePartInfo(nullptr, FIntPoint(), 0);
	}

	FSavePartInfo(TSubclassOf<UBasePart> Class, FIntPoint Location, float Rotation)
	{

		PartClass = Class;
		PartLocation = Location;
		PartRotation = Rotation;
	}
};

USTRUCT(BlueprintType)
struct FTextureDataTable : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;

};