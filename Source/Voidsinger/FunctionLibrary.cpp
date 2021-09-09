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
bool UFunctionLibrary::PointsConnected(TMap<FIntPoint, UBasePart*> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint)
{
	TArray<FIntPoint> ConectivityArray = TArray<FIntPoint>();
	return PointsConnected(PartGrid, StartPoint, EndPoint, ConectivityArray);
}
bool UFunctionLibrary::PointsConnected(TMap<FIntPoint, UBasePart*> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConnectivityArray)
{
	
	if (StartPoint == EndPoint)
	{
		return true;
	}
	
	ConnectivityArray.Emplace(StartPoint);

	bool ReturnValue = false;

	const bool IsXCloser = abs((EndPoint - StartPoint).X) < abs((EndPoint - StartPoint).Y);
	bool XIsPosive = (EndPoint - StartPoint).X > 0;
	bool YIsPosive = (EndPoint - StartPoint).Y > 0;
	UE_LOG(LogTemp, Warning, TEXT("Direction x=%i, y=%i"), (EndPoint - StartPoint).X, (EndPoint - StartPoint).Y);

	for (int i = 0; i < 4; i++)
	{
		FIntPoint TargetPoint = (!IsXCloser ^ (i % 2 == 1)) ? FIntPoint((XIsPosive ^ (i > 1)) ? 1 : -1, 0) : FIntPoint(0, (YIsPosive ^ (i > 1)) ? 1 : -1);
		UE_LOG(LogTemp, Warning, TEXT("Target Point x=%i, y=%i, Xclose=%i, Xpos=%i, Ypos=%i"), TargetPoint.X, TargetPoint.Y, (IsXCloser ^ (i % 2 == 1)) ? 1 : 0, !(XIsPosive ^ (i > 1)) ? 1 : 0, !(YIsPosive ^ (i > 1)) ? 1 : 0);
		if (!ConnectivityArray.Contains(StartPoint + TargetPoint) && PartGrid.Contains(StartPoint + TargetPoint))
		{
			ReturnValue = PointsConnected(PartGrid, StartPoint + TargetPoint, EndPoint, ConnectivityArray);
			if (ReturnValue)
			{
				break;
			}
		}
	}
	return ReturnValue;
}
