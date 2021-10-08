// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PixelMesh;

	FPartData()
	{
		FPartData(nullptr, 0, nullptr);
	}

	FPartData(UBasePart* PartRef, float Temp, UStaticMeshComponent* MeshRef)
	{
		Part = PartRef;
		Temperature = Temp;
		PixelMesh = MeshRef;
	}

	void SetTemperature(const float NewTemp)
	{
		Temperature = NewTemp;
	}
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

	UPROPERTY()
	int Index;

	UPROPERTY()
	UTexture2D* Texture;

};