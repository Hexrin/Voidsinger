// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/BasePart.h"
#include "GameFramework/SaveGame.h"
#include "SaveShip.generated.h"

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

	bool operator==(const FSavePartInfo& Other) const
	{
		return PartClass.Get() == Other.PartClass.Get() && PartLocation == Other.PartLocation && PartRotation == Other.PartRotation;
	}
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int UserIndex;

};

UCLASS()
class VOIDSINGER_API UIsDamagedSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDamaged{ false };
};
