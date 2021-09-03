// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary.h"

static FIntPoint UFunctionLibrary::RotateIntPoint(FIntPoint IntPoint, TEnumAsByte<EPartRotation> Rotation) 
{
	switch (Rotation)
	{
	case Degrees0:
		return IntPoint;

	case Degrees90:

		return FIntPoint(-IntPoint.Y, IntPoint.X);

	case Degrees180:
		return IntPoint * -1;

	case Degrees270:
		return FIntPoint(IntPoint.Y, -IntPoint.X));

	default:
		
		return IntPoint;
	}

}