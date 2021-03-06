// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EditConfigBase.generated.h"

/**
 * 
 */
UCLASS()
class EDITCONFIGFILES_API UEditConfigBase : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public: 

	UFUNCTION(BlueprintPure)
	UObject* GetCDO(TSubclassOf<UObject> Class);

	UFUNCTION(BlueprintCallable)
	void SaveObjectConfig(UObject* Object);

};
