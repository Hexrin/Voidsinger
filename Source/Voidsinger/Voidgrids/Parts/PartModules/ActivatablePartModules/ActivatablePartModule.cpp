// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivatablePartModule.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Kismet/GameplayStatics.h"

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Initializes the part module's protected and private variables. Overriden from PartModule to call BindToDelegates().
 *
 * @param OwningPart - The part that this module is being initialized from
 */
void UActivatablePartModule::InitializeVariables(UPart* OwningPart)
{
	Super::InitializeVariables(OwningPart);

	BindToDelegates();
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ---------------- *\
\* \/ Activation \/ */

/**
 * Called when the owning part is activated and calls the functionality of this part.
 */
void UActivatablePartModule::OnActivate_Implementation(const FPartActivationData Data)
{
}

/**
 * Calls the "OnActivate" function with the data given
 *
 * @param Data - The data of this activation
 */
void UActivatablePartModule::Activate(const FPartActivationData Data)
{
	UE_LOG(LogTemp, Warning, TEXT("part module activate data verbs num %i"), Data.Verbs.Num());
	OnActivate(Data);
}

/* /\ Activation /\ *\
\* ---------------- */

/* ---------------------- *\
\* \/ Delegate Binding \/ */

void UActivatablePartModule::BindToDelegates()
{
	for (UBaseActivationCue* EachActivationCue : ActivationCues)
	{
		EachActivationCue->Initialize(this);
		EachActivationCue->OnActivate.AddUniqueDynamic(this, &UActivatablePartModule::Activate);
	}

}

/* /\ Delegate Binding /\ *\
\* ---------------------- */

/* ----------- *\
\* \/ World \/ */

/*
 * Gives the activatable part module access to the world
 * 
 * @return - A reference to the world
 */
UWorld* UActivatablePartModule::GetWorld() const
{
	if (Part->IsValidLowLevel())
	{
		return Part->GetWorld();
	}

	return nullptr;
}

/* /\ World /\ *\
\* ----------- */