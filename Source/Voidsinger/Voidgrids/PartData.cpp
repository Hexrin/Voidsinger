// Fill out your copyright notice in the Description page of Project Settings.


#include "PartData.h"

FPrimaryAssetId UPartData::GetPrimaryAssetId() const
{
	FString Thing = GetClass()->GetName();
	Thing.RemoveFromEnd("_C");
	return FPrimaryAssetId(FName("PartData"), FName(Thing));
}