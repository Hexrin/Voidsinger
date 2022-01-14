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

	//PhysicsComponent = CreateDefaultSubobject<UShipPhysicsComponent>(TEXT("Physics Component"));
	//PartGrid = CreateDefaultSubobject<UPartGridComponent>(TEXT("Part Grid"));
	//MovementComponent = CreateDefaultSubobject<UShipMovementComponent>(TEXT("Movement Component"));	
	
	MeshData = TMap<FIntPoint, int32>();

	//This seems weird -Mabel Suggestion
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
	
	//Debug... OH NO! -Mabel Suggestion
	//UE_LOG(LogTemp, Warning, TEXT("Default parts num %i"), DefaultParts.Num())

	//PartGrid->BuildShip(DefaultParts);
}

// Called every frame
void ABaseShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Handle Target Rotation
	//Make a functions called HandleTargetRotation() and call that on tick. Having a function is better because "tick" is not
	//descriptive of what is being done on tick. -Mabel Suggestion
	if (TargetLookDirection.SizeSquared2D() != 0)
	{
		//float AngVel = PhysicsComponent->GetAngularVelocity();

		//Why are booleans named "Direction"? Directions are typically associated with vectors. I don't understand what 
		//saying "DecelDirection" is true would actually mean based on the english. Name better, like bIsClockwise or something.
		//bDecelDirection? -Mabel Suggestion
		//bool DecelDirection = AngVel < 0;
		//bTargetRotationDirection? -MabelSuggestion
		bool TargetRotationDirection = FVector::CrossProduct(TargetLookDirection, GetActorForwardVector()).Z < 0;

		float TargetRotationDistance = abs(FMath::Acos(FVector::DotProduct(TargetLookDirection, GetActorForwardVector())));

		//float MaxDecelerationSpeed = abs(MovementComponent->GetMaximumAccelerationInRotation(DecelDirection));
		//float MaxTurnSpeed = abs(MovementComponent->GetMaximumAccelerationInRotation(TargetRotationDirection));

		//float TimeToDecelerate = abs(AngVel / MaxDecelerationSpeed);
		//float TimeToDestination = TargetRotationDistance / abs(AngVel);

		//Comment your switch, also just use an if statement instead. You only have 2 possible cases, 0 or 1. Perfect
		//for an if statement. Here, you use an if statement with a switch, so... you only need the if. -Mabel Suggestion
		//switch ((TargetRotationDistance > MovementComponent->GetLookDirectionErrorTollerance()) ? 1 : 0)
		//{
		//case 1:
		//	if (abs(TimeToDecelerate - TimeToDestination) > MovementComponent->GetRotationDirectionUpdateThreshold())
		//	{
		//		bCurrentRotationDeccelerationStatus = (TimeToDecelerate > TimeToDestination);
		//	}

		//	if (!bCurrentRotationDeccelerationStatus)
		//	{
		//		MovementComponent->RotateShip(TargetRotationDirection, 1);
		//		//UE_LOG(LogTemp, Warning, TEXT("Turn"));
		//		break;
		//	}

		//default:
		//	MovementComponent->RotateShip(DecelDirection, TimeToDecelerate / TimeToDestination);
		//	//UE_LOG(LogTemp, Warning, TEXT("Decel"));
		//	break;
		//}
	}

	//Handle Target Move Direction
	//Same as above -Mabel Suggestion
	if (TargetMoveDirection.SizeSquared() != 0)
	{
		//While this is a clever way of commenting, I think it would be more readable if you just made separate variables.
		// TargetVelocity = TargetMoveDirection * TargetMoveSpeed
		// CurrentRelativeVelocity = blahblah
		// FVector2D DeltaV = TargetVelocity - CurrentRelativeVelocity
		// -Mabel Suggestion
		//				   |----------Target Velocity----------|   |----------------------Current Relative Velocity----------------------|
		//FVector2D DeltaV = TargetMoveDirection * TargetMoveSpeed - PhysicsComponent->GetVelocity().GetRotated(GetActorRotation().Yaw * -1);
		//MovementComponent->Move(DeltaV, DeltaV.SizeSquared() - FMath::Square(MovementComponent->GetMoveSpeedErrorTollerance()));
	}	
}

// Called to bind functionality to input
void ABaseShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Did this function need to be overriden if there is nothing happening inside of it? -Mabel Suggestion
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


/*
* Function comments from the .h should be copied to the .cpp
* 
* **Function needs implementation**
* 
* Outdated name.
* Should be GetResourceCapacity(EResourceType ResourceType)
* Comment should be updated to match.
*
* - Liam Suggestion
*/
float const ABaseShip::GetFluidCapacity(EResourceType Fluid)
{
	return 100.0f;
}


/*
* Function comments from the .h should be copied to the .cpp
* 
* **Function needs implementation**
* 
* Outdated name.
* Should be GetFluidAmount(EResourceType ResourceType)
* Comment should be updated to match.
*
* - Liam Suggestion
*/
float const ABaseShip::GetFluidAmount(EResourceType Fluid)
{
	return 100.0f;
}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
//void ABaseShip::AddResourceSystem(UBaseResourceSystem* System)
//{
	//ResourceSystems.Add(System);
//}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
//void ABaseShip::RemoveResourceSystem(UBaseResourceSystem* System)
//{
	//if (ResourceSystems.Contains(System))
	//{
		//ResourceSystems.Remove(System);
	//}
//}

//Function comments from the .h should be copied to the .cpp - Liam Suggestion
EFactions ABaseShip::GetFaction()
{
	return Faction;
}

//Function comments from the.h should be copied to the.cpp - Liam Suggestion
//TArray<UBaseResourceSystem*> ABaseShip::GetResourceSystems()
//{
//	return ResourceSystems;
//}

//Comment -Mabel Suggestion
void ABaseShip::SetTargetMoveDirection(FVector2D Vector)
{
	TargetMoveDirection = Vector.GetSafeNormal();
}

//Comment -Mabel Suggestion
FVector2D ABaseShip::GetTargetMoveDirection()
{
	return TargetMoveDirection;
}

//Comment -Mabel Suggestion
void ABaseShip::SetTargetMoveSpeed(float Vector)
{
	TargetMoveSpeed = abs(Vector);
}

//Comment -Mabel Suggestion
float ABaseShip::GetTargetMoveSpeed()
{
	return TargetMoveSpeed;
}

//Comment -Mabel Suggestion
void ABaseShip::SetTargetLookDirection(FVector Vector)
{
	TargetLookDirection = Vector.GetSafeNormal2D();
}

//Comment -Mabel Suggestion
const FVector ABaseShip::GetTargetLookDirection()
{
	return TargetLookDirection;
}


//Reading this over, I feel like the mesh handling could actually for reals be a separate component. The only questionable ones
//are the GetVerticesAroundLocation and the SetMeshRelativeLocation because the use functions on the base ship. I don't think
//SetMeshRelativeLocation should change the location of the ship anyway though, but you would probably have to rework
//the GetVerticesAroundLocation.
//Comment -Mabel Suggestion
void ABaseShip::AddMeshAtLocation(FIntPoint Location)
{
	

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

	//Magic numbers >:( at least comment them -Mabel Suggestion
	TArray<int32> Triangles = CreateTrianglesForSquare(0,2,4,6);
	//Triangles += CreateTrianglesForSquare(3, 1, 7, 5);
	Triangles += CreateTrianglesForSquare(2, 0, 3, 1);
	Triangles += CreateTrianglesForSquare(0, 4, 1, 5);
	Triangles += CreateTrianglesForSquare(4, 6, 5, 7);
	Triangles += CreateTrianglesForSquare(6, 2, 7, 3);

	MeshComponent->CreateMeshSection(SectionIndex, GetVerticesAroundLocation(Location), Triangles, TArray<FVector>(), UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

	MeshData.Emplace(Location, SectionIndex);
}

//Comment -Mabel Suggestion
void ABaseShip::RemoveMeshAtLocation(FIntPoint Location)
{
	MeshComponent->ClearMeshSection(MeshData.FindRef(Location));
}

//Comment -Mabel Suggestion
void ABaseShip::SetMeshRelativeLocation(FVector2D Location)
{
	MeshComponent->SetRelativeLocation(FVector(Location, 0));

	//Why does setting a meshes location move the entire ship? That seems a bit odd. -Mabel
	//AddActorWorldOffset(FVector(-1 * Location - PhysicsComponent->GetCenterOfMass(), 0));
}

//Comment -Mabel Suggestion
void ABaseShip::SetMeshMaterialAtLocation(FIntPoint Location, UMaterialInterface* Material)
{
	MeshComponent->SetMaterial(MeshData.FindRef(Location), Material);
}

//Comment -Mabel Suggestion
TArray<FVector> ABaseShip::GetVerticesAroundLocation(FVector2D Location)
{
	TArray<FVector> ReturnValue = TArray<FVector>();
	for (int i = 0; i < 4; i++)
	{
		//float AdjustmentValue1 = PartGrid->GetPartGridScale() / (i < 2 ? 2 : -2);
		//float AdjustmentValue2 = PartGrid->GetPartGridScale() / ((i % 2 == 0) ? 2 : -2);
		//ReturnValue.Emplace(FVector(Location, 0) + FVector(AdjustmentValue1, AdjustmentValue2, 0.1));
		//ReturnValue.Emplace(FVector(Location, 0) + FVector(AdjustmentValue1, AdjustmentValue2, -0.1));
	}
	
	return ReturnValue;
}

//Comment -Mabel Suggestion
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

//Function comments from the.h should be copied to the.cpp - Liam Suggestion
void ABaseShip::SaveEditorShip()
{
	//Bad name. It implies that it is storing a return value or a pass by refernce - Liam Suggestion
//	TArray<FPartData> OutArray = PartGrid->GetPartGrid().GetValueArray();;
//
////Comment -Mabel Suggestion
//void ABaseShip::SetTargetMoveDirection(FVector2D Vector)
//{
//	TargetMoveDirection = Vector.GetSafeNormal();
//}
//
////Comment -Mabel Suggestion
//FVector2D ABaseShip::GetTargetMoveDirection()
//{
//	return TargetMoveDirection;
//}
//
////Comment -Mabel Suggestion
//void ABaseShip::SetTargetMoveSpeed(float Vector)
//{
//	TargetMoveSpeed = abs(Vector);
//}
//
////Comment -Mabel Suggestion
//float ABaseShip::GetTargetMoveSpeed()
//{
//	return TargetMoveSpeed;
//}
//
////Comment -Mabel Suggestion
//void ABaseShip::SetTargetLookDirection(FVector Vector)
//{
//	TargetLookDirection = Vector.GetSafeNormal2D();
//}
//
////Comment -Mabel Suggestion
//const FVector ABaseShip::GetTargetLookDirection()
//{
//	return TargetLookDirection;
//}
//
//
////Reading this over, I feel like the mesh handling could actually for reals be a separate component. The only questionable ones
////are the GetVerticesAroundLocation and the SetMeshRelativeLocation because the use functions on the base ship. I don't think
////SetMeshRelativeLocation should change the location of the ship anyway though, but you would probably have to rework
////the GetVerticesAroundLocation.
////Comment -Mabel Suggestion
//void ABaseShip::AddMeshAtLocation(FIntPoint Location)
//{
//	
//
//	int32 SectionIndex = 0; 
//	
//	if (MeshData.Contains(Location))
//	{
//		SectionIndex = MeshData.FindRef(Location);
//		RemoveMeshAtLocation(Location);
//	}
//	else
//	{
//		TArray<FIntPoint> Keys = TArray<FIntPoint>();
//		MeshData.GenerateKeyArray(Keys);
//		SectionIndex = Keys.Num();
//	}
//
//	//Magic numbers >:( at least comment them -Mabel Suggestion
//	TArray<int32> Triangles = CreateTrianglesForSquare(0,2,4,6);
//	Triangles += CreateTrianglesForSquare(2, 0, 3, 1);
//	Triangles += CreateTrianglesForSquare(0, 4, 1, 5);
//	Triangles += CreateTrianglesForSquare(4, 6, 5, 7);
//	Triangles += CreateTrianglesForSquare(6, 2, 7, 3);
//
//	MeshComponent->CreateMeshSection(SectionIndex, GetVerticesAroundLocation(Location), Triangles, TArray<FVector>(), UV, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
//	MeshComponent->SetGenerateOverlapEvents(false);
//	MeshComponent->SetGenerateOverlapEvents(true);
//	MeshData.Emplace(Location, SectionIndex);
//}
//
////Comment -Mabel Suggestion
//void ABaseShip::RemoveMeshAtLocation(FIntPoint Location)
//{
//	MeshComponent->ClearMeshSection(MeshData.FindRef(Location));
//}
//
////Comment -Mabel Suggestion
//void ABaseShip::SetMeshRelativeLocation(FVector2D Location)
//{
//	MeshComponent->SetRelativeLocation(FVector(Location, 0));
//
//	//Why does setting a meshes location move the entire ship? That seems a bit odd. -Mabel
//	AddActorWorldOffset(FVector(-1 * Location - PhysicsComponent->GetCenterOfMass(), 0));
//}
//
////Comment -Mabel Suggestion
//void ABaseShip::SetMeshMaterialAtLocation(FIntPoint Location, UMaterialInterface* Material)
//{
//	MeshComponent->SetMaterial(MeshData.FindRef(Location), Material);
//}
//
////Comment -Mabel Suggestion
//TArray<FVector> ABaseShip::GetVerticesAroundLocation(FVector2D Location)
//{
//	TArray<FVector> ReturnValue = TArray<FVector>();
//	for (int i = 0; i < 4; i++)
//	{
//		float AdjustmentValue1 = PartGrid->GetPartGridScale() / (i < 2 ? 2 : -2);
//		float AdjustmentValue2 = PartGrid->GetPartGridScale() / ((i % 2 == 0) ? 2 : -2);
//		ReturnValue.Emplace(FVector(Location, 0) + FVector(AdjustmentValue1, AdjustmentValue2, 0.1));
//		ReturnValue.Emplace(FVector(Location, 0) + FVector(AdjustmentValue1, AdjustmentValue2, -0.1));
//	}
//	
//	return ReturnValue;
//}
//
////Comment -Mabel Suggestion
//TArray<int32> ABaseShip::CreateTrianglesForSquare(int32 UpperRight, int32 UpperLeft, int32 LowerRight, int32 LowerLeft)
//{
//	TArray<int32> Triangles = TArray<int32>();
//	Triangles.Emplace(UpperRight);
//	Triangles.Emplace(UpperLeft);
//	Triangles.Emplace(LowerLeft);
//
//	Triangles.Emplace(LowerLeft);
//	Triangles.Emplace(LowerRight);
//	Triangles.Emplace(UpperRight);
//	return Triangles;
//}
//
////Function comments from the.h should be copied to the.cpp - Liam Suggestion
//void ABaseShip::SaveEditorShip()
//{
//	//Bad name. It implies that it is storing a return value or a pass by refernce - Liam Suggestion
//	TArray<FPartInstanceData> OutArray = PartGrid->GetPartGrid().GetValueArray();;
//	
//	TArray<FSavePartInfo> InfoToSave;
//	//Part set unnecessary. You can use AddUnquie() instead. - Liam Suggestion
//	TSet<UPart*> PartSet;
//	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
//	for (auto& i : OutArray)
//	{
//		if (!PartSet.Contains(i.Part))
//		{
//			InfoToSave.Emplace(FSavePartInfo(i.Part->GetClass(), i.Part->GetPartGridLocation(), i.Part->GetRelativeRotation()));
//			PartSet.Emplace(i.Part);
//		}
//	}
//	//ClassCurrentlyEditing should be a parameter of SaveEditorShips(). - Liam Suggestion
//	GetMutableDefault<ABaseShip>(ClassCurrentlyEditing)->DefaultParts.Empty();
//	GetMutableDefault<ABaseShip>(ClassCurrentlyEditing)->DefaultParts = InfoToSave;
}



