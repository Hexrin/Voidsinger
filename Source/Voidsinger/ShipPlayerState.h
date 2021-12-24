//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/PlayerState.h"
//#include "VoidsingerTypes.h"
//#include "Parts/BasePart.h"
//#include "ShipPlayerState.generated.h"
//
///**
// * 
// */
//
//
////Comment the class -Mabel Suggestion
//UCLASS()
//class VOIDSINGER_API AShipPlayerState : public APlayerState
//{
//	GENERATED_BODY()
//	
//public:
//
//	//what is this for -Mabel Suggestion
//	TGridMap<TSubclassOf<UBasePart>> ShipBlueprint;
//
//public:
//	/**
//	 * Gets the number of pixels the player has.
//	 *
//	 * @return The number of pixels the player has.
//	 */
//	UFUNCTION(BlueprintPure)
//	int32 GetPixels();
//
//	/**
//	 * Adds the given number of pixels to the player's bank.
//	 *
//	 * @param Amount - The number of pixels to add.
//	 */
//	UFUNCTION(BlueprintCallable)
//	void DepositPixels(int32 Amount);
//
//	/**
//	 * Withdraw the given number of pixels from the player's bank.
//	 *
//	 * @param Amount - The number of pixels to withdraw.
//	 * @return Wether or not the player has enough pixels.
//	 */
//	UFUNCTION(BlueprintCallable)
//	bool WithdrawPixels(int32 Amount);
//
//protected:
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere)
//	int32 Pixels{ 100 };
//
//	//Stores the percent of the pixel value of enemy ships gained when an enemy is defeated.
//	UPROPERTY(BlueprintReadOnly, EditAnywhere)
//	float SalvagingEfficiency{ 0.1 };
//};
