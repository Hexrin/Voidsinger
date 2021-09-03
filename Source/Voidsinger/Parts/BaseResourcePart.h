// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Parts/BasePart.h"
#include "BaseResourcePart.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseResourcePart : public UBasePart
{
	GENERATED_BODY()
	
public:

	UBaseResourcePart();

	TEnumAsByte<EResourceType> ResourceType;

	UFUNCTION(BlueprintCallable)
	void Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot);

};
