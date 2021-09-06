// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrary.h"

FIntPoint UFunctionLibrary::RotateIntPoint(FIntPoint IntPoint, TEnumAsByte<EPartRotation> Rotation) 
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
		return FIntPoint(IntPoint.Y, -IntPoint.X);

	default:
		
		return IntPoint;
	}

}

//bool UFunctionLibrary::PointsConected(TMap<FIntPoint, UBasePart*> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConectivityArray)
//{
//	if (StartPoint == EndPoint)
//	{
//		return true;
//	}
//	bool ReturnValue = false;
//	for (int i = 0; i < )
//	{
//
//	}
//	
//	if (PartGrid.Contains(StartPoint + FIntPoint(1, 0)) && !ReturnValue)
//	{
//		ReturnValue = PointsConected(PartGrid, StartPoint + FIntPoint(1, 0), EndPoint, ConectivityArray);
//	}
//	
//	return false;
//}
