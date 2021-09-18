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

    //Make the metasound
    //static ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("MetaSoundSource'/Game/Sound/VoidsongInstrument.VoidsongInstrument'"));
    /*USoundBase* Sound = Cast<USoundBase>(VoidsongInstrumentTest.TryLoad());
    if (IsValid(Sound))
    {
        VoidsongInstrument = UGameplayStatics::SpawnSound2D(this, Sound);
    }*/

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
    InputComponent->BindAction("VoidsongActivate", IE_Pressed, this, &APlayerShip::ActivateVoidsong);
}

void APlayerShip::BeginPlay()
{
    Super::BeginPlay();
    VoidsongInstrument = UGameplayStatics::SpawnSound2D(this, Cast<USoundBase>(VoidsongInstrumentAsset.LoadSynchronous()));
}

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
    VoidsongCombo.Emplace(input);

    ResetVoidsongTimer = 0;
    ShouldResetVoidsongTimerTick = true;

    VoidsongInstrument->GetParameterInterface()->Trigger(FName(FString::FromInt(input)));
}

void APlayerShip::PlayVoidsong(TArray<int> Sequence)
{
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
    PlayVoidsong(VoidsongCombo);
}

void APlayerShip::ResetVoidsong()
{
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
}

