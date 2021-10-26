// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Voidsongs/BaseVerbVoidsong.h"
#include "VoidGameMode.generated.h"

/**
 * 
 */

UINTERFACE(Blueprintable)
class UActivateInterface : public UInterface
{
	GENERATED_BODY()
};

class IActivateInterface
{
	GENERATED_BODY()

public:

	virtual void Activate();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVoidsongDelegate, const TArray<TEnumAsByte<EFactions>>& , Factions, const TArray<TSubclassOf<UObject>>&, NounClasses);

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

	UFUNCTION(BlueprintCallable)
	void ActivateWithEffects(UObject* ObjectHit);

	UFUNCTION(BlueprintCallable)
	void Broadcast(TArray<TEnumAsByte<EFactions>> Factions, TArray<TSubclassOf<UObject>> NounClasses, TArray<TSubclassOf<UBaseVerbVoidsong>> Verbs);

	UFUNCTION()
	FVoidsongDelegate GetVoidsongDelegate();

	UPROPERTY(BlueprintAssignable)
	FVoidsongDelegate OnVoidsongDelegate;

	UPROPERTY()
	TArray<TSubclassOf<UBaseVerbVoidsong>> VerbsActive;

};
