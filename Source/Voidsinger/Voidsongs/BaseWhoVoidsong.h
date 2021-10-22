// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWhoVoidsong.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseWhoVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetWhos();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> Who;

};
