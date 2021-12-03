// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWhoVoidsong.generated.h"

/**
 * 
 */

 //Should be renamed to UBaseFactionVoidsong for consitancy - Liam Suggestion
UCLASS()
class VOIDSINGER_API UBaseFactionVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()
	
public:

	//The faction that this Voidword will activate nouns on
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EFactions> Faction;

};
