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

// \/ Activate \/ /

void UActivatablePartModule::Activate(const FPartActivationData Data)
{
	OnActivate(Data);
}

/**
 * Calls the "ActivateWithEffectiveness" function with an Effectiveness of 1 so the part module's functionality is executed.
 */

//void UActivatablePartModule::Activate(bool bApplyEffect)
//{
//	if (bApplyEffect)
//	{
//		OnActivate(FPartActivationData());
//	}
//}
//
///**
// * Calls the "OnActivate" function so the part module's functionality is executed
// *
// * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
// */
//void UActivatablePartModule::ActivateWithEffectiveness(float Effectiveness)
//{
//	OnActivate(FPartActivationData(Effectiveness));
//}
//
///**
// * Calls the "OnActivate" function with the Verbs played in a Voidsong so the part module's functionality of executed
// *
// * @param Verbs - The Verbs played in the Voidsong
// * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
// */
//void UActivatablePartModule::ActivateFromVoidsong(const TArray<UBaseVerbMotif*> Verbs, float Effectiveness)
//{
//	OnActivate(FPartActivationData(Verbs, Effectiveness));
//}
//
///**
// * Calls the "OnActivate" function so the part module's functionality is executed
// *
// * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
// * @param Vector - The direction to activate this part in.
// * @param Rotation - The rotation of the effect of this voidsong.
// */
//void UActivatablePartModule::ActivateWithEffectivenessVectorAndRotation(float Effectiveness, FVector2D Vector, float Rotation)
//{
//	OnActivate(FPartActivationData(Effectiveness, Vector, Rotation));
//}

// /\ Activate /\ /

/* /\ Activation /\ *\
\* ---------------- */

/* ---------------------- *\
\* \/ Delegate Binding \/ */

void UActivatablePartModule::BindToDelegates()
{
	//Bind "BindToVoidsong" to Voidsong played
	//Cast<AStarSystemGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->VoidsongManager->OnVoidsongPlayed.AddDynamic(this, &UActivatablePartModule::BindToVoidsong);

	// \/ Bind Activate to the Activation Cues selected \/ /

	//if ((bool)(ActivationCues & EActivationCue::OnDamaged))
	//{
	//	Part->OnDamaged.AddDynamic(this, &UActivatablePartModule::Activate);
	//}
	//if ((bool)(ActivationCues & EActivationCue::OnRepaired))
	//{
	//	Part->OnRepaired.AddDynamic(this, &UActivatablePartModule::Activate);
	//}
	//if ((bool)(ActivationCues & EActivationCue::OnFunctionalityLost))
	//{
	//	Part->OnFunctionalityLost.AddDynamic(this, &UActivatablePartModule::Activate);
	//}
	//if ((bool)(ActivationCues & EActivationCue::OnFunctionalityRestored))
	//{
	//	Part->OnFunctionalityRestored.AddDynamic(this, &UActivatablePartModule::Activate);
	//}
	//if ((bool)(ActivationCues & EActivationCue::OnDestroyed))
	//{
	//	Part->OnDestroyed.AddDynamic(this, &UActivatablePartModule::Activate);
	//}
	//if ((bool)(ActivationCues & EActivationCue::OnFullyRepaired))
	//{
	//	Part->OnFullyRepaired.AddDynamic(this, &UActivatablePartModule::Activate);
	//}
	//if ((bool)(ActivationCues & EActivationCue::OnTick))
	//{
	//	//Bind to on tick here (Delegate is not available yet)
	//}

	// /\ Bind Activate to the Activation Cues selected /\ /

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