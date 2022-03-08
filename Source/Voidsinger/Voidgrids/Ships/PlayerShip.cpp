#include "PlayerShip.h"
#include "Kismet/GameplayStatics.h"

/**
 * Initializes the camera and posesses the player.
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
 * Binds movement, camera control, and voidsong input.
 */
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    InputComponent->BindAxis("Zoom", this, &APlayerShip::ZoomAxisInput);

	// \/ Voidsong input bindings \/ /

	InputComponent->BindAction("Voidsong1", IE_Pressed, this, &APlayerShip::Voidsong1InputAction);
	InputComponent->BindAction("Voidsong2", IE_Pressed, this, &APlayerShip::Voidsong2InputAction);
	InputComponent->BindAction("Voidsong3", IE_Pressed, this, &APlayerShip::Voidsong3InputAction);
	InputComponent->BindAction("Voidsong4", IE_Pressed, this, &APlayerShip::Voidsong4InputAction);
	InputComponent->BindAction("Voidsong5", IE_Pressed, this, &APlayerShip::Voidsong5InputAction);
	InputComponent->BindAction("PlayVoidsong", IE_Pressed, this, &APlayerShip::PlayVoidsongInputAction);

	// /\ Voidsong input bindings /\ /
}

/**
 * Initializes the Voidsong Instrument
 */
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	//Initializes the Voidsong Instrument
	VoidsongInstrument = UGameplayStatics::SpawnSound2D(this, Cast<USoundBase>(VoidsongInstrumentAsset.LoadSynchronous()));
}

/* ---------- *\
\* \/ Tick \/ */

/**
 * Tick override for checking if enough time has passed for the Voidsong to be cleared
 *
 * @param DeltaTime - The time between ticks
 */
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// \/ Check if the Voidsong sequence should be cleared \/ /

	if (TimeSinceLastVoidsongInput += DeltaTime >= VoidsongClearTime)
	{
		ClearVoidsongSequence();
	}

	// /\ Check if the Voidsong sequence should be cleared /\ /

}

/* /\ Tick /\ *\
\* ---------- */

/**
 * Repairs a random pixel.
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

/* ---------------------------- *\
\* \/ Voidsong Input Actions \/ */

/**
 * Input action for Voidsong input 1
 */
void APlayerShip::Voidsong1InputAction()
{
	RecordVoidsongInput(1);
}

/**
 * Input action for Voidsong input 2
 */
void APlayerShip::Voidsong2InputAction()
{
	RecordVoidsongInput(2);
}

/**
 * Input action for Voidsong input 3
 */
void APlayerShip::Voidsong3InputAction()
{
	RecordVoidsongInput(3);
}

/**
 * Input action for Voidsong input 4
 */
void APlayerShip::Voidsong4InputAction()
{
	RecordVoidsongInput(4);
}

/**
 * Input action for Voidsong input 5
 */
void APlayerShip::Voidsong5InputAction()
{
	RecordVoidsongInput(1);
}

/**
 * Input action for playing a Voidsong
 */
void APlayerShip::PlayVoidsongInputAction()
{
	PlaySequence(VoidsongSequence);
}

/* /\ Voidsong Input Actions /\ *\
\* ---------------------------- */

/* ----------------------------- *\
\* \/ Voidsong Input Handling \/ */

/**
 * Records the incoming Voidsong input
 *
 * @param Input - The input that was pressed
 */
void APlayerShip::RecordVoidsongInput(int32 Input)
{
	//Add the input to the sequence
	VoidsongSequence.Emplace(Input);

	//Reset time since last Voidsong input 
	TimeSinceLastVoidsongInput = 0;

	// \/ Play the Voidsong Instrument \/ /

	if (IsValid(VoidsongInstrument))
	{
		//VoidsongInstrument->GetParameterInterface()->Trigger(FName(FString::FromInt(Input)));
	}
	else
	{
		//Print an error message if VoidsongInstrument isn't valid
		UE_LOG(LogTemp, Error, TEXT("Attempted to use an invalid Voidsong Instrument on %s"), *GetFName().ToString());
	}

	// /\ Play the Voidsong Instrument /\ /

}

/**
 * Clears the current sequence of Voidsong inputs
 */
void APlayerShip::ClearVoidsongSequence()
{
	VoidsongSequence.Empty();
	TimeSinceLastVoidsongInput = 0;
}

/* /\ Voidsong Input Handling /\ *\
\* ----------------------------- */

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
 * Plays a Voidsong with the Motifs played in the sequence
 *
 * @param Sequence - The sequence that was played
 */
void APlayerShip::PlaySequence(TArray<int32> Sequence)
{
	if (!Sequence.IsEmpty() && bCanPlayVoidsongs)
	{
		// \/ Find Factions, Nouns, and Verbs played \/ /

		TSet<UBaseFactionMotif*> Factions;
		TSet<UBaseNounMotif*> Nouns;
		TArray<UBaseVerbMotif*> Verbs;

		ParseSequenceIntoMotifData(Sequence, Factions, Nouns, Verbs);

		// /\ Find Factions, Nouns, and Verbs played /\ /

		//If any motifs were played, play a Voidsong with those motifs
		if (!Factions.IsEmpty() || !Nouns.IsEmpty() || !Verbs.IsEmpty())
		{
			PlayVoidsong(Factions, Nouns, Verbs);
		}
	}
}

/**
 * Parses a sequence into separate Motifs
 *
 * @param Sequence- The sequence to parse
 * @param OutFactions - The Faction Motifs found
 * @param OutNouns - The Noun Motifs found
 * @param OutVerbs - The Verb Motifs found
 */
void APlayerShip::ParseSequenceIntoMotifData(TArray<int32> Sequence, TSet<UBaseFactionMotif*>& OutFactions, TSet<UBaseNounMotif*>& OutNouns, TArray<UBaseVerbMotif*>& OutVerbs)
{
	for (UBaseMotif* EachPlayableMotif : PlayableMotifs.GetMotifs())
	{
		TArray<int32> TrimmedSequence = Sequence;
		TrimmedSequence.SetNum(EachPlayableMotif->ActivationSequence.Num());

		if (TrimmedSequence == EachPlayableMotif->ActivationSequence)
		{

			if (IsValid(Cast<UBaseFactionMotif>(EachPlayableMotif)))
			{
				OutFactions.Emplace(Cast<UBaseFactionMotif>(EachPlayableMotif));
			}
			else if (IsValid(Cast<UBaseNounMotif>(EachPlayableMotif)))
			{
				OutNouns.Emplace(Cast<UBaseNounMotif>(EachPlayableMotif));
			}
			else if (IsValid(Cast<UBaseVerbMotif>(EachPlayableMotif)))
			{
				OutVerbs.Emplace(Cast<UBaseVerbMotif>(EachPlayableMotif));
			}

			for (int EachIndex : EachPlayableMotif->ActivationSequence)
			{
				Sequence.RemoveAt(0);
			}

			if (!Sequence.IsEmpty())
			{
				ParseSequenceIntoMotifData(Sequence, OutFactions, OutNouns, OutVerbs);
			}

			break;
		}
	}
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */

/* ------------- *\
\* \/ Faction \/ */

/**
 * Gets the faction of the Voidgrid.
 *
 * @return The faction of the Voidgrid.
 */
EFaction APlayerShip::GetFaction() const
{
	return EFaction::Player;
}

/* /\ Faction /\ *\
\* ------------- */