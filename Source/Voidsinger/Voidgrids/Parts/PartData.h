// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartModules/PartModule.h"
#include "Engine/DataAsset.h"
#include "PartData.generated.h"

/**
 * Stores information realating to a part type
 */
UCLASS()
class VOIDSINGER_API UPartData : public UDataAsset
{
	GENERATED_BODY()

public:
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

	//Stores the total mass of this part.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Mass{ 1 };

	//Stores the cost to place the part in Pixels.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Cost;

	//Heat Resistance is this parts ability to resist melting or frezing. If a part reaches a tempature whoes absolute value is above its heat resistance then it will either melt or freeze.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "10"))
	float HeatResistance{ 1 };

	//Strength is this parts ability to resist explosions and impacts. A the explosion radius of any explosions effecting this part will be divided by the strenth.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="1", ClampMax="10"))
	float Strength{ 1 };

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

	//Gets the primar asset Id for the asset mager and build UI.
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};