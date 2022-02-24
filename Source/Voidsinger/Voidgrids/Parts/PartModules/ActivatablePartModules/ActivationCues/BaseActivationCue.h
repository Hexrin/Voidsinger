// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/PartActivationData.h"
#include "BaseActivationCue.generated.h"

// \/ Forward declarations \/ /

class UPart;

// /\ Forward declarations /\ /

//Broadcasts the activation data when this activation cue is called
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActivationCue, const FPartActivationData, ActivationData);

/* \/ ================== \/ *\
|  \/ UBaseActivationCue \/  |
\* \/ ================== \/ */

/**
 * Base class for activation cues. Activation cues contain delegates that part modules can bind to.
 */
UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class VOIDSINGER_API UBaseActivationCue : public UObject
{
	GENERATED_BODY()
	
public:

	/*
	 * Initializes this activation cue's variables and bindings
	 */
	virtual void Initialize(UPart* OwningPart);

	//Delegate for broadcasting the activation data when this activation cue is called
	UPROPERTY(BlueprintAssignable)
	FActivationCue OnActivate;

};

/* /\ ================== /\ *\
|  /\ UBaseActivationCue /\  |
\* /\ ================== /\ */
