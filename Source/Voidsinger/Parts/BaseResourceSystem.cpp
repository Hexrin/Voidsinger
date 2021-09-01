// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Parts/BaseResourceSystem.h"
#include "Voidsinger/Ships/BaseShip.h"

UBaseResourceSystem::UBaseResourceSystem()
{

}

//void UBaseResourceSystem::AddResources(float Amount)
//{
//	ResourceAmount += Amount;
//}
//
//void UBaseResourceSystem::DrawResources(float Amount)
//{
//	ResourceAmount -= Amount;
//}
//
//void UBaseResourceSystem::AddPart(UBasePart* AddedPart)
//{
//	ConnectedParts.Add(AddedPart);
//}
//
//void UBaseResourceSystem::RemovePart(UBasePart* RemovedPart)
//{
//	ConnectedParts.Remove(RemovedPart);
//}
//
//void UBaseResourceSystem::MergeSystems(UBaseResourceSystem* MergedSystem)
//{
//	ConnectedParts.Append(MergedSystem->ConnectedParts);
//	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->ResourceSystems.Remove(MergedSystem);
//
//}
//
//void UBaseResourceSystem::RemoveSection(TArray<UBasePart*> RemovedParts)
//{
//	for (int i = 0; i < RemovedParts.Num(); i++)
//	{
//		ConnectedParts.Remove(RemovedParts[i]);
//	}
//
//
//	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>(UBaseResourceSystem::StaticClass()));
//	NewSystem->AddSection(RemovedParts);
//	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->ResourceSystems.Add(NewSystem);
//
//}
//
//void UBaseResourceSystem::AddSection(TArray<UBasePart*> AddedParts)
//{
//	ConnectedParts.Append(AddedParts);
//}
//
//void UBaseResourceSystem::ScanSystemForBreaks()
//{
//	TArray<TArray<UBasePart*>> SeparatedSystems;
//
//
//	
//
//
//}
//
//bool UBaseResourceSystem::AreShapesAdjacent(TArray<FIntPoint> Shape1, TArray<FIntPoint> Shape2)
//{
//
//	TArray<FIntPoint> Shape = Part->GetShape();
//
//	for (int i = 0; i < PartsToCheck.Num(); i++)
//	{
//		for (int j = 0; j < PartsToCheck[i]->GetShape().Num(); j++)
//		{
//			for (int k = 0; k < Shape.Num(); k++)
//			{
//				if (UKismetMathLibrary::InRange_FloatFloat(float(PartsToCheck[i]->GetShape()[j].X), float(Shape[k].X - 1), float(Shape[k].X + 1)))
//				{
//					return true;
//				}
//			}
//		}
//	}
//	return false;
//}
//
//TArray<UBasePart*> UBaseResourceSystem::FindDisconnectedParts(TArray<UBasePart*> Parts)
//{
//	TArray<UBasePart*> DisconnectedParts;
//
//	for (int i = 0; i < Parts.Num(); i++)
//	{
//
//		if (!IsPartAdjacent(Parts[i]))
//		{
//			DisconnectedParts.Add(Parts[i]);
//		}
//
//	}
//	return DisconnectedParts;
//}
