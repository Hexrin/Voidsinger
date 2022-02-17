// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VoidsingerGameInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FExplodeDelegate, FVector, ExplosionLocation, float, ExplosionRadius);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPixelBalanceChangedDelegate, int32, ChangeAmount);

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

};
