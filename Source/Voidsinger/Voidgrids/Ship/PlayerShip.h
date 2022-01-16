#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "Camera/CameraComponent.h"
#include "PlayerShip.generated.h"

/**
 *
 */
UCLASS()
class VOIDSINGER_API APlayerShip : public AVoidgrid
{
	GENERATED_BODY()


public:
	/* ------------ *\
	\* \/ Camera \/ */

public:
	//Store a renfernce to the players camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* Camera;

	//Stores a refernce to the atacment point of the camera on the ship.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USceneComponent* CameraRoot;

	//Sets the zoom percent.
	UFUNCTION(BlueprintCallable)
		void SetCameraZoom(float Percent);

	//Gets the zoom percent of the camera.
	UFUNCTION(BlueprintCallable)
		float GetCameraZoom() const;

	//Updates the zoom percent by the given scaled input.
	UFUNCTION()
		void ZoomAxisInput(float AxisValue);

protected:

	//The minimum zoom of the camera.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
		float CameraMinHeight{ 50 };

	//The maximum zoom of the camera.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
		float CameraMaxHeight{ 1500 };

	//The amount zoom input is scaled by.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
		float CameraZoomAxisScaling{ 0.01 };

	//The power of the zoom funtion. The higher the number the faster it zooms out at large zooms.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
		float CameraZoomPower{ 2 };

private:

	//Comment -Mabel Suggestion
	UPROPERTY(EditAnywhere)
		float CameraHeight;

	/* /\ Camera /\ *\
	\* ------------ */

};
