// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Parts/BasePart.h"
#include "CorePart.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UCorePart : public UBasePart
{
	GENERATED_BODY()

	//Constructor
	UCorePart();

	virtual void DestroyPart() override;

protected:

	//Death event for demo
	UFUNCTION(BlueprintImplementableEvent)
	void CoreDestroyed();

};
