// Fill out your copyright notice in the Description page of Project Settings.

#include "VoidsingerGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelScriptActor.h"
#include "LevelManager.h"

/* ------------ *\
\* \/ Pixels \/ */

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
 * @return Wether or not the player has enough pixels.
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

/* /\ Pixels /\ *\
\* ------------ */



/* ------------------- *\
\* \/ Level Manager \/ */

/**
 * Loads a level, initilizes a level manager and calls loading delegates.
 *
 * @param Level - The level to load.
 * @param LevelManagerClass - The class level manager used in this level.
 */
void UVoidsingerGameInstance::LoadLevelWithManager(const TSoftObjectPtr<UWorld> Level, const TSubclassOf<ALevelManager> LevelManagerClass)
{
    if (IsValid(GetWorld()) && !Level.IsNull())
    {
        BeginLoading();
        //Unload Old Level
        FLatentActionInfo LatentUnloadInfo;
        UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, CurrentLevel, LatentUnloadInfo, true);

        //Load New Level
        FLatentActionInfo LatentLoadInfo;
        LatentLoadInfo.CallbackTarget = this;
        LatentLoadInfo.ExecutionFunction = FName("ResetLevelManager");
        LatentLoadInfo.Linkage = 1;
        LatentLoadInfo.UUID = 1;

        UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, Level, true, true, LatentLoadInfo);

        //Set Level Manager
        if (IsValid(LevelManager))
        {
            LevelManager->Destroy();
        }
        CurrentLevelManagerClass = LevelManagerClass;
    }
}


void UVoidsingerGameInstance::ResetLevelManager()
{
    CurrentLevel = GetWorld()->GetLevel(1);
    if (IsValid(LevelManager))
    {
        LevelManager->Destroy();
    }

    if (IsValid(CurrentLevelManagerClass.Get()))
    {
        FActorSpawnParameters SpawnParams = FActorSpawnParameters();
        SpawnParams.OverrideLevel = GetWorld()->GetLevel(1);
        GetWorld()->SpawnActor<ALevelManager>(CurrentLevelManagerClass.Get(), SpawnParams);
    }
}
/* /\ Level Manager /\ *\
\* ------------------- */