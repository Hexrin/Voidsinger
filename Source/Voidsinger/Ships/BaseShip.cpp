// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShip.h"
#include "Voidsinger/Parts/BaseResourceSystem.h"

// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	PhysicsComponent = CreateDefaultSubobject<UShipPhysicsComponent>(TEXT("Physics Component"));
	PartGrid = CreateDefaultSubobject<UPartGridComponent>(TEXT("Part Grid"));
	MovementComponent = CreateDefaultSubobject<UShipMovementComponent>(TEXT("Movement Component"));	
	
}

// Called when the game starts or when spawned
void ABaseShip::BeginPlay()
{
	Super::BeginPlay();
	
	PartGrid->BuildShip(DefaultParts);

}

// Called every frame
void ABaseShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float const ABaseShip::GetFluidCapacity(TEnumAsByte<EResourceType> Fluid)
{
	return 100.0f;
}

float const ABaseShip::GetFluidAmount(TEnumAsByte<EResourceType> Fluid)
{
	return 100.0f;
}

void ABaseShip::AddResourceSystem(UBaseResourceSystem* System)
{
	ResourceSystems.Add(System);
}

void ABaseShip::RemoveResourceSystem(UBaseResourceSystem* System)
{

	if (ResourceSystems.Contains(System))
	{
		ResourceSystems.Remove(System);

	}
}

void ABaseShip::AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong)
{
	//Creates the voidsong object from the given class and adds it to available voidsongs
	AvailableVoidsongs.Emplace(NewObject<UBaseVoidsong>(this, Voidsong));
}

void ABaseShip::PlayVoidsong(TArray<int> Sequence)
{
	//Check the available voidsongs and see if their activation sequence matches the sequence inputted. If so, activate that voidsong.
	for (auto& i : AvailableVoidsongs)
	{
		if (i->ActivationCombo == Sequence)
		{
			i->Activate();
		}
	}
}

void ABaseShip::LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs)
{
	//Creates voidsong objects with the list of voidsongs given. Adds them to available voidsongs
	for (auto& i : Voidsongs)
	{
		AvailableVoidsongs.Emplace(NewObject<UBaseVoidsong>(this, i));
	}
}

void ABaseShip::CallLaser(float Damage, float Duration)
{
	UE_LOG(LogTemp, Warning, TEXT("Broadcast should be called...?"))
	OnLaserDelegate.Broadcast(Damage, Duration);
}

void ABaseShip::SaveEditorShip()
{

	TArray<FPartData> OutArray;
	PartGrid->GetPartGrid().GenerateValueArray(OutArray);
	TArray<FSavePartInfo> InfoToSave;
	TSet<UBasePart*> PartSet;
	for (auto& i : OutArray)
	{
		if (!PartSet.Contains(i.Part))
		{
			InfoToSave.Emplace(FSavePartInfo(i.Part->GetClass(), i.Part->GetPartGridLocation(), i.Part->GetRotation()));
			PartSet.Emplace(i.Part);
		}
	}
	GetMutableDefault<ABaseShip>(ClassCurrentlyEditing)->DefaultParts.Empty();
	GetMutableDefault<ABaseShip>(ClassCurrentlyEditing)->DefaultParts = InfoToSave;
}



