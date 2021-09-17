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

    if (ShouldResetVoidsongTimerTick)
    {
        ResetVoidsongTimer += DeltaTime;
        if (ResetVoidsongTimer >= VoidsongResetDelay)
        {
            ResetVoidsong();
            ResetVoidsongTimer = 0.0;
        }
    }

    if (ShouldPlayVoidsongTimerTick)
    {
        PlayVoidsongTimer += DeltaTime;
        if (PlayVoidsongTimer >= PlayVoidsongDelay)
        {
            PlayVoidsong(VoidsongCombo);
            PlayVoidsongTimer = 0.0;
        }
    }
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

void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //Set up Voidsong bindings
    InputComponent->BindAction("Voidsong1", IE_Pressed, this, &APlayerShip::Voidsong1Call);
    InputComponent->BindAction("Voidsong2", IE_Pressed, this, &APlayerShip::Voidsong2Call);
    InputComponent->BindAction("Voidsong3", IE_Pressed, this, &APlayerShip::Voidsong3Call);
    InputComponent->BindAction("Voidsong4", IE_Pressed, this, &APlayerShip::Voidsong4Call);
    InputComponent->BindAction("Voidsong5", IE_Pressed, this, &APlayerShip::Voidsong5Call);
}

void APlayerShip::Voidsong1Call()
{
    UE_LOG(LogTemp, Warning, TEXT("is it not recieving input?"));
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
    VoidsongCombo.Emplace(input);

    ResetVoidsongTimer = 0;
    ShouldResetVoidsongTimerTick = true;

    PlayVoidsongTimer = 0;
    ShouldPlayVoidsongTimerTick = true;

}

void APlayerShip::PlayVoidsong(TArray<int> Sequence)
{
    ShouldPlayVoidsongTimerTick = false;
    for (auto& i : AvailableVoidsongs)
    {
        if (i->ActivationCombo == Sequence)
        {
            UE_LOG(LogTemp, Warning, TEXT("Activate should be called"));
            i->Activate();
            ResetVoidsong();
        }
    }
}

void APlayerShip::ResetVoidsong()
{
    UE_LOG(LogTemp, Warning, TEXT("ResetVoidsong"));
    VoidsongCombo.Empty();
    ShouldResetVoidsongTimerTick = false;
}

void APlayerShip::LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs)
{
    for (auto& i : Voidsongs)
    {
        AvailableVoidsongs.Emplace(NewObject<UBaseVoidsong>(this, i));
    }
}

void APlayerShip::AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong)
{
    UBaseVoidsong* bruhVar = NewObject<UBaseVoidsong>(this, Voidsong);
    AvailableVoidsongs.Emplace(bruhVar);
    if (IsValid(bruhVar))
    {
        UE_LOG(LogTemp, Warning, TEXT("It is valid though"));
    }
}

