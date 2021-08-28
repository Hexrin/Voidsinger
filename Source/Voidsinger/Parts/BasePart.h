// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePart.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBasePart : public UObject
{

	GENERATED_BODY()

public:
	UBasePart();

protected:
	UPROPERTY(EditAnywhere)
	TArray<FIntPoint> DesiredShape;
	
};
