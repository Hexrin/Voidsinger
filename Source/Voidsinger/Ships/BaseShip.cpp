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
	MeshComponent->bUseComplexAsSimpleCollision = false;

	PhysicsComponent = CreateDefaultSubobject<UShipPhysicsComponent>(TEXT("Physics Component"));
	PartGrid = CreateDefaultSubobject<UPartGridComponent>(TEXT("Part Grid"));
	MovementComponent = CreateDefaultSubobject<UShipMovementComponent>(TEXT("Movement Component"));	
	
	MeshData = TMap<FIntPoint, FPartMeshData>();
	RelativeMeshLocation = FVector2D();
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
	UBaseVoidsong* NewVoidsong = NewObject<UBaseVoidsong>(this, Voidsong);
	AvailableVoidsongs.Emplace(NewVoidsong);
	OnAddVoidsongDelegate.Broadcast(NewVoidsong);
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

void ABaseShip::AddMeshAtLocation(FIntPoint Location)
{
	//MeshComponent->CreateMeshSection()
	if (MeshData.Contains(Location))
	{
		RemoveMeshAtLocation(Location);
	}

	
}


void ABaseShip::RemoveMeshAtLocation(FIntPoint Location)
{
	
}

void ABaseShip::SetMeshRelativeLocation(FVector2D Location)
{
	RelativeMeshLocation = Location;
	UpdateMesh(false);
}

void ABaseShip::UpdateMesh(bool MeshChanged)
{
	//DrawDebugPoint(GetWorld(), FVector(RelativeMeshLocation, 0), 10, FColor::Purple, true);
	//TArray<FVector> AdjVertices = TArray<FVector>();
	//AdjVertices.SetNum(Vertices.Num());

	//TArray<FVector2D> AdjUV = TArray<FVector2D>();
	//AdjUV.SetNum(Vertices.Num());

	//for (int i = 0; i < Vertices.Num(); i++)
	//{
	//	AdjVertices[i] = Vertices[i] + FVector(RelativeMeshLocation, 0);
	//	AdjUV[i] = UVs[0][i] - RelativeMeshLocation;
	//}

	//if (MeshChanged)
	//{
	//	MeshComponent->CreateMeshSection(0, AdjVertices, Triangles, TArray<FVector>(), AdjUV, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	//}
	//else
	//{
	//	MeshComponent->UpdateMeshSection(0, AdjVertices, TArray<FVector>(), AdjUV, TArray<FColor>(), TArray<FProcMeshTangent>());
	//}

	//MeshComponent->ClearCollisionConvexMeshes();
	//for (auto& Val : CollisionMeshes)
	//{
	//	TArray<FVector> AdjCollision = TArray<FVector>();
	//	//AdjCollision.SetNum(Val.Value.Num());
	//	for (FVector Vertex : Val.Value)
	//	{
	//		AdjCollision.Emplace(Vertex + FVector(RelativeMeshLocation, 0));
	//	}

	//	MeshComponent->AddCollisionConvexMesh(AdjCollision);
	//}
}

TSet<FVector> ABaseShip::GetVerticesAroundLocation(FVector2D Location)
{
	TSet<FVector> ReturnValue = TSet<FVector>();
	for (int i = 0; i < 4; i++)
	{
		float AdjustmentValue1 = PartGrid->GetPartGridScale() / (i < 2 ? 2 : -2);
		float AdjustmentValue2 = PartGrid->GetPartGridScale() / ((i % 2 == 0) ? 2 : -2);
		ReturnValue.Emplace(FVector(Location, 0) + FVector(AdjustmentValue1, AdjustmentValue2, 0));
	}
	
	return ReturnValue;
}

void ABaseShip::AddTrianglesForSquare(int32 UpperRight, int32 UpperLeft, int32 LowerRight, int32 LowerLeft, FIntPoint Location)
{
	TriangleIndices.Emplace(Location, Triangles.Emplace(UpperRight));
	Triangles.Emplace(UpperLeft);
	Triangles.Emplace(LowerLeft);

	Triangles.Emplace(LowerLeft);
	Triangles.Emplace(LowerRight);
	Triangles.Emplace(UpperRight);
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



