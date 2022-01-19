#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "Camera/CameraComponent.h"
#include "PlayerShip.generated.h"

/**
 * A ship controllable by a player.
 * Handles movement input actions.
 */
UCLASS()
class VOIDSINGER_API APlayerShip : public AShip
{
	GENERATED_BODY()


public:
	/**
	 * Inilizes the camrea and posesses the player.
	 */
	APlayerShip();

	/**
	 * Binds movment, camera control, and voidsong input.
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Repairs a random pixel.
	 */
	UFUNCTION(BlueprintCallable)
	void Repair();

	/* ------------ *\
	\* \/ Camera \/ */

public:
	//Store a renfernce to the players camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	//Stores a refernce to the atacment point of the camera on the ship.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USceneComponent* CameraRoot;

	/**
	 * Sets the zoom percent.
	 * 
	 * @param Percent - The new zoom percent.
	 */
	UFUNCTION(BlueprintCallable)
	void SetCameraZoom(float Percent);

	/**
	 * Gets the current zoom percent.
	 *
	 * @return The current zoom percent.
	 */
	UFUNCTION(BlueprintCallable)
	float GetCameraZoom() const;

	/**
	 * Updates the zoom percent by the given scaled input.
	 * 
	 * @param AxisValue - The the input axis value that will be used to update the zoom percent.
	 */
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
	//Stores the current camera zoom percent.
	UPROPERTY()
	float CameraZoomPercent{ 0.f };

	/* /\ Camera /\ *\
	\* ------------ */

	/* ---------------------------- *\
	\* \/ Voidsong Input Actions \/ */

private:

	/**
	 * Input action for Voidsong input 1
	 */
	void Voidsong1InputAction();

	/**
	 * Input action for Voidsong input 2
	 */
	void Voidsong2InputAction();

	/**
	 * Input action for Voidsong input 3
	 */
	void Voidsong3InputAction();

	/**
	 * Input action for Voidsong input 4
	 */
	void Voidsong4InputAction();

	/**
	 * Input action for Voidsong input 5
	 */
	void Voidsong5InputAction();

	/**
	 * Input action for playing a Voidsong
	 */
	void PlayVoidsongInputAction();

	/* /\ Voidsong Input Actions /\ *\
	\* ---------------------------- */

	/* ----------------------------- *\
	\* \/ Voidsong Input Handling \/ */

private:

	/**
	 * Records the incoming Voidsong input
	 * 
	 * @param Input - The input that was pressed
	 */
	void RecordVoidsongInput(int32 Input);


	/* /\ Voidsong Input Handling /\ *\
	\* ----------------------------- */

};
