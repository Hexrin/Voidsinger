#include "PlayerShip.h"

void APlayerShip::SetCameraZoom(float Percent)
{
	//CameraHeight = (CameraMaxHeight - CameraMinHeight)FMath::Pow(Percent, CameraZoomPower) + CameraMinHeight;
}

float APlayerShip::GetCameraZoom() const
{
	return 0.0f;
}

void APlayerShip::ZoomAxisInput(float AxisValue)
{
}
