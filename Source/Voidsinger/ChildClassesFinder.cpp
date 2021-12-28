// Fill out your copyright notice in the Description page of Project Settings.


#include "ChildClassesFinder.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ChildClassesFinder.h"

//This is a modifed version of the code created by Zhi_Kang_Shao here: https://forums.unrealengine.com/t/tobjectiterator-uclass-skipping-unloaded-assets/68249/3
UChildClassesFinder::UChildClassesFinder()
{
	ChildClassesMap = TMap<UClass*, FChildClasses>();
}


TMap<UClass*, FChildClasses> UChildClassesFinder::GetChildClassesMap() const
{
	return ChildClassesMap;
}

void UChildClassesFinder::GenerateChildClassesMap()
{
	if (ChildClassesMap.Num() == 0)
	{
		//Initalize ChildClassesMap using ParentClasses
		for (UClass* ParentClass : ParentClasses)
		{
			ChildClassesMap.Emplace(ParentClass);
		}

		//Load asset registry module
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
		IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

		//Scan specific path
		TArray<FString> PathsToScan;
		PathsToScan.Add(TEXT("/Game/"));
		AssetRegistry.ScanPathsSynchronous(PathsToScan);

		//Get all assets in the path, does not load them
		TArray<FAssetData> ScriptAssetList;
		AssetRegistry.GetAssetsByPath(FName("/Game/"), ScriptAssetList, /*bRecursive=*/true);

		for (const FAssetData& Asset : ScriptAssetList)
		{
			//Skip non blueprint assets
			const UBlueprint* BlueprintObj = Cast<UBlueprint>(Asset.GetAsset());
			if (!BlueprintObj)
				continue;

			//Check whether blueprint class has parent class we're looking for
			UClass* BlueprintClass = BlueprintObj->GeneratedClass;
			if (BlueprintClass)
			{
				for (UClass* ParentClass : ParentClasses)
				{
					if (BlueprintClass->IsChildOf(ParentClass))
					{
						ChildClassesMap.Find(ParentClass)->Add(BlueprintClass);
					}
				}
			}
		}
	}
}