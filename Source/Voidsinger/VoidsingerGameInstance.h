// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ChildClassesFinder.h"
#include "LevelManager.h"
#include "VoidsingerGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExplodeDelegate, FVector, ExplosionLocation, float, ExplosionRadius);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPixelBalanceChangedDelegate, int32, ChangeAmount);
//Broadcasts an event relating to loading of an asset.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadingDelegate);
UCLASS()
class VOIDSINGER_API UVoidsingerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	UChildClassesFinder* ChildClassFinder;

	/* ------------ *\
	\* \/ Pixels \/ */

	/**
	 * Gets the number of pixels the player has.
	 *
	 * @return The number of pixels the player has.
	 */
	UFUNCTION(BlueprintPure)
	int32 GetPixels();

	/**
	 * Sets the number of pixels the player has.
	 *
	 * @param NewPixels - The number of pixels the player should have.
	 */
	UFUNCTION(BlueprintCallable)
	void SetPixels(int32 NewPixels);

	/**
	 * Adds the given number of pixels to the player's bank.
	 *
	 * @param Amount - The number of pixels to add.
	 */
	UFUNCTION(BlueprintCallable)
	void DepositPixels(int32 Amount);

	/**
	 * Withdraw the given number of pixels from the player's bank.
	 *
	 * @param Amount - The number of pixels to withdraw.
	 * @return Wether or not the player has enough pixels.
	 */
	UFUNCTION(BlueprintCallable)
	bool WithdrawPixels(int32 Amount);

	/**
	 * Called When the pixel balence of the player changes.
	 */
	UPROPERTY(BlueprintAssignable)
	FPixelBalanceChangedDelegate OnPixelBalenceChanged;

	UPROPERTY(BlueprintAssignable)
	FExplodeDelegate OnExplodeDelegate;

protected:
	//Comment -Mabel Suggestion
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 Pixels{ 100 };

	//Stores the percent of the pixel value of enemy ships gained when an enemy is defeated.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float SalvagingEfficiency{ 0.1 };

	/* /\ Pixels /\ *\
	\* ------------ */



	/* ------------------- *\
	\* \/ Level Manager \/ */

public:
	/**
	 * Loads a level, initilizes a level manager and calls loading delegates.
	 * 
	 * @param Level - The level to load.
	 * @param LevelManagerClass - The class level manager used in this level.
	 */
	UFUNCTION(BlueprintCallable, Meta = (WorldContext = "WorldContextObject"))
	void LoadLevelWithManager(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level, const TSubclassOf<ALevelManager> LevelManagerClass);

	/**
	 * Gets a refernce to the current level manager.
	 *
	 * @return a pointer to the current level manager.
	 */
	UFUNCTION(BlueprintPure)
	FORCEINLINE ALevelManager* GetLevelManager() const { return LevelManager; };

private:
	//Stores a referce to the level manger.
	UPROPERTY()
	ALevelManager* LevelManager;

	//Stores the current streaming level.
	TSoftObjectPtr<UWorld> CurrentLevel;

	/* /\ Level Manager /\ *\
	\* ------------------- */

	/* ------------- *\
	\* \/ Loading \/ */

public:
	//Called when loading begins.
	UPROPERTY(BlueprintAssignable)
	FLoadingDelegate OnBeginLoading;

	//Called when a loading time is compleate.
	UPROPERTY(BlueprintAssignable)
	FLoadingDelegate OnEndLoading;

	//Sets the ULevel of the level manger.
	UFUNCTION()
	void SetLevelManagerLevel();
private:

	//Calls OnBeginLoading
	FORCEINLINE void BeginLoading() const { OnBeginLoading.Broadcast();  };

	//Calls OnEndLoading
	FORCEINLINE void EndLoading() const { OnEndLoading.Broadcast(); };

	/* /\ Loading /\ *\
	\* ------------- */
};
