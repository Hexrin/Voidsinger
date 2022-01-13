// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVoidsong.h"

FPrimaryAssetId UBaseVoidsong::GetPrimaryAssetId() const
{
	FString Thing = GetClass()->GetName();
	Thing.RemoveFromEnd("_C");

	return FPrimaryAssetId(FName("Voidsong"), FName(Thing));
}

/*
* Function comments from the .h should be copied to the .cpp 
* 
* This should be deleted because
* If a constuctor is empty it doesn't need to be declared or defined, you can just used the constuctor from its parent.
* - Liam Suggestion
*/
UBaseVoidsong::UBaseVoidsong()
{
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
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

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void UBaseVoidsong::Tick(float DeltaTime)
{
	if (!bIsBeingDestroyed)
	{
		//Call Blueprint Implementable Event
		EventTick(DeltaTime);
	}
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
bool UBaseVoidsong::IsTickable() const
{
	return (!IsTemplate(RF_ClassDefaultObject));
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
TStatId UBaseVoidsong::GetStatId() const
{
	return TStatId();
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
TArray<int> UBaseVoidsong::GetActivationCombo()
{
	return ActivationCombo;
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
FText UBaseVoidsong::GetVoidsongDisplayText()
{
	return VoidsongDisplayText;
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
float UBaseVoidsong::GetDuration()
{
	return Duration;
}