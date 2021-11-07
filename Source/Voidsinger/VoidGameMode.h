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

//class UBasePart;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FVoidsongDelegate, const TArray<TEnumAsByte<EFactions>>& , Factions, const TArray<TSubclassOf<UObject>>&, NounClasses, const TArray<UBaseVoidsong*>&, AvailableVoidsongs);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeactivateVoidsongDelegate);

UCLASS()
class VOIDSINGER_API AVoidGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AVoidGameMode();

	UFUNCTION(BlueprintCallable)
	void ActivateWithEffects(AActor* ActorHit, AActor* ActorThatActivated, TArray<UBasePart*> PartsHit, FVector LocationCalledFrom, FVector WorldLocation, float Effectiveness);

	UFUNCTION(BlueprintCallable)
	void Broadcast(TArray<TEnumAsByte<EFactions>> Factions, TArray<TSubclassOf<UObject>> NounClasses, TArray<UBaseVerbVoidsong*> Verbs, TArray<UBaseVoidsong*> AvailableVoidsongs);

	UFUNCTION()
	void UnsetVerbs();

	UFUNCTION()
	FVoidsongDelegate GetVoidsongDelegate();

	UFUNCTION(BlueprintPure)
	TArray<UBaseVerbVoidsong*> GetVerbsActive();

	UPROPERTY(BlueprintAssignable)
	FVoidsongDelegate OnVoidsongDelegate;

	UPROPERTY(BlueprintAssignable)
	FDeactivateVoidsongDelegate OnDeactivateVoidsongDelegate;

	UPROPERTY()
	TArray<UBaseVerbVoidsong*> VerbsActive;

};
