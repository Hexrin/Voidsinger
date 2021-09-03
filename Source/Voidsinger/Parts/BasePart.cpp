// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePart.h"

UBasePart::UBasePart()
{
}

const TArray<FIntPoint> UBasePart::GetDesiredShape()
{
	TArray<FIntPoint> TempShape = TArray<FIntPoint>();
	for (int i = 0; i < _DesiredShape.Num(); i++)
	{
		switch (Rotation)
		{
		case Degrees0:
			TempShape.Add(_DesiredShape[i]);
			break;
		case Degrees90:
			TempShape.Add(FIntPoint(-DesiredShape[i].Y, DesiredShape[i].X));
			break;
		case Degrees180:
			TempShape.Add(DesiredShape[i] * -1);
			break;
		case Degrees270:
			TempShape.Add(FIntPoint(DesiredShape[i].Y, -DesiredShape[i].X));
			break;
		default:
			break;
		}
	}
	return TempShape;
}

const TArray<FIntPoint> UBasePart::GetDesiredShape(TEnumAsByte<EPartRotation> Rot)
{
	TArray<FIntPoint> TempShape = TArray<FIntPoint>();
	for (int i = 0; i < _DesiredShape.Num(); i++)
	{
		switch (Rot)
		{
		case Degrees0:
			TempShape.Add(_DesiredShape[i]);
			break;
		case Degrees90:
			TempShape.Add(FIntPoint(-_DesiredShape[i].Y, _DesiredShape[i].X));
			break;
		case Degrees180:
			TempShape.Add(_DesiredShape[i] * -1);
			break;
		case Degrees270:
			TempShape.Add(FIntPoint(_DesiredShape[i].Y, -_DesiredShape[i].X));
			break;
		default:
			break;
		}
	}
	return TempShape;
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

const FIntPoint UBasePart::GetLocation()
{
	return FIntPoint();
}

const TEnumAsByte<EPartRotation> UBasePart::GetRotation()
{
	return TEnumAsByte<EPartRotation>();
}

const TArray<FIntPoint> UBasePart::GetShape()
{
	TArray<FIntPoint> ActualShape = GetDesiredShape();
	for (int i = 0; i < DestroyedPixels.Num(); i++)
	{
		ActualShape.Remove(DestroyedPixels[i]);
	}
	return ActualShape;
}

float UBasePart::GetMass()
{
	return _Mass/ _DesiredShape.Num();
}

void UBasePart::Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGrid)
{
	Rotation = Rot;
	Location = Loc;
	PartGridComponent = PartGrid;
	_Mass = Mass;
	_Cost = Cost;
	_DesiredShape = DesiredShape;
}
