// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShip.h"
#include "Voidsinger/Parts/BasePart.h"

APlayerShip::APlayerShip()
{
    
    //Numbers -Mabel Suggestion
    if (!CameraHeight)
    {
        CameraHeight = FMath::GetMappedRangeValueClamped(FVector2D(0,1), FVector2D(CameraMinHeight, CameraMaxHeight), 0.5);
    }

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Camera->SetUsingAbsoluteRotation(true);
    Camera->SetRelativeLocation(FVector(0, 0, CameraHeight));

    //>:( -Mabel Suggestion
    Camera->SetRelativeRotation(FRotator(-90, -90, 90));

    AutoPossessPlayer = EAutoReceiveInput::Player0;

}

//Comment -Mabel Suggestion
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

    /*
    * Ew overcomplicated code can replaced with the following:
    * 
    *  if(ResetVoidsongTimer < VoidsongResetDelay)
    *  {
    *       ResetVoidsongTimer += DeltaTime;
    *  }
    *  else
    *  {
    *       ResetVoidsong();
    *  }
    *  
    // OR
    *  
    *  if(ResetVoidsongTimer += DeltaTime >= VoidsongResetDelay)
    *  {
    *       ResetVoidsong();
    *  }
    * 
    * - Liam Suggestion
    */

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

//Comment -Mabel Suggestion
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

//Comment -Mabel Suggestion
void APlayerShip::BeginPlay()
{
    Super::BeginPlay();

    //Spawns the voidsong instrument
    VoidsongInstrument = UGameplayStatics::SpawnSound2D(this, Cast<USoundBase>(VoidsongInstrumentAsset.LoadSynchronous()));
    SetTargetMoveDirection(FVector2D(1, 0));
    
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
//Voidsong inputs
void APlayerShip::Voidsong1Call()
{
    AddVoidsongInput(1);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void APlayerShip::Voidsong2Call()
{
    AddVoidsongInput(2);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void APlayerShip::Voidsong3Call()
{
    AddVoidsongInput(3);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void APlayerShip::Voidsong4Call()
{
    AddVoidsongInput(4);
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
void APlayerShip::Voidsong5Call()
{
    AddVoidsongInput(5);
}

//Comment -Mabel Suggestion
void APlayerShip::AddCameraLocation(FVector2D DeltaLoc)
{
    SetCameraLocation(FVector2D(Camera->GetRelativeLocation()) + DeltaLoc - PhysicsComponent->GetVelocity() * CameraVelocityAdjScaling);
}

//Comment -Mabel Suggestion
void APlayerShip::SetCameraLocation(FVector2D NewLoc)
{
    Camera->SetRelativeLocation(FVector(NewLoc + PhysicsComponent->GetVelocity() * CameraVelocityAdjScaling, CameraHeight));
}

//Comment -Mabel Suggestion
void APlayerShip::SetCameraZoom(float Percent)
{
    CameraHeight = FMath::Lerp(CameraMinHeight, CameraMaxHeight, FMath::Clamp(Percent, 0.f, 1.f));
    AddCameraLocation(FVector2D(0,0));
}

//Comment -Mabel Suggestion
const float APlayerShip::GetCameraZoom()
{
    return FMath::GetMappedRangeValueUnclamped(FVector2D(CameraMinHeight, CameraMaxHeight), FVector2D(0,1), CameraHeight);
}

//Comment -Mabel Suggestion
void APlayerShip::ZoomAxisCall(float AxisValue)
{
    SetCameraZoom(GetCameraZoom() + -1 * AxisValue * CameraZoomAxisScaling);
}

//Comment -Mabel Suggestion
void APlayerShip::ToggleBuildUICall()
{
    SetBuildMode(!IsInBuildMode());
}

//Comment -Mabel Suggestion
void APlayerShip::SetBuildMode(bool NewBuildMode)
{
    bBuildMode = NewBuildMode;
    Camera->SetAbsolute(false, !NewBuildMode, false);

    if (bBuildMode)
    {
        OpenBuildMenu();
    }
    else
    {
        CloseBuildMenu();
    }
}

//Comment -Mabel Suggestion
bool APlayerShip::IsInBuildMode()
{
    return bBuildMode;
}

/*
* Function comments from the .h should be copied to the .cpp
* 
* UE doesnt like int, use int32 or int64 instead.
*
* Confusing Name
* I don't think add makes sense in this context
* Consider renaming to RecordVoidsongInput to match the comment or,
* VoidsongCall to match the input actions that it handels
* - Liam Suggestion
*/
void APlayerShip::AddVoidsongInput(int Input)
{
    //Add the input to the list of inputs
    VoidsongCombo.Emplace(Input);

    //Call the event dispatcher
    OnVoidsongInputDelegate.Broadcast(FIntArray(VoidsongCombo));

    //Reset the reset voidsong timer
    ResetVoidsongTimer = 0;
    //Unnecessary Variable - Liam Suggestion
    ShouldResetVoidsongTimerTick = true;

    //Play the voidsong instrument
    if (IsValid(VoidsongInstrument))
    {
        VoidsongInstrument->GetParameterInterface()->Trigger(FName(FString::FromInt(Input)));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("The Voidsong instrument isn't valid! Set it on the player ship."));
    }
}

/*
* Function comments from the .h should be copied to the .cpp
*
* Confusing Name. It is unclear that this is an input action callback
* - Liam Suggestion
*/
void APlayerShip::ActivateVoidsong()
{
    //Calls play voidsong with the player's current sequence of inputs
    PlaySequence(VoidsongCombo);
    ResetVoidsong();
}

/*
* Function comments from the .h should be copied to the .cpp
* 
* Imprecise name. Consider renaming to ClearVoidsongInput() or similar
* - Liam Suggestion
*/
void APlayerShip::ResetVoidsong()
{
    //Empties the sequence of inputs
    VoidsongCombo.Empty();
    //Unnecessary Variable - Liam Suggestion
    ShouldResetVoidsongTimerTick = false;

    OnResetVoidsongDelegate.Broadcast();
}

//Comment -Mabel Suggestion
void APlayerShip::InvertMoveDirection()
{
    //I was looking for what InvertMoveDirection would do, and now I have to look for what bMovementInputInvert does.  Kind of annoying tbh. -Mabel Suggestion
    bMovementInputInvert = !bMovementInputInvert;
}




