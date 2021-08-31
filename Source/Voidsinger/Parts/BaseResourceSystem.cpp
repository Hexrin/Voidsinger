// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Parts/BaseResourceSystem.h"
#include "Voidsinger/Ships/BaseShip.h"

UBaseResourceSystem::UBaseResourceSystem()
{

}

void UBaseResourceSystem::AddResources(float Amount)
{
	ResourceAmount += Amount;
}

void UBaseResourceSystem::DrawResources(float Amount)
{
	ResourceAmount -= Amount;
}

void UBaseResourceSystem::AddPart(UBasePart* AddedPart)
{
	ConnectedParts.Add(AddedPart);
}

void UBaseResourceSystem::RemovePart(UBasePart* RemovedPart)
{
	ConnectedParts.Remove(RemovedPart);
}

void UBaseResourceSystem::MergeSystems(UBaseResourceSystem* MergedSystem)
{
	ConnectedParts.Append(MergedSystem->ConnectedParts);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->ResourceSystems.Remove(MergedSystem);

}

void UBaseResourceSystem::RemoveSection(TArray<UBasePart*> RemovedParts)
{
	for (int i = 0; i < RemovedParts.Num(); i++)
	{
		ConnectedParts.Remove(RemovedParts[i]);
	}


	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>(UBaseResourceSystem::StaticClass()));
	NewSystem->ConnectedParts = RemovedParts;
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->ResourceSystems.Add(NewSystem);

}
