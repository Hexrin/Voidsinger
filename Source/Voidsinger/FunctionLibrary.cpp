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
bool UFunctionLibrary::PointsConected(TMap<FIntPoint, UBasePart*> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint)
{
	TArray<FIntPoint> ConectivityArray = TArray<FIntPoint>();
	return PointsConected(PartGrid, StartPoint, EndPoint, ConectivityArray);
}
bool UFunctionLibrary::PointsConected(TMap<FIntPoint, UBasePart*> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConectivityArray)
{
	if (StartPoint == EndPoint)
	{
		return true;
	}
	bool ReturnValue = false;

	const bool IsXCloser = abs((EndPoint - StartPoint).X) > abs((EndPoint - StartPoint).Y);
	bool XIsPosive = (EndPoint - StartPoint).X > 0;
	bool YIsPosive = (EndPoint - StartPoint).Y > 0;
	UE_LOG(LogTemp, Warning, TEXT("Direction x=%i, y=%i"), (EndPoint - StartPoint).X, (EndPoint - StartPoint).Y);

	for (int i = 0; i < 3; i++)
	{
		FIntPoint TargetPoint = (IsXCloser ^ (i % 2 == 0)) ? FIntPoint((XIsPosive ^ (i > 2)) ? 1 : -1, 0) : FIntPoint(0, (YIsPosive ^ (i > 2)) ? 1 : -1);
		UE_LOG(LogTemp, Warning, TEXT("Target Point x=%i, y=%i"), TargetPoint.X, TargetPoint.Y);
		if (PartGrid.Contains(StartPoint + TargetPoint))
		{
			ReturnValue = PointsConected(PartGrid, StartPoint + TargetPoint, EndPoint, ConectivityArray);
			if (ReturnValue)
			{
				break;
			}
		}
	}
	return ReturnValue;
}
