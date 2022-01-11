// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsongs/VoidsongComponent.h"

// Sets default values for this component's properties
UVoidsongComponent::UVoidsongComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVoidsongComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVoidsongComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


/* ------------------------------------------------------------------------------------- *\
\* \/ Old Voidsong functions from ABaseShip (put here for reference, will be deleted) \/ */

//Function comments from the .h should be identical to the one in the .cpp - Liam Suggestion
//Adds a new voidsong
void UVoidsongComponent::AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong)
{
	//Creates the voidsong object from the given class and adds it to available voidsongs
	/*UBaseVoidsong* NewVoidsong = NewObject<UBaseVoidsong>(this, Voidsong);
	AvailableVoidsongs.Emplace(NewVoidsong);
	OnAddVoidsongDelegate.Broadcast(NewVoidsong);*/
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
//Plays the voidsong sequence
void UVoidsongComponent::PlaySequence(TArray<int> Sequence)
{
	if (!Sequence.IsEmpty() && CanActivateVoidsong)
	{
		TArray<EFactions> Factions;
		TArray<TSubclassOf<UObject>> Nouns;
		TArray<UBaseVerbVoidsong*> Verbs;

		float Duration = DecideVoidsongsPlayed(Sequence, Factions, Nouns, Verbs);

		if (!Factions.IsEmpty() || !Nouns.IsEmpty() || !Verbs.IsEmpty())
		{
			//Cast<AVoidGameMode>(GetWorld()->GetAuthGameMode())->Broadcast(Factions, Nouns, Verbs, AvailableVoidsongs);
		}

		//Delete print string or come up with a good way for c++ debug modes - Liam Suggestion
		UE_LOG(LogTemp, Warning, TEXT("duration %f"), Duration);
		if (Duration != 0)
		{
			//Does this work with multiple simultaneous Voidsongs? I think it does but be sure to test it. - Liam Suggestion
			FTimerHandle DurationTimer;
			GetWorld()->GetTimerManager().SetTimer(DurationTimer, this, &ABaseShip::DurationDelay, Duration);
			CanActivateVoidsong = false;
		}

	}
}

/*
* Function comments from the .h should be copied to the .cpp
*
* Function should be void and have a pass by refernce parameter for returning duration
* because it would make the logic for returning duration much easier.
* - Liam Suggestion
*/
float UVoidsongComponent::DecideVoidsongsPlayed(TArray<int> Sequence, TArray<EFactions>& Factions, TArray<TSubclassOf<UObject>>& Nouns, TArray<UBaseVerbVoidsong*>& Verbs)
{
	/*float Duration = 0;

	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
	for (UBaseVoidsong* i : AvailableVoidsongs)
	{
		//UE doesnt like int, use int32 or int64 instead. - Liam Suggestion
		TArray<int> TrimmedSequence = Sequence;

		//UBaseVoidsong::ActivationCombo
		TrimmedSequence.SetNum(i->ActivationCombo.Num());
		if (i->ActivationCombo == TrimmedSequence)
		{
			if (IsValid(Cast<UBaseFactionVoidsong>(i)))
			{
				Factions.Emplace(Cast<UBaseFactionVoidsong>(i)->Faction);
			}
			else if (IsValid(Cast<UBaseNounVoidsong>(i)))
			{
				Nouns.Emplace(Cast<UBaseNounVoidsong>(i)->Noun);
			}
			else if (IsValid(Cast<UBaseVerbVoidsong>(i)))
			{
				Verbs.Emplace(Cast<UBaseVerbVoidsong>(i));
			}

			Duration += i->Duration;

			//Local variablee unnecessary replace with Sequence - Liam Suggestion
			TArray<int> RecursiveArray = Sequence;

			for (auto& j : i->ActivationCombo)
			{
				RecursiveArray.RemoveAt(0);
			}

			if (!RecursiveArray.IsEmpty())
			{
				DecideVoidsongsPlayed(RecursiveArray, Factions, Nouns, Verbs);
			}
			break;
		}

	}*/

	/*
	* This is only returns the duration of the first Voidsong in a sentance
	* To correctly return a value form a recursive function typicaly you need to return itself
	* eg: return DecideVoidsongsPlayed(RecursiveArray, Factions, Nouns, Verbs);
	* - Liam Suggestion
	*/
	//return Duration;

	return 0.0;
}

/*
* Function comments from the .h should be copied to the .cpp
*
* Confusing Fucntion Name.
* What is a duration delay?
* Judgeing by the name I'd think this function would return the duration of a delay for a Voidsong.
* - Liam Suggestion
*/
void UVoidsongComponent::DurationDelay()
{
	//Delete print string or come up with a good way for c++ debug modes - Liam Suggestion
	//UE_LOG(LogTemp, Warning, TEXT("Can Activate again"))
	CanActivateVoidsong = true;
	//This system definently does not work with multiple simultanous voidsongs - Liam Suggestion
	//Cast<AVoidGameMode>(GetWorld()->GetAuthGameMode())->UnsetVerbs();
	OnUnsetVerbsDelegate.Broadcast();
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UVoidsongComponent::LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs)
{
	/*
	* Iterator should have a name that tells what it actualy is and what its iterating through
	*
	* If suggestion on lines 11-18 of BaseVoidsong.h are followed then unessaray for loop can be replaced with:
	* AvailableVoidsongs += Voidsongs;
	* - Liam Suggestion
	*/
	//Creates voidsong objects with the list of voidsongs given. Adds them to available voidsongs
	for (auto& i : Voidsongs)
	{
		AvailableVoidsongs.Emplace(NewObject<UBaseVoidsong>(this, i));
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UVoidsongComponent::BroadcastActivateParts(const TArray<TSubclassOf<UObject>>& NounClasses)
{
	//Varialbe name should be meaningful - Liam Suggestion
	TArray<EFactions> Temp;
	Temp.Emplace(GetFaction());
	OnActivatePartsDelegate.Broadcast(Temp, NounClasses, AvailableVoidsongs);
}

/* /\ Old Voidsong functions from ABaseShip (put here for reference, will be deleted) /\ *\
\* ------------------------------------------------------------------------------------- */