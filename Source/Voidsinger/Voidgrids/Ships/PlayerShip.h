#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
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

	/* ------------------ *\
	\* \/ Initializers \/ */

	/**
	 * Initializes the camera and posesses the player.
	 */
	APlayerShip();

	/**
	 * Binds movement, camera control, and voidsong input.
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * Initializes the Voidsong Instrument
	 */
	virtual void BeginPlay() override;

	/* /\ Initializers /\ *\
	\* ------------------ */

	/* ---------- *\
	\* \/ Tick \/ */

	/**
	 * Tick override for checking if enough time has passed for the Voidsong sequence to be cleared
	 * 
	 * @param DeltaTime - The time between ticks
	 */
	virtual void Tick(float DeltaTime) override;

	/* /\ Tick /\ *\
	\* ---------- */

	/**
	 * Repairs a random pixel.
	 */
	UFUNCTION(BlueprintCallable)
	void Repair();

	/* ------------ *\
	\* \/ Camera \/ */

public:
	//Store a reference to the player's camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	//Stores a reference to the attachment point of the camera on the ship.
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

	/**
	 * Clears the current sequence of Voidsong inputs
	 */
	void ClearVoidsongSequence();

public:

	//The time the player has to wait without playing any Voidsong inputs to clear the Voidsong sequence
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta =  (ClampMin = "0"))
	float VoidsongClearTime = 3.0;

	//The asset to use as the Voidsong instrument
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> VoidsongInstrumentAsset;

private:

	//A reference to the Voidsong instrument
	UAudioComponent* VoidsongInstrument;

	//The sequence of Voidsong inputs that the player has played
	TArray<int32> VoidsongSequence;

	//Stores the time since the last Voidsong input was played
	float TimeSinceLastVoidsongInput = 0;

	/* /\ Voidsong Input Handling /\ *\
	\* ----------------------------- */

	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

private:

	/**
	 * Plays a Voidsong with the Motifs played in the sequence
	 * 
	 * @param Sequence - The sequence that was played
	 */
	void PlaySequence(TArray<int32> Sequence);

	/**
	 * Parses a sequence into separate Motifs
	 * 
	 * @param Sequence- The sequence to parse
	 * @param OutFactions - The Faction Motifs found
	 * @param OutNouns - The Noun Motifs found
	 * @param OutVerbs - The Verb Motifs found
	 */
	void ParseSequenceIntoMotifData(TArray<int32> Sequence, TArray<UBaseFactionMotif*>& OutFactions, TArray<UBaseNounMotif*>& OutNouns, TArray<UBaseVerbMotif*>& OutVerbs);

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */

	/* ------------- *\
	\* \/ Faction \/ */

public:

	/**
	 * Gets the faction of the Voidgrid.
	 *
	 * @return The faction of the Voidgrid.
	 */
	virtual EFaction GetFaction() const override;

	/* /\ Faction /\ *\
	\* ------------- */
};
