// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "BaseNounVoidsong.generated.h"

/**
 * 
 */

class UBasePart;

UCLASS(Abstract, Blueprintable)
class VOIDSINGER_API UBaseNounVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()

public:

	//The class of the noun that this Voidword will activate
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBasePart> Noun;

};
