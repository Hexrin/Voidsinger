// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVoidsong.h"

UBaseVoidsong::UBaseVoidsong()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resource system init"));
}

UWorld* UBaseVoidsong::GetWorld() const
{
	if (GetOuter() == nullptr)
	{
		return nullptr;
	}

	// Special case for behavior tree nodes in the editor
	if (Cast<UPackage>(GetOuter()) != nullptr)
	{
		// GetOuter should return a UPackage and its Outer is a UWorld
		return Cast<UWorld>(GetOuter()->GetOuter());
	}

	// In all other cases...
	return GetOuter()->GetWorld();
}
void UBaseVoidsong::Tick(float DeltaTime)
{
	if (!bIsBeingDestroyed)
	{
		//Call Blueprint Implementable Event
		EventTick(DeltaTime);
	}
}
bool UBaseVoidsong::IsTickable() const
{
	return (!IsTemplate(RF_ClassDefaultObject));
}
TStatId UBaseVoidsong::GetStatId() const
{
	return TStatId();
}
void UBaseVoidsong::Activate()
{
	if (CanActivateAgain)
	{
		//Call the effect of the voidsong.
		Effect();

		//Make sure the voidsong can't be activated again
		CanActivateAgain = false;

		//Set a timer for the duration. When it's up the voidsong will deactivate.
		FTimerHandle DurationTimer;
		GetWorld()->GetTimerManager().SetTimer(DurationTimer, this, &UBaseVoidsong::DurationDelay, Duration);
	}
}

void UBaseVoidsong::DurationDelay()
{
	//Undo the effect of the voidsong.
	Deactivate();

	//Set a timer for the cooldown. When it's up the voidsong will be able to be activated again.
	FTimerHandle CooldownTimer;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &UBaseVoidsong::CooldownDelay, Cooldown);
}

void UBaseVoidsong::CooldownDelay()
{
	CanActivateAgain = true;
}
