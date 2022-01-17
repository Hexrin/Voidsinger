#include "PlayerShip.h"


/**
 * Inilizes the camrea and posesses the player.
 */
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

/**
 * Binds movment, camera control, and voidsong input.
 */
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    InputComponent->BindAxis("Zoom", this, &APlayerShip::ZoomAxisInput);
}

/**
 * Repairs a random pixel pixel.
 */
void APlayerShip::Repair()
{
	RepairPixel();
}

/**
 * Sets the zoom percent.
 *
 * @param Percent - The new zoom percent.
 */
void APlayerShip::SetCameraZoom(float Percent)
{
	//Prevents the player from execeding min/max zoom.
	CameraZoomPercent = FMath::Clamp(Percent, 0.f, 1.f);
	Camera->SetRelativeLocation(FVector(0, 0, (CameraMaxHeight - CameraMinHeight) * powf(CameraZoomPercent, CameraZoomPower) + CameraMinHeight));
}

/**
 * Gets the current zoom percent.
 *
 * @return The current zoom percent.
 */
float APlayerShip::GetCameraZoom() const
{
	return CameraZoomPercent;
}

/**
 * Updates the zoom percent by the given scaled input.
 *
 * @param AxisValue - The the input axis value that will be used to update the zoom percent.
 */
void APlayerShip::ZoomAxisInput(float AxisValue)
{
	SetCameraZoom(CameraZoomPercent + AxisValue * CameraZoomAxisScaling * -1);
}
