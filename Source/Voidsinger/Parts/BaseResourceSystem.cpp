// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Parts/BaseResourceSystem.h"
#include "Voidsinger/Ships/BaseShip.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UBaseResourceSystem::UBaseResourceSystem()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resource system init"));
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
	ScanSystemForBreaks();
}

void UBaseResourceSystem::MergeSystems(UBaseResourceSystem* MergedSystem)
{
	ConnectedParts.Append(MergedSystem->ConnectedParts);
	if (IsValid(GetWorld()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Merge Systems"));
		Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->RemoveResourceSystem(MergedSystem);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The world is not valid on the resource system for some unexplicable reason (ask Mabel)"));
	}
}

void UBaseResourceSystem::CreateNewSystem(TArray<UBasePart*> RemovedParts)
{
	for (int i = 0; i < RemovedParts.Num(); i++)
	{
		ConnectedParts.Remove(RemovedParts[i]);
	}

	UBaseResourceSystem* NewSystem = (NewObject<UBaseResourceSystem>(ThisClass::StaticClass()));
	NewSystem->AddSection(RemovedParts);
	Cast<ABaseShip>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))->AddResourceSystem(NewSystem);

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
		ScanSystemForBreaks();
	}
}

void UBaseResourceSystem::ScanSystemForBreaks()
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
			CreateNewSystem(SeparatedSystems[i].UBasePartArray);
		}

		SeparatedSystems.Empty();
	}
	else
	{
		for (int i = 0; i < DisconnectedParts.Num(); i++)
		{
			PartsToScan.Remove(DisconnectedParts[i]);
		}
		SeparatedSystems.Add(FUBasePartArray(PartsToScan));
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

TEnumAsByte<EResourceType> UBaseResourceSystem::GetType()
{
	return SystemType;
}

void UBaseResourceSystem::SetType(TEnumAsByte<EResourceType> Type)
{
	//UE_LOG(LogTemp, Warning, TEXT("New Resource System Type = %i"), Type.GetValue());
	SystemType = Type;
}

UWorld* UBaseResourceSystem::GetWorld() const
{

	if (ConnectedParts.Num() > 0)
	{
		return ConnectedParts[0]->GetWorld();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("You forgot to add a part to the system before calling MergeSystem() or CreateNewSystem(). AddPart(). Ask Mabel."));
		return nullptr;
	}
}
