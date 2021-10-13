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
    Camera->SetUsingAbsoluteRotation(true);
    Camera->SetRelativeLocation(FVector(0, 0, CameraHeight));
    Camera->SetRelativeRotation(FRotator(-90, -90, 90));

    AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void APlayerShip::Tick(float DeltaTime)
{
    FVector WorldDirection = FVector();
    FVector WorldLocation = FVector();

    GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
    WorldLocation = FMath::LinePlaneIntersection(Camera->GetComponentLocation(), Camera->GetComponentLocation() + WorldDirection * 10000, FPlane(GetActorLocation(), FVector(0, 0, 1)));

    TargetLookDirection = (WorldLocation - GetActorLocation()).GetSafeNormal2D();

    

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


    //Movement Bindings
    InputComponent->BindAction("MoveForward", IE_Pressed, this, &APlayerShip::MoveForwardPressedCall);
    InputComponent->BindAction("MoveBackward", IE_Pressed, this, &APlayerShip::MoveBackwardPressedCall);
    InputComponent->BindAction("MoveRight", IE_Pressed, this, &APlayerShip::MoveRightPressedCall);
    InputComponent->BindAction("MoveLeft", IE_Pressed, this, &APlayerShip::MoveLeftPressedCall);

    InputComponent->BindAction("MoveForward", IE_Released, this, &APlayerShip::MoveForwardReleasedCall);
    InputComponent->BindAction("MoveBackward", IE_Released, this, &APlayerShip::MoveBackwardReleasedCall);
    InputComponent->BindAction("MoveRight", IE_Released, this, &APlayerShip::MoveRightReleasedCall);
    InputComponent->BindAction("MoveLeft", IE_Released, this, &APlayerShip::MoveLeftReleasedCall);
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

void APlayerShip::MoveForwardPressedCall()
{
    bDecelerating = false;
    SetTargetMoveDirection(FVector2D(1, 0));
}

void APlayerShip::MoveBackwardPressedCall()
{
    bDecelerating = false;
    SetTargetMoveDirection(FVector2D(-1, 0));
}

void APlayerShip::MoveRightPressedCall()
{
    bDecelerating = false;
    SetTargetMoveDirection(FVector2D(0, 1));
}

void APlayerShip::MoveLeftPressedCall()
{
    bDecelerating = false;
    SetTargetMoveDirection(FVector2D(0, -1));
}

void APlayerShip::MoveForwardReleasedCall()
{
    bDecelerating = true;
}

void APlayerShip::MoveBackwardReleasedCall()
{
    bDecelerating = true;
}

void APlayerShip::MoveRightReleasedCall()
{
    bDecelerating = true;
}

void APlayerShip::MoveLeftReleasedCall()
{
    bDecelerating = true;
}

void APlayerShip::AddVoidsongInput(int Input)
{
    //Add the input to the list of inputs
    VoidsongCombo.Emplace(Input);

    //Reset the reset voidsong timer
    ResetVoidsongTimer = 0;
    ShouldResetVoidsongTimerTick = true;

    //Play the voidsong instrument
    if (IsValid(VoidsongInstrument))
    {
        VoidsongInstrument->GetParameterInterface()->Trigger(FName(FString::FromInt(Input)));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("The voidsong instrument isn't valid! Set it on the player ship."));
    }
}

void APlayerShip::ActivateVoidsong()
{
    //Calls play voidsong with the player's current sequence of inputs
    PlayVoidsong(VoidsongCombo);
    ResetVoidsong();
}

void APlayerShip::ResetVoidsong()
{
    //Empties the sequence of inputs
    VoidsongCombo.Empty();
    ShouldResetVoidsongTimerTick = false;
}




