// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ChildClassesFinder.h"
#include "VoidsingerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UVoidsingerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	UChildClassesFinder* ChildClassFinder;
};