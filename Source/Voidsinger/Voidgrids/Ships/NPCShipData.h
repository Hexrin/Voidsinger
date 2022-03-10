// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Engine/DataAsset.h"
#include "NPCShipData.generated.h"

/**
 * Stores information relating to an AI Ship
 */
UCLASS()
class VOIDSINGER_API UNPCShipData : public UDataAsset
{
	GENERATED_BODY()

public:
	//Stores the name of this ship as it appears in UI.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	//Stores the mold of this ship.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FMinimalPartInstanceData> Mold;

	//The faction of this ship.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EFaction Faction;

	//Gets the primary asset Id for the asset manger and build UI.
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};