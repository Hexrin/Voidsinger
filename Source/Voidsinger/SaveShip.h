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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FIntPoint> PartShape;

	//Comment -Mabel Suggestion
	FSavePartInfo(TSubclassOf<UBasePart> Class = nullptr, FIntPoint Location = FIntPoint::ZeroValue, float Rotation = 0, TSet<FIntPoint> Shape = TSet<FIntPoint>())
	{

		PartClass = Class;
		PartLocation = Location;
		PartRotation = Rotation;
		PartShape = Shape;
	}

	FSavePartInfo(UBasePart* Part)
	{

		PartClass = Part->GetClass();
		PartLocation = Part->GetPartGridLocation();
		PartRotation = Part->GetRelativeRotation();
		PartShape = Part->GetShape();
	}

	bool operator==(const FSavePartInfo& Other) const
	{
		return PartClass.Get() == Other.PartClass.Get() && PartLocation == Other.PartLocation && PartRotation == Other.PartRotation;
	}
};

//Hash function for FSavePartInfo
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FSavePartInfo& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FSavePartInfo& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FSavePartInfo));
	return Hash;
}
#endif


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
	TSet<FSavePartInfo> SavedShip;

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
