// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/PartActivationData.h"
#include "BaseActivationCue.generated.h"

// \/ Forward declarations \/ /

class UActivatablePartModule;

// /\ Forward declarations /\ /

//Broadcasts the activation data when this activation cue is called
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActivationCue, const FPartActivationData, ActivationData);

/* \/ ================== \/ *\
|  \/ UBaseActivationCue \/  |
\* \/ ================== \/ */

/**
 * Base class for activation cues. Activation cues contain delegates that part modules can bind to.
 */
UCLASS(EditInlineNew, Abstract)
class VOIDSINGER_API UBaseActivationCue : public UObject
{
	GENERATED_BODY()
	
	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/*
	 * Initializes this activation cue's variables and bindings
	 * 
	 * @param OwningModule - The module that owns this activation cue
	 */
	UFUNCTION(Category = "Initialization")
	virtual void Initialize(UActivatablePartModule* OwningModule);

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------------- *\
	\* \/ Delegation \/ */

	//Delegate for broadcasting the activation data when this activation cue is called
	UPROPERTY(BlueprintAssignable, Category = "Delegation")
	FActivationCue OnActivate;

	/* /\ Delegation /\ *\
	\* ---------------- */
};

/* /\ ================== /\ *\
|  /\ UBaseActivationCue /\  |
\* /\ ================== /\ */
