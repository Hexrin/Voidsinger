// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidsingerGameInstance.h"

/**
* Gets the number of pixels the player has.
*
* @return The number of pixels the player has.
*/
int32 UVoidsingerGameInstance::GetPixels()
{
    return Pixels;
}

/**
 * Sets the number of pixels the player has.
 *
 * @param NewPixels - The number of pixels the player should have.
 */
UFUNCTION(BlueprintCallable)
void UVoidsingerGameInstance::SetPixels(int32 NewPixels)
{
    Pixels = NewPixels;
}

/**
 * Adds the given number of pixels to the player's bank.
 *
 * @param Amount - The number of pixels to add.
 */
void UVoidsingerGameInstance::DepositPixels(int32 Amount)
{
    if (Amount > 0)
    {
        Pixels += Amount;
        OnPixelBalenceChanged.Broadcast(Amount);
    }
}

/**
 * Withdraw the given number of pixels from the player's bank.
 *
 * @param Amount - The number of pixels to withdraw.
 * @return Whether or not the player has enough pixels.
 */
bool UVoidsingerGameInstance::WithdrawPixels(int32 Amount)
{
    if (Pixels >= Amount)
    {
        if (Amount > 0)
        {
            Pixels -= Amount;
            OnPixelBalenceChanged.Broadcast(-1 * Amount);
        }
        return true;
    }

    return false;
}
