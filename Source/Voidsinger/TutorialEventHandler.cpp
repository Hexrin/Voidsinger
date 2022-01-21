// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialEventHandler.h"

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Constructor used for calling "OnConstruct"
 */
UTutorialEventHandler::UTutorialEventHandler()
{
	OnConstruct();
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ------------- *\
\* \/ Getters \/ */

/**
 * Overriden so this object can have access to the world
 *
 * @return - The world
 */
UWorld* UTutorialEventHandler::GetWorld() const
{
	//Get the outer's world
	if (GetOuter() && !HasAnyFlags(RF_ClassDefaultObject))
	{
		return GetOuter()->GetWorld();
	}

	//If the outer isn't valid, return nullptr
	return nullptr;
}

/* /\ Getters /\ *\
\* ------------- */