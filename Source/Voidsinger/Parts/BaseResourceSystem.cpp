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
	StartScanSystemForBreaks();
}

void UBaseResourceSystem::MergeSystems(UBaseResourceSystem* MergedSystem)
{
	ConnectedParts.Append(MergedSystem->ConnectedParts);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->ResourceSystems.Remove(MergedSystem);

}

void UBaseResourceSystem::CreateNewSystem(TArray<UBasePart*> RemovedParts)
{
	for (int i = 0; i < RemovedParts.Num(); i++)
	{
		ConnectedParts.Remove(RemovedParts[i]);
	}


	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>(ThisClass::StaticClass()));
	NewSystem->AddSection(RemovedParts);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->ResourceSystems.Add(NewSystem);

}

void UBaseResourceSystem::AddSection(TArray<UBasePart*> AddedParts)
{
	ConnectedParts.Append(AddedParts);
}

void UBaseResourceSystem::RemoveSection(TArray<UBasePart*> RemovedParts)
{
	for (int i = 0; i < RemovedParts.Num(); i++)
	{
		ConnectedParts.Remove(RemovedParts[i]);
		StartScanSystemForBreaks();
	}
}

void UBaseResourceSystem::StartScanSystemForBreaks()
{
	
	ScanSystemForBreaks(ConnectedParts);

}

void UBaseResourceSystem::ScanSystemForBreaks(TArray<UBasePart*> PartsToScan)
{
	TArray<UBasePart*> DisconnectedParts = FindDisconnectedParts(PartsToScan);

	if (DisconnectedParts.IsEmpty())
	{
		for (int i = 0; i < SeparatedSystems.Num(); i++)
		{
			CreateNewSystem(SeparatedSystems[i]);
		}

		SeparatedSystems.Empty();
	}
	else
	{
		for (int i = 0; i < DisconnectedParts.Num(); i++)
		{
			PartsToScan.Remove(DisconnectedParts[i]);
		}
		SeparatedSystems.Add(PartsToScan);
		ScanSystemForBreaks(DisconnectedParts);
	}

}

bool UBaseResourceSystem::AreShapesAdjacent(TArray<FIntPoint> Shape1, TArray<FIntPoint> Shape2)
{

	for (int i = 0; i < Shape1.Num(); i++)
	{
		for (int j = 0; j < Shape2.Num(); i++)
		{
			if (UKismetMathLibrary::InRange_FloatFloat(float(Shape1[i].X), float(Shape2[j].X - 1), float(Shape2[j].X + 1)) && UKismetMathLibrary::InRange_FloatFloat(float(Shape1[i].Y), float(Shape2[j].Y - 1), float(Shape2[j].Y + 1)))
			{
				return true;
			}
		}
	}
	
	return false;
}

TArray<UBasePart*> UBaseResourceSystem::FindDisconnectedParts(TArray<UBasePart*> Parts)
{
	TArray<UBasePart*> DisconnectedParts;
	TArray<FIntPoint> CombinedShape;

	CombinedShape.Append(Parts[0]->GetShape());

	for (int i = 1; i < Parts.Num(); i++)
	{

		if (AreShapesAdjacent(CombinedShape, Parts[i]->GetShape()))
		{
			CombinedShape.Append(Parts[i]->GetShape());
		}
		else
		{
			DisconnectedParts.Add(Parts[i + 1]);
		}
	}
	return DisconnectedParts;
}
