// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "VoidsingerGameInstance.h"
#include "LevelManager.h"

/**
 * Sets this as the current level manager.
 */
void ALevelManager::PostActorCreated()
{
	Super::PostActorCreated();
	if (IsValid(GetWorld()) && IsValid(Cast<UVoidsingerGameInstance>(UGameplayStatics::GetGameInstance(this))))
	{
		Cast<UVoidsingerGameInstance>(UGameplayStatics::GetGameInstance(this))->SetLevelManager(this);
	}
}