// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TutorialEventHandler.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VOIDSINGER_API UTutorialEventHandler : public UObject
{
	GENERATED_BODY()

	/* ------------- *\
	\* \/ Getters \/ */

private:

	/**
	 * Overriden so this object can have access to the world
	 * 
	 * @return - The world
	 */
	virtual	class UWorld* GetWorld() const override;

	/* /\ Getters /\ *\
	\* ------------- */

};
