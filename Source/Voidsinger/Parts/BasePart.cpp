// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"

UBasePart::UBasePart()
{
	Init(FIntPoint(), EPartRotation::Degrees0);
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
	return TempShap;
}

const TArray<FIntPoint> UBasePart::GetDesiredShape(TEnumAsByte<EPartRotation> Rot)
{
	TArray<FIntPoint> TempShap = TArray<FIntPoint>();
	for (int i = 0; i < DesiredShape.Num(); i++)
	{
		switch (Rot)
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
	return TempShap;
}

const FArrayBounds UBasePart::GetShapeBounds()
{
	FArrayBounds Bounds = FArrayBounds();

	for (int i = 0; i < GetDesiredShape().Num(); i++)
	{
		if (GetDesiredShape()[i].X > Bounds.UpperBounds.X)
		{
			Bounds.UpperBounds.X = GetDesiredShape()[i].X;
		}
		if (GetDesiredShape()[i].Y > Bounds.UpperBounds.Y)
		{
			Bounds.UpperBounds.Y = GetDesiredShape()[i].Y;
		}

		if (GetDesiredShape()[i].X < Bounds.LowerBounds.X)
		{
			Bounds.LowerBounds.X = GetDesiredShape()[i].X;
		}
		if (GetDesiredShape()[i].Y < Bounds.LowerBounds.Y)
		{
			Bounds.LowerBounds.Y = GetDesiredShape()[i].Y;
		}
	}

	return Bounds;
}

const FArrayBounds UBasePart::GetShapeBounds(TEnumAsByte<EPartRotation> Rot)
{
	FArrayBounds Bounds = FArrayBounds();

	for (int i = 0; i < GetDesiredShape(Rot).Num(); i++)
	{
		if (GetDesiredShape(Rot)[i].X > Bounds.UpperBounds.X)
		{
			Bounds.UpperBounds.X = GetDesiredShape(Rot)[i].X;
		}
		if (GetDesiredShape(Rot)[i].Y > Bounds.UpperBounds.Y)
		{
			Bounds.UpperBounds.Y = GetDesiredShape(Rot)[i].Y;
		}

		if (GetDesiredShape(Rot)[i].X < Bounds.LowerBounds.X)
		{
			Bounds.LowerBounds.X = GetDesiredShape(Rot)[i].X;
		}
		if (GetDesiredShape(Rot)[i].Y < Bounds.LowerBounds.Y)
		{
			Bounds.LowerBounds.Y = GetDesiredShape(Rot)[i].Y;
		}
	}

	return Bounds;
}

FIntPoint UBasePart::GetLocation()
{
	return FIntPoint();
}

TEnumAsByte<EPartRotation> UBasePart::GetRotation()
{
	return TEnumAsByte<EPartRotation>();
}

TArray<FIntPoint> UBasePart::GetShape()
{
	return TArray<FIntPoint>();
}

void UBasePart::Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot)
{
	Rotation = Rot;
	Location = Loc;
}
