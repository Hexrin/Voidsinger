//// Fill out your copyright notice in the Description page of Project Settings.
//
#pragma once
//
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "Engine/DataTable.h"
#include "Voidsinger/Voidgrids/Gridmap.h"
#include "VoidsingerTypes.generated.h"
//
//class UBaseResourceSystem;
//class UPart;
//
////Comment -Mabel Suggestion
////Are we even using this enum anymore? -Mabel Suggestion
//UENUM(BlueprintType)
//enum EDifficulty
//{
//	Easy			UMETA(DisplayName = "Easy"),
//	Medium			UMETA(DisplayName = "Medium"),
//	Hard			UMETA(DisplayName = "Hard"),
//	VeryHard		UMETA(DisplayName = "Very Hard"),
//};

//Resource types. 
UENUM(BlueprintType)
enum class EResourceType : uint8
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
enum class EFaction : uint8 
{
	Inanimate		UMETA(DisplayName = "Inanimate"),
	Player			UMETA(DisplayName = "Player"),
	Enemy			UMETA(DisplayName = "Enemy"),
};

////Comment -Mabel Suggestion
//USTRUCT(BlueprintType)
//struct VOIDSINGER_API FResourceSystemArray
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TArray<UBaseResourceSystem*> ResourceSystemArray;
//
//	FResourceSystemArray()
//	{
//		FResourceSystemArray(TArray<UBaseResourceSystem*>(0, 0));
//	}
//
//
//	FResourceSystemArray(TArray<UBaseResourceSystem*> Array)
//	{
//		ResourceSystemArray = Array;
//	}
//};
//

/**
 * Stores the an array of int32s
 */
USTRUCT(BlueprintType)
struct FIntArray
{
	GENERATED_BODY()


	//Stores an aray of integers
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> IntArray;

	/**
	 * Constucts an IntArray using a TArray.
	 */
	FIntArray(TArray<int32> Array = TArray<int32>())
	{
		IntArray = Array;
	}
};

/**
 * Stores the bounds of a 2D Box measured using Ints
 */
USTRUCT(BlueprintType)
struct VOIDSINGER_API FIntBoxBounds
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint LowerBounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint UpperBounds;

	FIntBoxBounds()
	{
		LowerBounds = FIntPoint::ZeroValue;
		UpperBounds = FIntPoint::ZeroValue;
	}

	FIntBoxBounds(FIntPoint Lower, FIntPoint Upper)
	{
		LowerBounds = Lower;
		UpperBounds = Upper;
	}

	FIntBoxBounds(int32 LowerX, int32 LowerY, int32 UpperX, int32 UpperY)
	{
		LowerBounds = FIntPoint(LowerX, LowerY);
		UpperBounds = FIntPoint(UpperX, UpperY);
	}
};
//
//USTRUCT(BlueprintType)
//struct VOIDSINGER_API FIntPointArray
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TArray<FIntPoint> IntPointArray;
//
//	TArray<FIntPoint> GetIntPointArray()
//	{
//		return IntPointArray;
//	}
//
//	FIntPointArray()
//	{
//		FIntPointArray(TArray<FIntPoint>(0,0));
//	}
//
//
//	FIntPointArray(TArray<FIntPoint> Array)
//	{
//		IntPointArray = Array;
//	}
//
//	
//};
//
//USTRUCT(BlueprintType)
//struct VOIDSINGER_API FUPartArray
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TArray<UPart*> UPartArray;
//
//	FUPartArray()
//	{
//		FUPartArray(TArray<UPart*>(0,0));
//	}
//
//
//	FUPartArray(TArray<UPart*> Array)
//	{
//		UPartArray = Array;
//	}
//};
//
////Comment -Mabel Suggestion
//USTRUCT(BlueprintType)
//struct VOIDSINGER_API FSavePartInfo
//{
//	GENERATED_BODY()
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TSubclassOf<UPart> PartClass;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FIntPoint PartLocation;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float PartRotation;
//
//	//Comment -Mabel Suggestion
//	FSavePartInfo()
//	{
//		FSavePartInfo(nullptr, FIntPoint(), 0);
//	}
//
//	//Comment -Mabel Suggestion
//	FSavePartInfo(TSubclassOf<UPart> Class, FIntPoint Location, float Rotation)
//	{
//
//		PartClass = Class;
//		PartLocation = Location;
//		PartRotation = Rotation;
//	}
//};
//
//USTRUCT(BlueprintType)
//struct FTextureDataTable : public FTableRowBase
//{
//
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	UTexture2D* Texture;
//
//};