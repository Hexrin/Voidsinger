//// Fill out your copyright notice in the Description page of Project Settings.

#include "VoidsingerGameMode.h"

/**
 * Intitializes default properties
 */
AVoidsingerGameMode::AVoidsingerGameMode()
{
	//Initialize the Voidsong Manager
	VoidsongManager = NewObject<UVoidsongManager>(UVoidsongManager::StaticClass());
}
