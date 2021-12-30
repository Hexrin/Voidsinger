// Fill out your copyright notice in the Description page of Project Settings.


#include "EditConfigBase.h"

UObject* UEditConfigBase::GetCDO(TSubclassOf<UObject> Class)
{
	return GetMutableDefault<UObject>(Class);
}

void UEditConfigBase::SaveObjectConfig(UObject* Object)
{
	Object->SaveConfig();
}