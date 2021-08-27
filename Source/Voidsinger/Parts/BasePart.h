// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pixel.h"
#include "CoreMinimal.h"

/**
 * 
 */
class VOIDSINGER_API UBasePart
{
public:
	UBasePart();
	~UBasePart();

private:
	TArray<UPixel> Pixels;
	
};
