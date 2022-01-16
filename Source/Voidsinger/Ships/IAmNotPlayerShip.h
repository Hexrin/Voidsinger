// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "BaseShip.h"
//#include "Voidsinger/VoidsingerTypes.h"
//#include "Voidsinger/Voidsongs/BaseVoidsong.h"
////#include "Voidsinger/StarSystemData.h"
//#include "Components/AudioComponent.h"
//#include "EngineUtils.h"
//#include "Camera/CameraComponent.h"
//#include "PlayerShip.generated.h"
//
///**
// * 
// */
//
////Struct needs commenting. What does it store(yes i know its obvious) - Liam Suggestion
///*USTRUCT(BlueprintType)
//struct FIntArray
//{
//	GENERATED_BODY()
//
//
//	//UE doesnt like int, use int32 or int64 instead - Liam Suggestion
//	UPROPERTY(BlueprintReadWrite)
//	TArray<int> IntArray = TArray<int>();
//
//	//What does default constuctor do? What are the default values of its properties? - Liam Suggestion
//	FIntArray()
//	{
//	}
//
//	//UE doesnt like int, use int32 or int64 instead - Liam Suggestion
//	FIntArray(TArray<int> NewInts)
//	{
//		IntArray = NewInts;
//	}
//};*/
//
////Delagates should be commented. What are they supposed to be bound to? - Liam Suggestion
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoidsongInputDelegate, FIntArray, CurrentInput);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetVoidsongDelegate);
//
//UCLASS()
//class VOIDSINGER_API APlayerShip : public ABaseShip
//{
//	GENERATED_BODY()
//	
//
//
//public:
//
//	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
//	//             FUNCTIONS             ||
//	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//
//
//	/*Initializer Functions*\
//	\*---------------------*/
//
//	//Constructor
//	APlayerShip();
//	
//	//Tick
//	virtual void Tick(float DeltaTime) override;
//
//	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
//	//Begin play
//	virtual void BeginPlay() override;
//
//	/*Getter Functions*\
//	\*----------------*/
//
//	//Gets the travel cost of going to the given star system
//	//Why is this on the player? Shouldn't it be on whatever is storing the start systems? Like the galaxay map or something? -Mabel Suggestion
//	//UFUNCTION(BlueprintPure)
//	//TMap<EResourceType, float> GetTravelCost(class UStarSystemData* Target);
//
//	//Gets the star system the player is currently in
//	//This maybe should be on the galaxy map as well. Idk -Mabel Suggestion
//	//UFUNCTION(BlueprintPure)
//	//class UStarSystemData* GetCurrentStarSystem();
//
//	/*Traveling Functions*\
//	\*-------------------*/
//
//	//Travels to the start system given
//	//Should this also be on the galaxy map -Mabel Suggestion
//	//UFUNCTION(BlueprintCallable)
//	//bool TravelToStarSystem(class UStarSystemData* Target);
//
//	/*Voidsong input actions*\
//	\*----------------------*/
//
//	//We could come up with a standardized way to name things bound to input actions/axies - Liam Healey
//	//Input actions
//	UFUNCTION()
//	void Voidsong1Call();
//
//	UFUNCTION()
//	void Voidsong2Call();
//
//	UFUNCTION()
//	void Voidsong3Call();
//
//	UFUNCTION()
//	void Voidsong4Call();
//
//	UFUNCTION()
//	void Voidsong5Call();
//
//
//	/*Voidsong Management*\
//	\*-------------------*/
//
//	/*
//	* UE doesnt like int, use int32 or int64 instead.
//	* 
//	* Confusing Name
//	* I don't think add makes sense in this context
//	* Consider renaming to RecordVoidsongInput to match the comment or,
//	* VoidsongCall to match the input actions that it handels
//	* - Liam Suggestion
//	*/ 
//	//Records the incoming voidsong input
//	UFUNCTION()
//	void AddVoidsongInput(int Input);
//
//	//Confusing Name. It is unclear that this is an input action callback - Liam Suggestion
//	//Activates the voidsong
//	void ActivateVoidsong();
//
//	//Imprecise name. Consider renaming to ClearVoidsongInput() or similar - Liam Suggestion
//	//Clears the sequence of inputs that the player has made
//	UFUNCTION(BlueprintCallable)
//	void ResetVoidsong();
//
//
//	/*-----Movement-----*\
//	\*------------------*/
//public:
//
//	//Comment -Mabel Suggestion
//	//Function name feels odd, what about ReverseMoveDirection? -Mabel Suggestion
//	UFUNCTION()
//	void InvertMoveDirection();
//
//protected:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float PlayerMaxSpeed{ 50 };
//
//private:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	bool bMovementInputInvert{ false };
//
//	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
//	//             VARIABLES             ||
//	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//
//
//
//	/*Voidsong Timers*\
//	\*---------------*/
//protected:
//	/*
//	* Should have a metadata min value clamp at 0 
//	* if ResetVoidsong() is renamed to ClearVoidsongInput() this function should be renamed to match as well.
//	* - Liam Suggestion
//	*/
//	//The time the player has to wait without playing to reset the Voidsong combo
//	UPROPERTY(EditDefaultsOnly)
//	float VoidsongResetDelay = 3.0;
//
//	/*
//	* Comment unclear.
//	* Reader needs to know what this variable is storing not how/where it is used.
//	* 
//	* If ResetVoidsong() is renamed to ClearVoidsongInput() this function should be renamed to match as well.
//	* - Liam Suggestion
//	*/
//	//The timer for counting to VoidsongResetDelay
//	UPROPERTY()
//	float ResetVoidsongTimer = 0;
//
//	/*
//	* Poor name.
//	* bool variables should always start with b.
//	* Name very long consider shortening.
//	* 
//	* Comment unclear.
//	* Reader needs to know what this variable is storing not how/where it is used.
//	* 
//	* Variable unecessary.
//	* Sould be replaced with either VoidsongCombo.Num() != 0 or,
//	* ResetVoidsongTimer < VoidsongResetDelay
//	*/
//	//Boolean for the Voidsong Timer
//	UPROPERTY()
//	bool ShouldResetVoidsongTimerTick = false;
//
//	/*Voidsong Management*\
//	\*-------------------*/
//
//	/*
//	* UE doesnt like int, use int32 or int64 instead.
//	* 
//	* Condusing name.
//	* Combo doesn't make sence in this context as it implies it is the key to a voidsong as opposed to what voidsongs have already been played.
//	* - Liam Suggestion
//	*/ 
//	//Array of the inputs that the player has put in to play voidsongs
//	UPROPERTY()
//	TArray<int> VoidsongCombo;
//
//	/*
//	* Confusing Comment
//	* It is instance not spawned and seting paraneters is implied and does not need to be stated.
//	* What is this used for and what sounds does it handel. Is it for all voidsongs or just when the player presses a Voidsong key. 
//	* - Liam Suggestion
//	*/ 
//	//The spawned metasound to set parameters on
//	UPROPERTY(EditAnywhere)
//	UAudioComponent* VoidsongInstrument;
//
//	//Is this the thing that plays when you press a voidsong key?
//	//The voidsong instrument reference. Is set through blueprint.
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
//	TSoftObjectPtr<USoundBase> VoidsongInstrumentAsset;
//
//	/*------Camera-------*\
//	\*-------------------*/
//public:
//
//	//Should the camera be a component? -Mabel Suggestion
//	
//	//Comment -Mabel Suggestion
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	class UCameraComponent* Camera;
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
//	class USceneComponent* CameraRoot;
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void AddCameraLocation(FVector2D DeltaLoc);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void SetCameraLocation(FVector2D NewLoc);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void SetCameraZoom(float Percent);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	const float GetCameraZoom();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION()
//	void ZoomAxisCall(float AxisValue);
//
//protected:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float CameraMinHeight{ 100 };
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float CameraMaxHeight{ 1000 };
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
//	float CameraVelocityAdjScaling{ 1 };
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
//	float CameraZoomAxisScaling{ 0.01 };
//
//private:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(EditAnywhere)
//	float CameraHeight;
//
//	/*-----Building------*\
//	\*-------------------*/
//public:
//
//	//Comment -Mabel Suggestion
//	void ToggleBuildUICall();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintCallable)
//	void SetBuildMode(bool NewBuildMode);
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintImplementableEvent)
//	void OpenBuildMenu();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintImplementableEvent)
//	void CloseBuildMenu();
//
//	//Comment -Mabel Suggestion
//	UFUNCTION(BlueprintPure)
//	bool IsInBuildMode();
//
//private:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY()
//	bool bBuildMode{ false };
//
//
//
//	/*-------Misc.-------*\
//	\*-------------------*/
//
//protected:
//
//	//Comment -Mabel Suggestion
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	int Pixels;
//
//
//	/*
//	* Delagate needs commenting. When is it called?
//	* 
//	* Weird name?
//	* Input kida doesn make sense
//	* Consider renaming to OnVoidsongInputRecivedDelagate?
//	* - Liam Suggestion
//	*/ 
//	UPROPERTY(BlueprintAssignable)
//	FVoidsongInputDelegate OnVoidsongInputDelegate;
//
//	/*
//	* Delagate needs commenting.
//	* When is it called?
//	* 
//	* Name Inconsistancy with previous function
//	* Should be named OnVoidsongResetDelegate
//	* - Liam Suggestion
//	*/
//
//	UPROPERTY(BlueprintAssignable)
//	FResetVoidsongDelegate OnResetVoidsongDelegate;
//	
//	//Comment -Mabel Suggestion
//	//UPROPERTY(VisibleAnywhere)
//	//class UStarSystemData* CurrentStarSystem;
//};
