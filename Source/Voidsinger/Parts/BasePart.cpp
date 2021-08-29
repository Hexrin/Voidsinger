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
	FIntPoint Max = FIntPoint();
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
	return Max;
}
