// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartModule.h"
#include "Engine/DataAsset.h"
#include "PartData.generated.h"

/**
 * Stores information realating to a part type
 */
UCLASS()
class VOIDSINGER_API UPartData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	//Stores the name of this part as it appears in UI.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	//Stores the material used to render the pixels of this.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture* Texture;

	//Stores the default part shape
	UPROPERTY(BlueprintReadOnly)
	TSet<FIntPoint> Shape;

	//The percent of the default shape required to be intact for this part to be activated.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float FunctionalityPercent{ 0.5f };

	//Stores the ways that this part can be activated.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<UPartModule*> Modules;

	//Stores the cost to place the part in Pixels.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Cost;

	//Stores whether or not this part is placable by the player.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPlaceable{ true };

	//Stores whether or not this part can be removed by the player.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bRemovable{ true };

	//Stores whether this part can be ovrriden when editing the mold.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bOverridable{ true };

	//Stores whether or not this part can be rotated.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bRotatable{ true };
};

/**
 * A part representing the lack of a part.
 */
UCLASS()
class VOIDSINGER_API UNullPart : public UPartData
{
	GENERATED_BODY()
};