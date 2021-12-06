// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Voidsinger/Voidgrids/Gridmap.h"
#include "VoidsingerTypes.generated.h"

class UBasePart;

//Comment -Mabel Suggestion
//Are we even using this enum anymore? -Mabel Suggestion
UENUM(BlueprintType)
enum EDifficulty
{
	Easy			UMETA(DisplayName = "Easy"),
	Medium			UMETA(DisplayName = "Medium"),
	Hard			UMETA(DisplayName = "Hard"),
	VeryHard		UMETA(DisplayName = "Very Hard"),
};

//Comment -Mabel Suggestion
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

//Faction types. Enemy will probably be changed but it works for now.
UENUM(BlueprintType)
enum EFactions
{
	Player			UMETA(DisplayName = "Player"),
	Enemy			UMETA(DisplayName = "Enemy"),
	Inanimate		UMETA(DisplayName = "Inanimate")
};

//Comment -Mabel Suggestion
//Is this ever used outside of the part grid? If not, move it to the part grid -Mabel Suggestion
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

//Comment -Mabel Suggestion
USTRUCT(BlueprintType)
struct VOIDSINGER_API FSavePartInfo
{
	GENERATED_BODY()

	//Comment -Mabel Suggestion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasePart> PartClass;

	//Comment -Mabel Suggestion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint PartLocation;

	//Comment -Mabel Suggestion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PartRotation;

	//Comment -Mabel Suggestion
	FSavePartInfo()
	{
		FSavePartInfo(nullptr, FIntPoint(), 0);
	}

	//Comment -Mabel Suggestion
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
