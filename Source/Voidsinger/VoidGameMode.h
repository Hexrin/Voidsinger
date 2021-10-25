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
class UFireInterface : public UInterface
{
	GENERATED_BODY()
};

class IFireInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void Fire();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVoidsongDelegate, TEnumAsByte<EFactions>, Faction, TSubclassOf<UObject>, NounClass);

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
	void ActivateWithEffects(UObject* ThingActivated);

	UFUNCTION(BlueprintCallable)
	void Broadcast(TEnumAsByte<EFactions> Faction, TSubclassOf<UObject> NounClass, TSubclassOf<UBaseVerbVoidsong> Verb);

	UFUNCTION()
	FVoidsongDelegate GetVoidsongDelegate();

	UPROPERTY(BlueprintAssignable)
	FVoidsongDelegate OnVoidsongDelegate;

};
