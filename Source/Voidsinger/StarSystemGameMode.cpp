//// Fill out your copyright notice in the Description page of Project Settings.

#include "StarSystemGameMode.h"

//old includes
//#include "ShipPlayerState.h"
//#include "Ships/PlayerShip.h"

/**
 * Intitializes default properties
 */
AStarSystemGameMode::AStarSystemGameMode()
{
	// \/ old stuff \/
	
	//LoadConfig();
	//PlayerStateClass = AShipPlayerState::StaticClass();
	//DefaultPawnClass = APlayerShip::StaticClass();

	// /\ old stuff /\

	//Initialize the Voidsong Manager
	VoidsongManager = NewObject<UVoidsongManager>(UVoidsongManager::StaticClass());
}
