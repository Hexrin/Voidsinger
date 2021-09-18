// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShip.h"


APlayerShip::APlayerShip()
{
    Pixels=55;
    
    if (!CameraHeight)
    {
        CameraHeight = 100;
    }


    //Setup Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Camera->SetRelativeLocation(FVector(0, 0, CameraHeight));
    Camera->SetRelativeRotation(FRotator(-90, -90, 90));

    AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void APlayerShip::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Deals with the timer for resetting the voidsong inputs. 
    if (ShouldResetVoidsongTimerTick)
    {
        ResetVoidsongTimer += DeltaTime;
        if (ResetVoidsongTimer >= VoidsongResetDelay)
        {
            ResetVoidsong();
            ResetVoidsongTimer = 0.0;
        }
    }
}

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //Set up Voidsong bindings
    InputComponent->BindAction("Voidsong1", IE_Pressed, this, &APlayerShip::Voidsong1Call);
    InputComponent->BindAction("Voidsong2", IE_Pressed, this, &APlayerShip::Voidsong2Call);
    InputComponent->BindAction("Voidsong3", IE_Pressed, this, &APlayerShip::Voidsong3Call);
    InputComponent->BindAction("Voidsong4", IE_Pressed, this, &APlayerShip::Voidsong4Call);
    InputComponent->BindAction("Voidsong5", IE_Pressed, this, &APlayerShip::Voidsong5Call);
    InputComponent->BindAction("VoidsongActivate", IE_Pressed, this, &APlayerShip::ActivateVoidsong);
}

void APlayerShip::BeginPlay()
{
    Super::BeginPlay();

    //Spawns the voidsong instrument
    VoidsongInstrument = UGameplayStatics::SpawnSound2D(this, Cast<USoundBase>(VoidsongInstrumentAsset.LoadSynchronous()));
}

TMap<TEnumAsByte<EResourceType>, float> APlayerShip::GetTravelCost(class UStarSystemData* Target)
{
    TMap<TEnumAsByte<EResourceType>, float> Costs;
    
    Costs.Add(EResourceType::Fuel, 100);
    Costs.Add(EResourceType::Hydrogen, 35);
    return Costs;
}

bool APlayerShip::TravelToStarSystem(class UStarSystemData* Target)
{
    CurrentStarSystem = Target;
    return true;
}

UStarSystemData* APlayerShip::GetCurrentStarSystem()
{
    return CurrentStarSystem;
}

//Voidsong inputs
void APlayerShip::Voidsong1Call()
{
    AddVoidsongInput(1);
}

void APlayerShip::Voidsong2Call()
{
    AddVoidsongInput(2);
}

void APlayerShip::Voidsong3Call()
{
    AddVoidsongInput(3);
}

void APlayerShip::Voidsong4Call()
{
    AddVoidsongInput(4);
}

void APlayerShip::Voidsong5Call()
{
    AddVoidsongInput(5);
}

void APlayerShip::AddVoidsongInput(int input)
{
    //Add the input to the list of inputs
    VoidsongCombo.Emplace(input);

    //Reset the reset voidsong timer
    ResetVoidsongTimer = 0;
    ShouldResetVoidsongTimerTick = true;

    //Play the voidsong instrument
    VoidsongInstrument->GetParameterInterface()->Trigger(FName(FString::FromInt(input)));
}

void APlayerShip::PlayVoidsong(TArray<int> Sequence)
{
    //Check the available voidsongs and see if their activation sequence matches the sequence inputted. If so, activate that voidsong.
    for (auto& i : AvailableVoidsongs)
    {
        if (i->ActivationCombo == Sequence)
        {
            i->Activate();
            ResetVoidsong();
        }
    }
}

void APlayerShip::ActivateVoidsong()
{
    //Calls play voidsong with the player's current sequence of inputs
    PlayVoidsong(VoidsongCombo);
}

void APlayerShip::ResetVoidsong()
{
    //Empties the sequence of inputs
    VoidsongCombo.Empty();
    ShouldResetVoidsongTimerTick = false;
}

void APlayerShip::LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs)
{
    //Creates voidsong objects with the list of voidsongs given. Adds them to available voidsongs
    for (auto& i : Voidsongs)
    {
        AvailableVoidsongs.Emplace(NewObject<UBaseVoidsong>(this, i));
    }
}

void APlayerShip::AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong)
{
    //Creates the voidsong object from the given class and adds it to available voidsongs
    AvailableVoidsongs.Emplace(NewObject<UBaseVoidsong>(this, Voidsong));
}

