// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/PartModule.h"
#include "Voidsinger/Voidsongs/Voidsong.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/PartActivationData.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivationCues/BaseActivationCue.h"
#include "ActivatablePartModule.generated.h"


/* -------------------------- *\
\* \/ Forward Declarations \/ */

class UPart;

/* /\ Forward Declarations /\ *\
\* ------------------------- */

/* \/ ====================== \/ *\
|  \/ UActivatablePartModule \/  |
\* \/ ====================== \/ */

/**
 * This subclass of UPartModule is activatable. Functionality is defined in the "OnActivate" BlueprintImplementable function. 
 * The "Activate" function is bound to whatever delegates are selected in the EActivationCue and EVoidsongCue enums.
 */
UCLASS()
class VOIDSINGER_API UActivatablePartModule : public UPartModule
{
	GENERATED_BODY()
	

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
	 * Initializes the part module's protected and private variables. Overriden from PartModule to call BindToDelegates().
	 *
	 * @param OwningPart - The part that this module is being initialized from
	 */
	virtual void InitializeVariables(UPart* OwningPart) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------------- *\
	\* \/ Activation \/ */

protected:

	/**
	 * Called when the owning part is activated and calls the functionality of this part.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, DisplayName = "Activate", Category = "Activation")
	void OnActivate(const FPartActivationData Data);
	virtual void OnActivate_Implementation(const FPartActivationData Data);

public:

	/**
	 * Calls the "OnActivate" function with the data given
	 * 
	 * @param Data - The data of this activation
	 */
	UFUNCTION()
	void Activate(const FPartActivationData Data);

	//Stores what Noun Motif will activate this part module. If unbound, this module will not be activatable by a Voidsong.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation")
	ENoun Noun;

	/* /\ Activation /\ *\
	\* ---------------- */

	/* ---------------------- *\
	\* \/ Delegate Binding \/ */

public:
	/**
	 * Binds the activate function to all selected delegates
	 */
	void BindToDelegates();

	//Stores what activation cues to bind to
	UPROPERTY(EditAnywhere, Category = "Delegate Binding", Instanced) 
	TArray<UBaseActivationCue*> ActivationCues;

	/* /\ Delegate Binding /\ *\
	\* ---------------------- */

	/* ----------- *\
	\* \/ World \/ */

public:

	/*
	 * Gives the activatable part module access to the world
	 *
	 * @return - A reference to the world
	 */
	virtual UWorld* GetWorld() const override;

	/* /\ World /\ *\
	\* ----------- */

};

/* /\ =========== /\ *\
|  /\ UPartModule /\  |
\* /\ =========== /\ */