// Fill out your copyright notice in the Description page of Project Settings.


#include "PartData.h"

FPrimaryAssetId UPartData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FName("PartData"), GetOutermost()->GetFName());
}