#include "PlayerShip.h"

APlayerShip::APlayerShip()
{	
	//Inilize Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(PixelMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Camera->SetUsingAbsoluteRotation(true);					//Make camera point down
	Camera->SetRelativeRotation(FRotator(-90, -90, 90));	//Make camera point down
	SetCameraZoom(0.5);
	
	//Posses this by the player.
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}


void APlayerShip::SetCameraZoom(float Percent)
{
	//Prevents the player from execeding min/max zoom.
	CameraZoomPercent = FMath::Clamp(Percent, 0.f, 1.f);
	Camera->SetRelativeLocation(FVector(0, 0, (CameraMaxHeight - CameraMinHeight) * powf(CameraZoomPercent, CameraZoomPower) + CameraMinHeight));
}

float APlayerShip::GetCameraZoom() const
{
	return CameraZoomPercent;
}

void APlayerShip::ZoomAxisInput(float AxisValue)
{
	SetCameraZoom(CameraZoomPercent + AxisValue * CameraZoomAxisScaling);
}
