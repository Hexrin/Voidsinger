// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShip.h"
#include "PlayerShip.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API APlayerShip : public ABaseShip
{
	GENERATED_BODY()
	

	protected:
	int Pixels;

	public:
	APlayerShip();
	
};
