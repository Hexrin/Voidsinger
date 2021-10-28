// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShip.h"
#include "Voidsinger/Parts/BaseResourceSystem.h"
#include "Voidsinger/Parts/BaseFreespacePart.h"

// Sets default values
ABaseShip::ABaseShip()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeshComponent->bUseComplexAsSimpleCollision = true;
	MeshComponent->bRenderCustomDepth = true;

	PhysicsComponent = CreateDefaultSubobject<UShipPhysicsComponent>(TEXT("Physics Component"));
	PartGrid = CreateDefaultSubobject<UPartGridComponent>(TEXT("Part Grid"));
	MovementComponent = CreateDefaultSubobject<UShipMovementComponent>(TEXT("Movement Component"));	
	
	MeshData = TMap<FIntPoint, int32>();

	if (UV.Num() == 0)
	{
		UV = TArray<FVector2D>();
		UV.Emplace(FVector2D(1, 1));
		UV.Emplace(FVector2D(1, 1));
		UV.Emplace(FVector2D(1, 0));
		UV.Emplace(FVector2D(1, 0));
		UV.Emplace(FVector2D(0, 1));
		UV.Emplace(FVector2D(0, 1));
		UV.Emplace(FVector2D(0, 0));
		UV.Emplace(FVector2D(0, 0));
	}
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

	//Handel Target Rotation
	if (TargetLookDirection.SizeSquared2D() != 0)
	{
		float AngVel = PhysicsComponent->GetAngularVelocity();

		bool DecelDirection = AngVel < 0;
		bool TargetRotationDirection = FVector::CrossProduct(TargetLookDirection, GetActorForwardVector()).Z < 0;

		float TargetRotationDistance = abs(FMath::Acos(FVector::DotProduct(TargetLookDirection, GetActorForwardVector())));

		float MaxDecelerationSpeed = abs(MovementComponent->GetMaximumAccelerationInRotation(DecelDirection));
		float MaxTurnSpeed = abs(MovementComponent->GetMaximumAccelerationInRotation(TargetRotationDirection));

		float TimeToDecelerate = abs(AngVel / MaxDecelerationSpeed);
		float TimeToDestination = TargetRotationDistance / abs(AngVel);

		switch ((TargetRotationDistance > MovementComponent->GetLookDirectionErrorTollerance()) ? 1 : 0)
		{
		case 1:
			if (abs(TimeToDecelerate - TimeToDestination) > MovementComponent->GetRotationDirectionUpdateThreshold())
			{
				bCurrentRotationDeccelerationStatus = (TimeToDecelerate > TimeToDestination);
			}

			if (!bCurrentRotationDeccelerationStatus)
			{
				MovementComponent->RotateShip(TargetRotationDirection, 1);
				UE_LOG(LogTemp, Warning, TEXT("Turn"));
				break;
			}

		default:
			MovementComponent->RotateShip(DecelDirection, TimeToDecelerate / TimeToDestination);
			UE_LOG(LogTemp, Warning, TEXT("Decel"));
			break;
		}
	}

	//Handel Target Move Direction
	if (TargetMoveDirection.SizeSquared() != 0)
	{
		//				   |----------Target Velocity----------|   |----------------------Current Relative Velocity----------------------|
		FVector2D DeltaV = TargetMoveDirection * TargetMoveSpeed - PhysicsComponent->GetVelocity().GetRotated(GetActorRotation().Yaw * -1);
		MovementComponent->Move(DeltaV, DeltaV.SizeSquared() - FMath::Square(MovementComponent->GetMoveSpeedErrorTollerance()));
	}	
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

TEnumAsByte<EFactions> ABaseShip::GetFaction()
{
	return Faction;
}

//Adds a new voidsong
void ABaseShip::AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong)
{
	//Creates the voidsong object from the given class and adds it to available voidsongs
	UBaseVoidsong* NewVoidsong = NewObject<UBaseVoidsong>(this, Voidsong);
	AvailableVoidsongs.Emplace(NewVoidsong);
	OnAddVoidsongDelegate.Broadcast(NewVoidsong);
}

//Plays the voidsong sequence
void ABaseShip::PlaySequence(TArray<int> Sequence)
{
	if (!Sequence.IsEmpty())
	{
		TArray<TEnumAsByte<EFactions>> Factions;
		TArray<TSubclassOf<UObject>> Nouns;
		TArray<TSubclassOf<UBaseVerbVoidsong>> Verbs;

		DecideVoidsongsPlayed(Sequence, Factions, Nouns, Verbs);

		if (!Factions.IsEmpty() || !Nouns.IsEmpty() || !Verbs.IsEmpty())
		{
			Cast<AVoidGameMode>(GetWorld()->GetAuthGameMode())->Broadcast(Factions, Nouns, Verbs);
		}
	}
}

void ABaseShip::DecideVoidsongsPlayed(TArray<int> Sequence, TArray<TEnumAsByte<EFactions>>& Factions, TArray<TSubclassOf<UObject>>& Nouns, TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs)
{
	for (auto& i : AvailableVoidsongs)
	{
		bool SequenceContainsVoidsong = true;
		for (int j = 0; j < i->ActivationCombo.Num(); j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Sequence %i Activation Combo %i Voidsong %s"), Sequence[j], i->ActivationCombo[j], *i->GetVoidsongDisplayText().ToString());
			if (Sequence[j] != i->ActivationCombo[j])
			{
				SequenceContainsVoidsong = false;
			}
		}
		if (SequenceContainsVoidsong)
		{
			if (IsValid(Cast<UBaseWhoVoidsong>(i)))
			{
				Factions.Emplace(Cast<UBaseWhoVoidsong>(i)->Faction);
			}
			else if (IsValid(Cast<UBaseNounVoidsong>(i)))
			{
				Nouns.Emplace(Cast<UBaseNounVoidsong>(i)->Noun);
			}
			else if (IsValid(Cast<UBaseVerbVoidsong>(i)))
			{
				Verbs.Emplace(Cast<UBaseVerbVoidsong>(i)->GetClass());
			}

			TArray<int> RecursiveArray = Sequence;

			for (auto& j : i->ActivationCombo)
			{
				RecursiveArray.RemoveAt(0);
			}

			if (!RecursiveArray.IsEmpty())
			{
				DecideVoidsongsPlayed(RecursiveArray, Factions, Nouns, Verbs);
			}

			break;
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

void ABaseShip::CallLaser(float DamageMultiplier, float DurationMultiplier)
{
	UE_LOG(LogTemp, Warning, TEXT("Broadcast should be called...?"))
	OnLaserDelegate.Broadcast(DamageMultiplier, DurationMultiplier);
}

TArray<UBaseResourceSystem*> ABaseShip::GetResourceSystems()
{
	return ResourceSystems;
}

void ABaseShip::SetTargetMoveDirection(FVector2D Vector)
{
	TargetMoveDirection = Vector.GetSafeNormal();
}

FVector2D ABaseShip::GetTargetMoveDirection()
{
	return TargetMoveDirection;
}

void ABaseShip::SetTargetMoveSpeed(float Vector)
{
	TargetMoveSpeed = abs(Vector);
}

float ABaseShip::GetTargetMoveSpeed()
{
	return TargetMoveSpeed;
}

void ABaseShip::SetTargetLookDirection(FVector Vector)
{
	TargetLookDirection = Vector.GetSafeNormal2D();
}

const FVector ABaseShip::GetTargetLookDirection()
{
	return TargetLookDirection;
}

void ABaseShip::AddMeshAtLocation(FIntPoint Location)
{
	//MeshComponent->CreateMeshSection()
	

	int32 SectionIndex = 0; 
	
	if (MeshData.Contains(Location))
	{
		SectionIndex = MeshData.FindRef(Location);
		RemoveMeshAtLocation(Location);
	}
	else
	{
		TArray<FIntPoint> Keys = TArray<FIntPoint>();
		MeshData.GenerateKeyArray(Keys);
		SectionIndex = Keys.Num();
	}

	TArray<int32> Triangles = CreateTrianglesForSquare(0,2,4,6);
	//Triangles += CreateTrianglesForSquare(3, 1, 7, 5);
	Triangles += CreateTrianglesForSquare(2, 0, 3, 1);
	Triangles += CreateTrianglesForSquare(0, 4, 1, 5);
	Triangles += CreateTrianglesForSquare(4, 6, 5, 7);
	Triangles += CreateTrianglesForSquare(6, 2, 7, 3);

	MeshComponent->CreateMeshSection(SectionIndex, GetVerticesAroundLocation(Location), Triangles, TArray<FVector>(), UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	MeshData.Emplace(Location, SectionIndex);
}

void ABaseShip::RemoveMeshAtLocation(FIntPoint Location)
{
	MeshComponent->ClearMeshSection(MeshData.FindRef(Location));
}

void ABaseShip::SetMeshRelativeLocation(FVector2D Location)
{
	MeshComponent->SetRelativeLocation(FVector(Location, 0));
	AddActorWorldOffset(FVector(-1 * Location - PhysicsComponent->GetCenterOfMass(), 0));
}

void ABaseShip::SetMeshMaterialAtLocation(FIntPoint Location, UMaterialInterface* Material)
{
	MeshComponent->SetMaterial(MeshData.FindRef(Location), Material);
}

TArray<FVector> ABaseShip::GetVerticesAroundLocation(FVector2D Location)
{
	TArray<FVector> ReturnValue = TArray<FVector>();
	for (int i = 0; i < 4; i++)
	{
		float AdjustmentValue1 = PartGrid->GetPartGridScale() / (i < 2 ? 2 : -2);
		float AdjustmentValue2 = PartGrid->GetPartGridScale() / ((i % 2 == 0) ? 2 : -2);
		ReturnValue.Emplace(FVector(Location, 0) + FVector(AdjustmentValue1, AdjustmentValue2, 0.1));
		ReturnValue.Emplace(FVector(Location, 0) + FVector(AdjustmentValue1, AdjustmentValue2, -0.1));
	}
	
	return ReturnValue;
}

TArray<int32> ABaseShip::CreateTrianglesForSquare(int32 UpperRight, int32 UpperLeft, int32 LowerRight, int32 LowerLeft)
{
	TArray<int32> Triangles = TArray<int32>();
	Triangles.Emplace(UpperRight);
	Triangles.Emplace(UpperLeft);
	Triangles.Emplace(LowerLeft);

	Triangles.Emplace(LowerLeft);
	Triangles.Emplace(LowerRight);
	Triangles.Emplace(UpperRight);
	return Triangles;
}

void ABaseShip::SaveEditorShip()
{

	TArray<FPartData> OutArray = PartGrid->GetPartGrid().GetValueArray();;
	
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



