// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"

UBasePart::UBasePart()
{
	
}

const TArray<FIntPoint> UBasePart::GetDesiredShape()
{
	TArray<FIntPoint> TempShap = TArray<FIntPoint>();
	for (int i = 0; i < DesiredShape.Num(); i++)
	{
		switch (Rotation)
		{
		case Degrees0:
			TempShap.Add(DesiredShape[i]);
			break;
		case Degrees90:
			TempShap.Add(FIntPoint(-DesiredShape[i].Y, DesiredShape[i].X));
			break;
		case Degrees180:
			TempShap.Add(DesiredShape[i] * -1);
			break;
		case Degrees270:
			TempShap.Add(FIntPoint(DesiredShape[i].Y, -DesiredShape[i].X));
			break;
		default:
			break;
		}
	}
	return DesiredShape;
}

const FIntPoint UBasePart::GetShapeBounds()
{
	FIntPoint Max = GetDesiredShape()[0];
	UE_LOG(LogTemp, Warning, TEXT("before X=%d, Y=%d"), Max.X, Max.Y)
	for (int i = 0; i < GetDesiredShape().Num(); i++)
	{
		if (GetDesiredShape()[i].X > Max.X)
		{
			Max.X = GetDesiredShape()[i].X;
		}
		if (GetDesiredShape()[i].Y > Max.Y)
		{
			Max.Y = GetDesiredShape()[i].Y;
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
