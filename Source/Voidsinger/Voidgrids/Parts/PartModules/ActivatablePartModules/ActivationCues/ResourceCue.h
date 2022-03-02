// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivationCues/BaseActivationCue.h"
#include "Voidsinger/Voidgrids/ResourceType.h"
#include "ResourceCue.generated.h"

// \/ Forward declarations \/ /

class AVoidgrid;

// /\ Forward declarations /\ /

/* \/ ============ \/ *\
|  \/ UResourceCue \/  |
\* \/ ============ \/ */

/**
 * Subclass of the base activation cue that is able to create and use resources
 */
UCLASS()
class VOIDSINGER_API UResourceCue : public UBaseActivationCue
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
	virtual void Initialize(UActivatablePartModule* OwningModule) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------------- *\
	\* \/ Delegation \/ */

public:

	//Stores when this activation cue will be activated
	UPROPERTY(Instanced, EditAnywhere)
	TArray<UBaseActivationCue*> ActivationCues;

	//Stores the resources and priority of this cue
	UPROPERTY(EditAnywhere)
	FResourceCall ResourceCall;

private:

	/*
	 * Creates a resource call
	 *
	 * @param Data - The activation data containing all relavent information, including the effectiveness
	 */
	UFUNCTION()
	void CreateResourceCall(FPartActivationData Data);

	/*
	 * Finishes the resource call by calling "OnActivate"
	 */
	UFUNCTION()
	void OnResourceCallCompleted();

	//Stores the Voidgrid that owns this activation cue
	UPROPERTY()
	AVoidgrid* Voidgrid;


	//Stores the activation data of the last activation
	UPROPERTY()
	FPartActivationData PartActivationData;

	/* /\ Delegation /\ *\
	\* ---------------- */

};

/* /\ ============ /\ *\
|  /\ UResourceCue /\  |
\* /\ ============ /\ */