// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "BaseWhoVoidsong.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseWhoVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	UClass* Who;

};
