// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShip.h"
#include "Voidsinger/Parts/BaseResourceSystem.h"
#include "Voidsinger/Parts/BaseFreespacePart.h"

// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;

	PhysicsComponent = CreateDefaultSubobject<UShipPhysicsComponent>(TEXT("Physics Component"));
	PartGrid = CreateDefaultSubobject<UPartGridComponent>(TEXT("Part Grid"));
	MovementComponent = CreateDefaultSubobject<UShipMovementComponent>(TEXT("Movement Component"));	
	

	UVs = TArray<TArray<FVector2D>>();
	for (int i = 0; i < 4; i++)
		UVs.Emplace(TArray<FVector2D>());
	
	Vertices = TArray<FVector>();
	Triangles = TArray<int32>();
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

	if (bDecelerating && PhysicsComponent->GetVelocity().SizeSquared() > PhysicsComponent->MinLinearVelocity)
	{
		SetTargetMoveDirection(-1 * PhysicsComponent->GetVelocity().GetRotated(-1 * GetActorRotation().Yaw));
		//UE_LOG(LogTemp, Warning, TEXT("Decellerate"));
	}

	

	FVector FutureForawardVector = FQuat(FVector(0,0,1), PhysicsComponent->GetAngularVelocity() * MovementComponent->GetDecelerationPredictionTime()).RotateVector(GetActorForwardVector());
	
	if (TargetLookDirection.SizeSquared2D() != 0 && !TargetLookDirection.Equals(FutureForawardVector, MovementComponent->GetLookDirectionTollerance()))
	{
		float RotationDirection = FVector::CrossProduct(TargetLookDirection, FutureForawardVector).Z;
		//UE_LOG(LogTemp, Warning, TEXT("SineThing = %f, TargetLookDirection = %s"), RotationDirection, *TargetLookDirection.ToString());
		MovementComponent->RotateShip(RotationDirection < 0, FMath::Abs(RotationDirection));
	}

	if (TargetMoveDirection.SizeSquared() != 0)
	{
		MovementComponent->Move(TargetMoveDirection, 1);
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

void ABaseShip::SetTargetMoveDirection(FVector2D Vector)
{
	TargetMoveDirection = Vector.GetSafeNormal();
}

FVector2D ABaseShip::GetTargetMoveDirection()
{
	return TargetMoveDirection;
}

void ABaseShip::AddMeshAtLocation(FIntPoint Location)
{
	TArray<int32> Indices = TArray<int32>();
	for (FVector Vetex : GetVerticesAroundLocation(FVector2D(Location)))
	{
		Indices.Emplace(Vertices.AddUnique(Vetex));
	}

	AddTriangles(Indices[2], Indices[1], Indices[0]);
	AddTriangles(Indices[2], Indices[3], Indices[1]);

	UVs[0].SetNum(Vertices.Num());
	UVs[1].SetNum(Vertices.Num());
	UVs[2].SetNum(Vertices.Num());
	UVs[3].SetNum(Vertices.Num());

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
		//	FPartData PartData = PartGrid->GetPartGrid().FindRef(Location);
		FVector2D UVToAdd = FVector2D(j<2?1:0, j%2==0?1:0);
		//	if (/*IsValid(Cast<UBaseFreespacePart>(PartData.Part))*/ true)
		//	{
		//		if (i == (Location.X + Location.Y) % 2)
		//		{
		//			UVToAdd = FVector2D((PartData.BitNumber + j % 2)/256, j < 2 ? 0 : 1);
		//		}
		//		else
		//		{
		//			UVToAdd = FVector2D(-1);
		//		}
		//	}
		//	else
		//	{
		//		if (i < 2)
		//		{
		//			UVToAdd = FVector2D(-1);
		//		}
		//		else
		//		{
		//			UVToAdd = FVector2D(Location - PartData.Part->GetPartGridLocation() - PartData.Part->GetPartBounds().LowerBounds);
		//		}
		//	}
			if (i == (abs(Location.X) % 2) + 2*(abs(Location.Y) % 2))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s UV%i #%i = %s"), *Location.ToString(), i, Indices[j], *UVToAdd.ToString());

				UVs[i][Indices[j]] = UVToAdd;
			}
		}
	}

	MeshComponent->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVs[0], UVs[1], UVs[2], UVs[3], TArray<FColor>(), TArray<FProcMeshTangent>(), false);
}

void ABaseShip::RemoveMeshAtLocation(FIntPoint Location)
{
}

void ABaseShip::SetMeshRelativeLocation(FVector2D Location)
{
}

TSet<FVector> ABaseShip::GetVerticesAroundLocation(FVector2D Location)
{
	TSet<FVector> ReturnValue = TSet<FVector>();
	for (int i = 0; i < 4; i++)
	{
		float AdjustmentValue1 = PartGrid->GetPartGridScale() / (i < 2 ? 2 : -2);
		float AdjustmentValue2 = PartGrid->GetPartGridScale() / ((i % 2 == 1) ? 2 : -2);
		ReturnValue.Emplace(FVector(Location, 0) + FVector(AdjustmentValue1, AdjustmentValue2, 0));
	}
	
	return ReturnValue;
}

void ABaseShip::AddTriangles(int32 A, int32 B, int32 C)
{
	Triangles.Emplace(A);
	Triangles.Emplace(B);
	Triangles.Emplace(C);
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



