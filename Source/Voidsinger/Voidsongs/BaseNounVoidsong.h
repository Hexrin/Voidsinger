// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "BaseNounVoidsong.generated.h"

/**
 * 
 */

class UBasePart;

UCLASS()
class VOIDSINGER_API UBaseNounVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBasePart> Noun;

};
