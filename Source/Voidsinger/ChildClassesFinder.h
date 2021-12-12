// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChildClassesFinder.generated.h"

/**
 * Stores an array of child classes
 */
USTRUCT(BlueprintType)
struct VOIDSINGER_API FChildClasses
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<UClass*> Classes;

	FChildClasses(TArray<UClass*> ClassArray = TArray<UClass*>())
		: Classes(ClassArray)
	{}

	void Add(UClass* Class)
	{
		Classes.Add(Class);
	}
};

/**
 * Finds all child classes of the given Parent Classes.
 */
UCLASS(EditInlineNew)
class VOIDSINGER_API UChildClassesFinder : public UObject
{
	GENERATED_BODY()

public:
	//Initilzes ChildClassesMap.
	UChildClassesFinder();

	//The classes to find the children of.
	UPROPERTY(EditDefaultsOnly)
	TArray<UClass*> ParentClasses;

	/**
	 * Gets a map linking parent classes to thier children.
	 * 
	 * @return A map linking parent classes to thier children.
	 */
	UFUNCTION(BlueprintPure)
	TMap<UClass*, FChildClasses> GetChildClassesMap() const;

	//Iterates though all assets to propagate ChildClassesMap.
	UFUNCTION(BlueprintCallable)
	void GenerateChildClassesMap();

private:
	//Stores a map linking parent classes to thier children.
	UPROPERTY()
	TMap<UClass*, FChildClasses> ChildClassesMap;
};
