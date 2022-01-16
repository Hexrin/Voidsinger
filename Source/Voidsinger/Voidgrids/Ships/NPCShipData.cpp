// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCShipData.h"

FPrimaryAssetId UNPCShipData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FName("PartData"), GetOutermost()->GetFName());
}