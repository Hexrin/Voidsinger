// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VoidGameMode.generated.h"

/**
 * 
 */
UCLASS(Config=VoidSettings)
class VOIDSINGER_API AVoidGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AVoidGameMode();

	UPROPERTY(Config)
	int Testaroo;

	UFUNCTION(BlueprintCallable)
	int PrintTestaroo();

};
