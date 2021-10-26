// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVoidsong.h"

UBaseVoidsong::UBaseVoidsong()
{
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

TArray<int> UBaseVoidsong::GetActivationCombo()
{
	return ActivationCombo;
}

FText UBaseVoidsong::GetVoidsongDisplayText()
{
	return VoidsongDisplayText;
}

