//// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Voidsongs/VoidsongManager.h"
#include "StarSystemGameMode.generated.h"

/**
 * Game mode. Provides a way to access the Voidsong Manager
 */
UCLASS()
class VOIDSINGER_API AStarSystemGameMode : public AGameModeBase
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
	 * Intitializes default properties
	 */
	AStarSystemGameMode();

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ------------------------- *\
	\* \/ Voidsong Management \/ */

public:

	UPROPERTY(BlueprintReadOnly, Category = "VoidsongManagement")
	UVoidsongManager* VoidsongManager;

	/* /\ Voidsong Management /\ *\
	\* ------------------------- */

};
