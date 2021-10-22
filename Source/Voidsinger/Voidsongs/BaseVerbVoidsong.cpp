// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidsongs/BaseVerbVoidsong.h"
#include "Kismet/GameplayStatics.h"

void UBaseVerbVoidsong::Activate(TArray<UBaseWhoVoidsong*> Whos, TArray<UBaseNounVoidsong*> Nouns)
{
	if (CanActivateAgain)
	{

		if (Whos.IsEmpty())
		{
			
		}
		else
		{
			WhosAffected = Whos;
		}
		NounsAffected = Nouns;
		//Call the effect of the voidsong.

		Effect(Whos, Nouns);

		//Make sure the voidsong can't be activated again
		CanActivateAgain = false;

		//Set a timer for the duration. When it's up the voidsong will deactivate. If Duration is 0 then just deactivate immediately.
		FTimerHandle DurationTimer;
		if (Duration != 0)
		{
			GetWorld()->GetTimerManager().SetTimer(DurationTimer, this, &UBaseVerbVoidsong::DurationDelay, Duration);
		}
		else
		{
			DurationDelay();
		}
	}
}

void UBaseVerbVoidsong::DurationDelay()
{
	//Undo the effect of the voidsong.
	Deactivate();

	//unset whos and nouns
	WhosAffected.Empty();
	NounsAffected.Empty();

	CanActivateAgain = true;

}

