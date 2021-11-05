// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShip.h"
#include "Voidsinger/Parts/BasePart.h"

APlayerShip::APlayerShip()
{
    Pixels=55;
    
    if (!CameraHeight)
    {
        CameraHeight = FMath::GetMappedRangeValueClamped(FVector2D(0,1), FVector2D(CameraMinHeight, CameraMaxHeight), 0.5);
    }

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Camera->SetUsingAbsoluteRotation(true);
    Camera->SetRelativeLocation(FVector(0, 0, CameraHeight));
    Camera->SetRelativeRotation(FRotator(-90, -90, 90));

    AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void APlayerShip::Tick(float DeltaTime)
{
    if (!bBuildMode)
    {
        FVector WorldDirection = FVector();
        FVector WorldLocation = FVector();

        GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
        WorldLocation = FMath::LinePlaneIntersection(Camera->GetComponentLocation(), Camera->GetComponentLocation() + WorldDirection * 10000, FPlane(GetActorLocation(), FVector(0, 0, 1)));

        SetTargetLookDirection((WorldLocation - GetActorLocation()));

        FVector2D MoveInputAxis = FVector2D(!bMovementInputInvert ? GetInputAxisValue("MoveForward") : 0, bMovementInputInvert ? GetInputAxisValue("MoveRight") : 0);
        //UE_LOG(LogTemp, Warning, TEXT("InputAxis: %s"), * MoveInputAxis.ToString());
        SetTargetMoveDirection(MoveInputAxis == FVector2D(0,0) ? FVector2D(1, 0) : MoveInputAxis);
        SetTargetMoveSpeed(FMath::Min(MoveInputAxis.SizeSquared(), 1.f) * PlayerMaxSpeed);
    }
    

    

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

    //Camera Bindings
    InputComponent->BindAxis("Zoom", this, &APlayerShip::ZoomAxisCall);
    

    //Build Mode Bindings
    InputComponent->BindAction("ToggleBuildMode", IE_Pressed, this, &APlayerShip::ToggleBuildUICall);


    //Move
    InputComponent->BindAction("ToggleStrafe", IE_Pressed, this, &APlayerShip::InvertMoveDirection);
    InputComponent->BindAction("ToggleStrafe", IE_Released, this, &APlayerShip::InvertMoveDirection);
    InputComponent->BindAxis("MoveForward");
    InputComponent->BindAxis("MoveRight");
}

void APlayerShip::BeginPlay()
{
    Super::BeginPlay();

    //Spawns the voidsong instrument
    VoidsongInstrument = UGameplayStatics::SpawnSound2D(this, Cast<USoundBase>(VoidsongInstrumentAsset.LoadSynchronous()));
    SetTargetMoveDirection(FVector2D(1, 0));
    
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

void APlayerShip::AddCameraLocation(FVector2D DeltaLoc)
{
    SetCameraLocation(FVector2D(Camera->GetRelativeLocation()) + DeltaLoc - PhysicsComponent->GetVelocity() * CameraVelocityAdjScaling);
}

void APlayerShip::SetCameraLocation(FVector2D NewLoc)
{
    Camera->SetRelativeLocation(FVector(NewLoc + PhysicsComponent->GetVelocity() * CameraVelocityAdjScaling, CameraHeight));
}

void APlayerShip::SetCameraZoom(float Percent)
{
    CameraHeight = FMath::Lerp(CameraMinHeight, CameraMaxHeight, FMath::Clamp(Percent, 0.f, 1.f));
    AddCameraLocation(FVector2D(0,0));
}

const float APlayerShip::GetCameraZoom()
{
    return FMath::GetMappedRangeValueUnclamped(FVector2D(CameraMinHeight, CameraMaxHeight), FVector2D(0,1), CameraHeight);
}

void APlayerShip::ZoomAxisCall(float AxisValue)
{
    SetCameraZoom(GetCameraZoom() + -1 * AxisValue * CameraZoomAxisScaling);
}

void APlayerShip::ToggleBuildUICall()
{
    SetBuildMode(!IsInBuildMode());
}

void APlayerShip::SetBuildMode(bool NewBuildMode)
{
    bBuildMode = NewBuildMode;
    if (bBuildMode)
    {
        Camera->AddWorldRotation(GetActorQuat());
        OpenBuildMenu();
    }
    else
    {
        Camera->AddWorldRotation(GetActorQuat().Inverse());
        CloseBuildMenu();
    }
}

bool APlayerShip::IsInBuildMode()
{
    return bBuildMode;
}

void APlayerShip::AddVoidsongInput(int Input)
{
    //Add the input to the list of inputs
    VoidsongCombo.Emplace(Input);

    //Call the event dispatcher
    OnVoidsongInputDelegate.Broadcast(FIntArray(VoidsongCombo));

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
    PlaySequence(VoidsongCombo);
    ResetVoidsong();
}

void APlayerShip::ResetVoidsong()
{
    //Empties the sequence of inputs
    VoidsongCombo.Empty();
    ShouldResetVoidsongTimerTick = false;

    OnResetVoidsongDelegate.Broadcast();
}

void APlayerShip::InvertMoveDirection()
{
    bMovementInputInvert = !bMovementInputInvert;
}




