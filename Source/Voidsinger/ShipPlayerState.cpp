//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "ShipPlayerState.h"
//
///**
//* Gets the number of pixels the player has.
//*
//* @return The number of pixels the player has.
//*/
//int32 AShipPlayerState::GetPixels()
//{
//    return Pixels;
//}
//
///**
// * Adds the given number of pixels to the player's bank.
// *
// * @param Amount - The number of pixels to add.
// */
//void AShipPlayerState::DepositPixels(int32 Amount)
//{
//    Pixels += FMath::Max(Amount, 0);
//}
//
///**
// * Withdraw the given number of pixels from the player's bank.
// *
// * @param Amount - The number of pixels to withdraw.
// * @return Wether or not the player has enough pixels.
// */
//bool AShipPlayerState::WithdrawPixels(int32 Amount)
//{
//    if (Pixels >= Amount)
//    {
//        Pixels -= Amount;
//        return true;
//    }
//
//    return false;
//}
