// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class VOIDSINGER_API UBasePart
{
public:
	UBasePart();
	~UBasePart();

protected:
	UPROPERTY(EditAnywhere)
	TArray<FIntPoint> DesiredShape;
	
};
