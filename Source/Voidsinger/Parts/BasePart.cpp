// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"

UBasePart::UBasePart()
{

}

const TArray<FIntPoint> UBasePart::GetDesiredShape()
{
	return DesiredShape;
}

const FIntPoint UBasePart::GetShapeBounds()
{
	FIntPoint Max = FIntPoint(0,0);
	UE_LOG(LogTemp, Warning, TEXT("before X=%d, Y=%d"), Max.X, Max.Y)
	for (int i = 0; i < DesiredShape.Num(); i++)
	{
		if (DesiredShape[i].X > Max.X)
		{
			Max.X = DesiredShape[i].X;
		}
		if (DesiredShape[i].Y > Max.Y)
		{
			Max.Y = DesiredShape[i].Y;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("after X=%d, Y=%d"), Max.X, Max.Y)
	return Max;
}

FIntPoint UBasePart::GetPartLocation()
{
	return FIntPoint();
}

TEnumAsByte<EPartRotation> UBasePart::GetPartRotation()
{
	return TEnumAsByte<EPartRotation>();
}
