#include "PlayerShip.h"

void ABaseShip::SetCameraZoom(float Percent)
{
	CameraHeight = (CameraMaxHeight - CameraMinHeight)FMath::Power(Percent, CameraZoomPower) + CameraMinHeight;
}
