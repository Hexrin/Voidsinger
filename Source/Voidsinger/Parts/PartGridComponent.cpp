//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "PartGridComponent.h"
//#include "Part.h"
//#include "BaseThrusterPart.h"
//#include "CorePart.h"
//#include "BaseFreespacePart.h"
//#include "Voidsinger/ShipPlayerState.h"
//#include "Voidsinger/Ships/BaseShip.h"
//
//// Sets default values for this component's properties
//UPartGridComponent::UPartGridComponent()
//{
//	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
//	// off to improve performance if you don't need them.
//	PrimaryComponentTick.bCanEverTick = true;
//	PrimaryComponentTick.TickInterval = .5;
//
//	//Initialize Variables
//	//Could GridBounds and GridHalfSize be done in the h file? Why is grid half size 250? that shouldn't be hard
//	//coded -Mabel Suggestion
//	GridBounds = FIntBoxBounds(FIntPoint(0, 0), FIntPoint(0, 0));
//	Ship = Cast<ABaseShip>(GetOwner());
//	GridHalfSize = FIntPoint(250);
//	
//	PartGrid = TGridMap<FPartInstanceData>();
//
//	if (!GridScale)
//	{
//		GridScale = 1;
//	}
//	
//	//Stop magic numbering -Mabel Suggestion
//	TimesSinceHeatTick = 0.f;
//	HeatTickRate = 0.5f;
//	HeatPropagationFactor = 0.5f;
//	HeatMeltTransferFactor = 1.f;
//
//	static ConstructorHelpers::FObjectFinder<UStaticMesh> PixelMeshAsset(*PathToPixelMesh);
//	if (PixelMeshAsset.Succeeded())
//	{
//		PixelMesh = PixelMeshAsset.Object;
//	}
//	
//}
//
//// Called when the game starts
//void UPartGridComponent::BeginPlay()
//{
//	//Why is this done in the construction script and begin play? -Mabel Suggestion
//	Ship = Cast<ABaseShip>(GetOwner());
//	Super::BeginPlay();
//	GridBounds = FIntBoxBounds(FIntPoint(0, 0), FIntPoint(0, 0));
//	// ...
//	
//}
//
//// Called every frame
//void UPartGridComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//	// ...
//
//	//this is... lag. Instead of calling distribute heat multiple times whenever delta time is greater than the tick rate, factor delta time into
//	//the distribute heat function. There's got to be a better way to do this. I understand why you want to call distribute heat multiple times (so that the spread is correct) but the
//	//iteration through the entire part grid is very not worth. Figure out a better way. -Mabel Suggestion
//	for (int i = 0; i < TimesSinceHeatTick / HeatTickRate; TimesSinceHeatTick -= HeatTickRate)
//	{
//		//I've found a place that iterates through the entire part grid potentially multiple times every tick... Perhaps... this is contributing massively to lag? -Mabel Suggestion
//		DistrubuteHeat();
//	}
//
//
//	TimesSinceHeatTick += DeltaTime;
//}
//
////Adds a complete part to the part grid
////I like how you commented the cpp but not the h -Mabel Suggestion
//bool UPartGridComponent::AddPart(TSubclassOf<UPart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace)
//{
//	//UE_LOG(LogTemp, Warning, TEXT("part type %s"), *PartType->GetFName().ToString());
//	if (IsValid(PartType))
//	{
//		TSet<FIntPoint> PartialPartShape = PartType.GetDefaultObject()->GetDesiredShape(Rotation);
//		return AddPart(PartialPartShape, PartType, Location, Rotation, bAlwaysPlace);
//	}
//	return false;
//}
//
////Adds a partial part to PartPrid
//bool UPartGridComponent::AddPart(TSet<FIntPoint> PartialPartShape, TSubclassOf<UPart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace)
//{
//	if (!IsValid(PartType))
//	{
//		return false;
//	}
//	//At first glance this function seems a bit long, it could probably be split into some sub-functions. -Mabel Suggestion
//	if (!PartialPartShape.IsEmpty())
//	{
//		//Create Part
//		//Thinking ahead, this function maybe shouldn't be creating a new object, and instead should be taking in a reference to an already existing object or something
//		//"But.... why?" I hear you ask... well, 2 reasons
//		//1. This function is already adding a "PartialPart", which would lead me to assume using logic that the part already exists 
//		//2. If parts split into more parts when they are cut in half, we probably wouldn't want to create a new object in this function because we would have more flexibility if 
//		//we create the object when the part is split (in terms of like, oh here, you get half the resources I had on me or something along those lines)
//		//In conclusion, I'm not so certain that this function should be creating a new object. -Mabel Suggestion
//		UPart* Part = NewObject<UPart>(this, PartType);
//		Part->InitializeVariables(Location, Rotation, this, PartType, PartialPartShape);
//
//		//Initalize Variables
//		//I find it funny that this comment is after the function "InitializeVariables" is called, haha. -Mabel Suggestion
//		TSet<FIntPoint> DesiredShape = Part->GetDesiredShape();
//		FIntBoxBounds PartBounds = Part->GetPartBounds();
//
//		AShipPlayerState* PlayerState = Cast<AShipPlayerState>(Ship->GetPlayerState());
//		//Move this to a new function called IsValidPosition or something, or IsWithinGridBounds idk -Mabel Suggestion
//		if 
//		(
//			//Detect if placement is in valid position
//			GridHalfSize.X >= Location.X + PartBounds.UpperBounds.X && -GridHalfSize.X <= Location.X + PartBounds.LowerBounds.X
//			&&
//			GridHalfSize.Y >= Location.Y + PartBounds.UpperBounds.Y && -GridHalfSize.Y <= Location.Y + PartBounds.LowerBounds.Y
//			&&
//			(bAlwaysPlace || CanShapeFit(Location, DesiredShape))
//
//			&&
//
//			//If on the player then check to see if they have enough pixels and withdraw them.
//			(Ship->GetFaction() != EFactions::Player || (IsValid(PlayerState) && PlayerState->WithdrawPixels(PartType.GetDefaultObject()->GetCost())))
//		)
//		{
//
//			//Update GridBounds
//			GridBounds.LowerBounds = GridBounds.LowerBounds.ComponentMin(Location + PartBounds.LowerBounds);
//			GridBounds.UpperBounds = GridBounds.UpperBounds.ComponentMax(Location + PartBounds.UpperBounds);
//
//
//			//Iterate though desired shape and add to part grid
//			//Comment some of these for loops and if statements because it's a lot to try to decipher -Mabel Suggestion
//
//			//.... ........... ........................... 
//			//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" 
//			//................ ... -Mabel Suggestion
//			for (FIntPoint PixelToCheck : DesiredShape)
//			{
//				//Ooof... No wonder adding large parts is slow. This is gonna sound dumb but there's got to be a better way...
//				//Adding and removing as they are now will never not be slow with big part/explosions/whatever. Having to iterate through every location of the part is...
//				//well, it's slow. I think we will probably need to find a better solution at some point. -Mabel Suggestion
//				
//				//How is this going to work with brush sizes? Currently it won't. Here's why: This will only add locations that are in the desired shape of the part.
//				//A freespace part isn't going to have a desired shape that matches the brush size. -Mabel Suggestion
//				
//				//What this is doing is backwards of how I it should be. If it's a freespace part, it *should* be adding every location in PartialPartShape. If it's not,
//				//it should check if *DesiredShape* contains *PartialPartShape[i]*, not the other way around. 
//				//Feel free to tell me that I am large dumb... -Mabel Suggestion
//				if (PartialPartShape.Contains(PixelToCheck))
//				{
//					FIntPoint CurrentLoc = FIntPoint(PixelToCheck + Location);
//
//					//Remove Overlapping Parts
//					//Why is this inside the check for "IsValidPosition"? This will never be called if it isn't in a valid 
//					//position which defeats the purpose of bAlwaysPlace 
//					//Note: On revisit, I see now that the previous previous if statement does have bAlwaysPlace in it's condition. The fact that I didn't notice that
//					//shows even more that a function along the lines of "IsWithinGridBounds" is needed, because it makes it so there's less logic surrounding bAlwaysPlace (and easier
//					//to understand) -Mabel Suggestion
//					if (bAlwaysPlace)
//					{
//						RemovePart(CurrentLoc);
//					}
//
//					//Create Mesh
//					Ship->AddMeshAtLocation(CurrentLoc);
//
//					//set PartGrid and material
//					Ship->SetMeshMaterialAtLocation(CurrentLoc, PartGrid.Emplace(CurrentLoc, FPartInstanceData(Part, 0.f, 0, Part->GetPixelMaterial())).DynamicMat);
//
//					//If it's commented out, delete it (unless it's useful debug stuff) -Mabel Suggestion
//					//Cast<AShipPlayerState>(Ship->GetPlayerState())->ShipBlueprint
//
//					//Remember to coment out debug stuff. We should probably add a comment with the word "Debug" in
//					//front of debug stuff -Mabel Suggestion
//					//UE_LOG(LogTemp, Warning, TEXT("Added: %s"), *Part->GetFName().ToString());
//				}
//			}
//
//			Part->InitializeFunctionality();
//			Ship->PixelValue += Part->GetCost();
//			Ship->PhysicsComponent->UpdateMassCalculations();
//
//			//Maybe instead of doing this here (which feels like it breaks encapsulation), you could do this in InitializeFunctionality() of the thruster. -Mabel Suggestion
//			if (Cast<UBaseThrusterPart>(Part))
//			{
//				Ship->MovementComponent->UpdateThrusters();
//			}
//
//			//Something similar could be done on the core part as I just suggested for the thruster, but this is slightly different considerent CorePart is a variable on
//			//the part grid component. (I wish the part grid didn't have to know about the core...) What do you think? -Mabel Suggestion
//			if (IsValid(Cast<UCorePart>(Part)))
//			{
//				CorePart = Cast<UCorePart>(Part);
//			}
//
//			return true;
//		}
//
//		Part->DestroyPart();
//		return false;
//	}
//	else
//	{
//		//This comment is meant to be a warning. I should probably figure out how to make it through an exception without crashing the game. This isn't just some
//		//random debug thing though. I have a couple things like this. -Mabel Suggestion
//		//UE_LOG(LogTemp, Warning, TEXT("PartialPartShape was empty"));
//		return false;
//	}
//}
//
////Remove an entire part. Returns True if a part was destroyed
//bool UPartGridComponent::RemovePart(FIntPoint Location, bool CheckForBreaks)
//{
//	//Check if location is valid
//	if (PartGrid.Contains(Location))
//	{
//		//Intialize Variables
//		class UPart* PartToRemove = PartGrid.FindRef(Location).Part;
//		if (!IsValid(Cast<UCorePart>(PartToRemove)))
//		{
//			FIntPoint PartLoc = PartToRemove->GetPartGridLocation();
//
//			//Refund Part Value
//			Cast<AShipPlayerState>(Ship->GetPlayerState())->DepositPixels(PartToRemove->GetCost());
//			Ship->PixelValue -= PartToRemove->GetCost();
//
//			//Iterate though the shape of PartToRemove and remove them from the part grid
//			for (FIntPoint Loc : PartToRemove->GetShape())
//			{
//				DestroyPixel(Loc + PartLoc, CheckForBreaks);
//			}
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else
//	{
//		return false;
//	}
//}
//
////Remove a single Pixel from the PartGrid. Returns true if a pixel was removed
//bool UPartGridComponent::DestroyPixel(FIntPoint Location, bool CheckForBreaks, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius)
//{
//	if (PartGrid.Contains(Location))
//	{
//		//Remove from grid
//		UPart* DamagedPart = PartGrid.FindRef(Location).Part;
//		//UE_LOG(LogTemp, Warning, TEXT("%s: %s -> %s >r> %s"),*DamagedPart->PartName.ToString(), *Location.ToString(), *(Location - DamagedPart->GetPartGridLocation()).ToString(), *FVector2D((Location - DamagedPart->GetPartGridLocation())).GetRotated(-1 * DamagedPart->GetRelativeRotation()).IntPoint().ToString());
//		DamagedPart->DestroyPixel(/*FVector2D(*/(Location - DamagedPart->GetPartGridLocation()), CheckForBreaks)/*.GetRotated(-1*DamagedPart->GetRelativeRotation()).IntPoint())*/;
//
//		//Destroy Mesh
//		Ship->RemoveMeshAtLocation(Location);
//
//		//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion"
//		//This should be a separate function because it doesn't have anything to do with actually destroying the pixel. Like "Transfer heat" or something. -Mabel Suggestion
//		//Edit: You have a "DistributeHeat" function! Use it.
//		//Edit 2: The way you have "DistributeHeat" set up wouldn't work for this situation, so either you need to make DistributeHeat more general or make a new function. If you do
//		//leave "DistributeHeat" the way it is, consider renaming it so it doesn't cause confusion.
//		for (int i = 0; i < 4; i++)
//		{
//			FIntPoint TargetPoint = Location +((i % 2 == 1) ? FIntPoint((i > 1) ? 1 : -1, 0) : FIntPoint(0, (i > 1) ? 1 : -1));
//
//			ApplyHeatAtLocation(TargetPoint, (PartGrid.FindRef(Location).GetTemperature() / 4) * HeatMeltTransferFactor);
//		}
//
//		TArray<FIntPoint> NumbersFound;
//		PartGrid.Remove(Location);
//
//		/*
//		* I've seen lines 230 - 294 before in BaseResouceSystem::RemovePixel. Should be a function on TGridMap<> 
//		* This function should have the signatures:
//		* 1) void CheckForBreaks(TArray<TGridMap<ValueType>>& NewGrids, BinaryPredicate ConectivityCondition)
//		* 2) void CheckForBreaks(TArray<TGridMap<ValueType>>& NewGrids, BinaryPredicate ConectivityCondition, Location)
//		* 
//		* This function will set NewGrids to all grids created by: 1) any breaks in the map 2) Any breaks created around Location.
//		* 2) assumes pixel at Location is empty.
//		*/
//		if (CheckForBreaks)
//		{
//			if (PartGrid.Contains(FIntPoint(Location.X + 1, Location.Y)))
//			{
//				NumbersFound.Add(FIntPoint(Location.X + 1, Location.Y));
//			}
//			if (PartGrid.Contains(FIntPoint(Location.X - 1, Location.Y)))
//			{
//				NumbersFound.Add(FIntPoint(Location.X - 1, Location.Y));
//			}
//			if (PartGrid.Contains(FIntPoint(Location.X, Location.Y + 1)))
//			{
//				NumbersFound.Add(FIntPoint(Location.X, Location.Y + 1));
//			}
//			if (PartGrid.Contains(FIntPoint(Location.X, Location.Y - 1)))
//			{
//				NumbersFound.Add(FIntPoint(Location.X, Location.Y - 1));
//			}
//
//			//If NumbersFound is less than 2 then you don't need to bother checking anything since there will be no breaks
//			if (NumbersFound.Num() > 1)
//			{
//				if (IsValid(CorePart) && !CorePart->GetShape().IsEmpty())
//				{
//					FIntPoint CorePixel;
//					for (FIntPoint Pixel : CorePart->GetShape())
//					{
//						CorePixel = Pixel;
//						break;
//					}
//
//					//Don't auto also name iterator -Mabel Suggestion (-Liam suggestion so I don't forget)
//					for (auto& i : NumbersFound)
//					{
//						if (PartGrid.Contains(i))
//						{
//							if (!PartGrid.PointsConnected(CorePixel, i, AlwaysConnect<FPartInstanceData>))
//							{
//								TSet<FIntPoint> Temp;
//								Temp.Emplace(i);
//								TSet<FIntPoint> ConnectedShape = UPartGridComponent::FindConnectedShape(Temp, PartGrid);
//
//								RemoveDisconnectedShape(ConnectedShape, FromExplosion, ExplosionLocation, ExplosionRadius);
//							}
//						}
//					}
//				}
//				else
//				{
//					//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
//					//For each in NumbersFound.Num() - 1 because of how PointsConnected works
//					for (int i = 0; i < NumbersFound.Num() - 1; i++)
//					{
//						//This needs to be improved, but right now it checks if the current index is connected to the next index.
//						//actually it might not need to be improved but i need to think about it
//						if (PartGrid.Contains(NumbersFound[i]) && PartGrid.Contains(NumbersFound[i + 1]))
//						{
//							if (!PartGrid.PointsConnected(NumbersFound[i], NumbersFound[i + 1], AlwaysConnect<FPartInstanceData>))
//							{
//								//If they're not connected, then call FindConnectedShape to figure out what part is not connected. Anything connected to the part that is not connected will
//								//also not be connected.
//								TSet<FIntPoint> Temp;
//								Temp.Emplace(NumbersFound[i + 1]);
//								TSet<FIntPoint> ConnectedShape = UPartGridComponent::FindConnectedShape(Temp, PartGrid);
//
//								RemoveDisconnectedShape(ConnectedShape, FromExplosion, ExplosionLocation, ExplosionRadius);
//
//							}
//						}
//					}
//				}
//			}
//		}
//		Ship->PhysicsComponent->UpdateMassCalculations();
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//
//}
//
///*
//* Function comments from the .h should be copied to the .cpp
//* 
//* Function should be overloaded. (This is false due confusion about what the parameters do/are used for but I'm leaving it because its good info)
//* Instead of having the FromExplosion parameter and then a bunch of other realted parameters,
//* The function should be overloaded with two parameter sets that do differnt things
//*
//* OR
//*
//* Create a one function for convering information about an explosion into a TArray<FIntPoint>.
//* This function could likly be used in other functions
//*
//*
//* Confusing parameters.
//* It seems like it will generate a new shape to be removed using the explosion info.
//* Explosion information should be a struct so that it can be constucted multiple ways and so it can be comprest to one parameter for beter readablilty.
//*
//* Function name inaccurate.
//* The function has not conectivity detection therefore it does not remove the *Disconnected* shape.
//* Name does not communicate that it creates a new ship.
//* Consider renaming to one of the following or similar:
//* DisconectShape()
//* CreateNewShipFromShape()
//* 
//* Why does the function take in a TArray<FIntPoint> when it could use a TGridMap<FPartInstanceData> and BuildShip() instead and reduce the logic required.
//* - Liam Suggestion
//*/
//void UPartGridComponent::RemoveDisconnectedShape(TSet<FIntPoint> ConnectedShape, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius)
//{
//	TSet<UPart*> PartsRemoved;
//
//	//Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion
//	for (auto& j : ConnectedShape)
//	{
//		PartsRemoved.Emplace(PartGrid.Find(j)->Part);
//	}
//
//	//Create a new ship with these parts
//	if (!PartsRemoved.IsEmpty())
//	{
//
//		ABaseShip* NewShip = GetWorld()->SpawnActor<ABaseShip>((GetOwner()->GetActorLocation(), FQuat(), FActorSpawnParameters()));
//
//		/*
//		* Iterator should have a name that tells what it actualy is and what its iterating through.
//		* Don't use auto&.
//		* - Liam Suggestion
//		*/
//		for (auto& j : PartsRemoved)
//		{
//			TSet<FIntPoint> PartialPartShape;
//
//			/*
//			* Iterator should have a name that tells what it actualy is and what its iterating through.
//			* Don't use auto&.
//			* - Liam Suggestion
//			*/
//			for (auto& k : j->GetShape())
//			{
//				if (ConnectedShape.Contains(k + j->GetPartGridLocation()))
//				{
//					PartialPartShape.Emplace(k);
//					DestroyPixel(k + j->GetPartGridLocation(), false);
//
//				}
//			}
//
//			NewShip->PartGrid->AddPart(PartialPartShape, j->GetClass(), j->GetPartGridLocation(), j->GetRelativeRotation());
//		}
//
//		//Long chunks of math should be commented. - Liam Suggestion.
//		FVector NewLocation = GetOwner()->GetActorLocation() + FVector(NewShip->PhysicsComponent->GetCenterOfMass(), 0) - FVector(Ship->PhysicsComponent->GetCenterOfMass(), 0);
//		//Confuesing variable name. What even is RotateLocation? It appears to have nothing to do with rotation. - Liam Suggestion
//		FVector RotateLocation = GetOwner()->GetActorLocation();
//
//		NewLocation = RotateLocation - GetOwner()->GetActorRotation().RotateVector(RotateLocation - NewLocation);
//
//
//		NewShip->SetActorLocation(NewLocation);
//		NewShip->SetActorRotation(GetOwner()->GetActorRotation());
//
//		//Long chunks of math should be commented. - Liam Suggestion.
//		float Radius = UKismetMathLibrary::Sqrt(FMath::Square((NewShip->PhysicsComponent->GetCenterOfMass().X + NewShip->GetActorLocation().X) - (Ship->PhysicsComponent->GetCenterOfMass().X + Cast<AActor>(GetOwner())->GetActorLocation().X)) + FMath::Square((NewShip->PhysicsComponent->GetCenterOfMass().Y + NewShip->GetActorLocation().Y) - (Ship->PhysicsComponent->GetCenterOfMass().Y + Cast<AActor>(GetOwner())->GetActorLocation().Y)));
//		float VelocityFromRotationMagnitude = Ship->PhysicsComponent->GetAngularVelocity() * Radius;
//		//Long chunks of math should be commented. - Liam Suggestion.
//		FVector2D VectorBetween = NewShip->PhysicsComponent->GetCenterOfMass() + FVector2D(NewShip->GetActorLocation()) - (Ship->PhysicsComponent->GetCenterOfMass() + FVector2D(Cast<AActor>(GetOwner())->GetActorLocation()));
//		FVector2D RotatedVector = VectorBetween.GetRotated(90);
//
//		RotatedVector.Normalize();
//
//
//		/*
//		* Delete Debug code or come up with a good way for c++ debug modes.
//		* 
//		* Unclear Comment. What is the debug code helping to visualize/test?
//		* - Liam Suggestion
//		*/ 
//		//Debug stuff
//		/*UE_LOG(LogTemp, Warning, TEXT("velocity from rotation magnitude %f"), VelocityFromRotationMagnitude);
//		UE_LOG(LogTemp, Warning, TEXT("angular velocity of the ship %f"), Ship->PhysicsComponent->GetAngularVelocity());
//		UE_LOG(LogTemp, Warning, TEXT("radius %f"), Radius);
//		DrawDebugDirectionalArrow(GetWorld(), NewShip->GetActorLocation(), NewShip->GetActorLocation() + FVector(RotatedVector * VelocityFromRotationMagnitude, 0), 5, FColor::Red, true);
//		DrawDebugDirectionalArrow(GetWorld(), NewShip->GetActorLocation(), NewShip->GetActorLocation() + FVector(Ship->PhysicsComponent->GetVelocity(), 0), 5, FColor::Blue, true);
//		DrawDebugDirectionalArrow(GetWorld(), NewShip->GetActorLocation(), NewShip->GetActorLocation() + FVector(Ship->PhysicsComponent->GetVelocity(), 0) + FVector(RotatedVector * VelocityFromRotationMagnitude, 0), 5, FColor::Green, true);
//		DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + FVector(VectorBetween, 0), 5, FColor::Yellow, true);*/
//
//		//Breaks encapsulation. This should be handled in ExplodeAtWorldLocation() - Liam Suggestion
//		if (FromExplosion)
//		{
//			NewShip->PartGrid->ExplodeAtLocation(ExplosionLocation, ExplosionRadius);
//		}
//
//		NewShip->PhysicsComponent->AddImpulse((RotatedVector * VelocityFromRotationMagnitude) + Ship->PhysicsComponent->GetVelocity(), NewShip->PhysicsComponent->GetCenterOfMass());
//	}
//	else
//	{
//		//Improve error message. Message should include where the error happened and why it was triggered - Liam Suggestion
//		UE_LOG(LogTemp, Error, TEXT("I don't think this should ever happen. Ask Mabel Suggestion about weird part grid component thing"));
//	}
//}
//
////Comment -Mabel Suggestion
//void UPartGridComponent::ApplyHeatAtLocation(FVector WorldLocation, float HeatToApply)
//{
//	//Shouldn't this call the other ApplyHeatAtLocation? -Mabel Suggestion
//	ApplyHeatAtLocation((FVector2D(WorldLocation - GetOwner()->GetActorLocation()).GetRotated(-1 * GetOwner()->GetActorRotation().Yaw) + Ship->PhysicsComponent->GetCenterOfMass()).RoundToVector().IntPoint(), HeatToApply);
//		
//	//If you don't need it, delete it -Mabel Suggestion
//	//PartGrid.FindRef(FVector2D(WorldLocation - GetOwner()->GetActorLocation()).RoundToVector().IntPoint()).SetTemperature(HeatToApply);
//}
//
////Comment -Mabel Suggestion
//void UPartGridComponent::ApplyHeatAtLocation(FIntPoint RelativeLocation, float HeatToApply)
//{
//	//DrawDebugPoint(GetWorld(), GetOwner()->GetActorTransform().TransformPosition(FVector(FVector2D(RelativeLocation), .1)), 5, PartGrid.Contains(RelativeLocation) ? FColor::Green : FColor::Red, false, 5);
//	if (PartGrid.Contains(RelativeLocation))
//	{
//		PartGrid.Find(RelativeLocation)->SetTemperature(PartGrid.Find(RelativeLocation)->GetTemperature() + HeatToApply);
//	}
//	
//}
//
////I need to comment -Mabel Suggestion
//void UPartGridComponent::ExplodeAtLocation(FVector WorldLocation, float ExplosionRadius)
// {
//	//Delete Debug code or come up with a good way for c++ debug modes. - Liam Suggestion
//	//UE_LOG(LogTemp, Warning, TEXT("explodd but part grid"))
//
//	/*
//	* Weird Name. 
//	* What does this variable have to do with floats?
//	* Relative Location of what?
//	* 
//	* Why is this variable a FVector insead of a FVector2D?
//	* The Z component is never used so it shouldnt be stored.
//	* This also makes the conversion to FIntPoint much easier because of FVector2D::IntPoint().
//	* - Liam Suggestion
//	*/
//	FVector FloatRelativeLoc = UKismetMathLibrary::InverseTransformLocation(GetOwner()->GetActorTransform(), WorldLocation) + FVector(Ship->PhysicsComponent->GetCenterOfMass(), 0);
//	
//	/*
//	* CheckX & CheckY should be one FVector2D as that reduces the code needed.
//	* eg: you could use Check.SizeSquared() insead of CheckX * CheckX + CheckY * CheckY
//	* 
//	* Bad Names. 
//	* What are the variables storing?
//	* All variables names should be nouns acording to UE coding standards.
//	* - Liam Suggestion
//	*/ 
//	float CheckX = -ExplosionRadius;
//	float CheckY = -ExplosionRadius;
//
//	/*
//	* Scope Issue. Should be declared inside the while loop because it is never used outside it.
//	*
//	* Bad Name.
//	* All variables names should be nouns acording to UE coding standards.
//	* - Liam Suggestion
//	*/
//	FIntPoint CheckGridLocation;
//	TArray<FIntPoint> LocationsToBeDestroyed;
//	//Delete Debug code or come up with a good way for c++ debug modes.
//	//DrawDebugPoint(GetWorld(), WorldLocation, ExplosionRadius * 50, FColor::Red, false, 2.0F);
//	
//	//UE_LOG(LogTemp, Warning, TEXT("Explosion %f"), ExplosionRadius);
//	//UE_LOG(LogTemp, Warning, TEXT("CheckY %f"), CheckY);
//
//	//This could be a for loop using Check as its iterator
//	while (ExplosionRadius >= CheckY)
//	{
//		CheckGridLocation = FIntPoint(FGenericPlatformMath::RoundToInt(CheckX + FloatRelativeLoc.X), FGenericPlatformMath::RoundToInt(CheckY + FloatRelativeLoc.Y));
//		if (PartGrid.Contains(CheckGridLocation) && CheckX * CheckX + CheckY * CheckY <= ExplosionRadius * ExplosionRadius)
//		{
//			/*
//			* Hard to read.
//			* Using Location & Extent for Box Creation would be simpler.
//			* 
//			* Comment be in proper English (sorry I know it is fun)
//			* - Liam Suggestion
//			*/
//			//obviously if it contains the FloatRelativeLoc then it ded
//			if (BoxContainsLocation(FVector2D(CheckGridLocation.X - GridScale / 2, CheckGridLocation.Y + GridScale / 2), FVector2D(CheckGridLocation.X + GridScale / 2, CheckGridLocation.Y - GridScale / 2), FVector2D(FloatRelativeLoc)))
//			{
//				LocationsToBeDestroyed.Emplace(CheckGridLocation);
//			}
//			//Comment too large. Should explain things as the code executes not all at the begining. Also Remove parts of the comment that dont explain how the code works. - Liam Suggestion
//			// PartGrid.Find(CheckGridLocation)->Part->GetStrength())
//			//y = mx + b || Ew use point slop form - Liam
//			// Assume for a second FloatRelativeLoc is the origin of the ship ((0,0) on the part grid) and the part is somewhere between directly above and directly to the right
//			// of the FloatRelativeLoc
//			// b is 0
//			//slope will be CheckGridLocation.Y/CheckGridLocation.X
//			//How to figure out what parts to check?
//			//assume for a second the CheckGridLocation is (1,2)
//			//Slope is 2
//			//y = 2x
//			//if we just go by integers of the slope next point checked is the origin. not super helpful
//			// Alternate x and y, but if the pixel doesn't contain something in the slope check the other?
//			// Check X - 1 from the CheckGridLocation, if the bounds of the pixel do not contain something in the slope, check y - 1
//			// How to check if bounds of pixel contain something in the slope?
//			// in this case, if top left location Y is > the Y with the slope of the line at that X location 
//			// and bottom right Y is < the Y with the slop of the line at that X location
//			// Divide radius by the strength of the pixel found, then check y-1 x-1. Divide by that strength then check y-2 x-1
//			// and so on till you get to origin. So there needs to be a recursive function. or maybe a while()? Yes, while(Location checking does not contain origin)
//			//So separate slope rise and slope run.
//			// What if FloatRelativeLoc is not 0,0 (still assuming part is up and to the right)
//			//slope rise and run will be (CheckGridLocation.Y - FloatRelativeLoc.Y) and (CheckGridLocation.X - FloatRelativeLoc.X)
//			// Check X - 1 from the CheckGridLocation, if the bounds of the pixel do not contain something in the slope, check y - 1
//			// Divide radius by the strength of all the pixels found
//			//and so on until you get to the location that contains the FloatRelativeLoc. So there should also be a check for each location if it contains the FloatRelativeLoc
//			//How to check if it contains the FloatRelativeLoc?
//			//Top left of location X is < FloatRelativeLoc.X and Y is > FloatRelativeLoc.Y,
//			//and bottom right of location X is > FloatRelationLoc.X and Y is < FloatRelativeLoc.Y
//			//What if the part is not up and to the right?
//			//Need to figure out what quadrant the part is in or what axis it's on compared to the FloatRelativeLoc
//			//Okay so a switch based on the quadrant
//			else
//			{
//				//Should be an FIntPoint for readablitity and FIntPoint functions. - Liam Suggestion
//				int XDirection = 0;
//				int YDirection = 0;
//
//				/*
//				* bool property should start with b
//				* Stop Checking what?
//				* - Liam Suggestion
//				*/
//				bool StopChecking = false;
//
//				/*
//				* Should be an FVector2D for readablitity and FVector2D functions.
//				* 
//				* Bad Name.
//				* Rise and Run are not technical terms.
//				* Slope means deltaY/deltaX which is not what this variable is storing. It stores a direction
//				* - Liam Suggestion
//				*/
//				float SlopeRise = CheckGridLocation.X - FloatRelativeLoc.X;
//				float SlopeRun = CheckGridLocation.Y - FloatRelativeLoc.Y;
//
//				/*
//				* Arbitrary switch cases need commenting.
//				* 
//				* Why are you using FVector2D(float,float) insead of FVector2D(FVector)?
//				* 
//				* If GetQuadrantFromLocation() returned a FVector2D then switch would be unnesseary.
//				* - Liam Suggestion
//				*/
// 				switch (GetQuadrantFromLocation(FVector2D(CheckGridLocation.X, CheckGridLocation.Y), FVector2D(FloatRelativeLoc)))
//				{
//
//				case 0:
//					StopChecking = true;
//					break;
//				case 1:
//					XDirection = -1;
//					YDirection = -1;
//					break;
//				case 2:
//					XDirection = -1;
//					YDirection = 1;
//					break;
//				case 3:
//					XDirection = 1;
//					YDirection = 1;
//					break;
//				case 4:
//					XDirection = 1;
//					YDirection = -1;
//					break;
//				case 5:
//					YDirection = -1;
//					break;
//				case 6:
//					XDirection = -1;
//					break;
//				case 7:
//					YDirection = 1;
//					break;
//				case 8:
//					XDirection = 1;
//					break;
//				}
//
//				/*
//				* Should be a FIntPoint.
//				* Location of what?
//				* - Liam Suggestion
//				*/
//				int XLocation = CheckGridLocation.X;
//				int YLocation = CheckGridLocation.Y;
//				float NewRadius = ExplosionRadius;
//				while (!StopChecking)
//				{
//					//Delete unused code - Liam Suggestion
//					//XLocation += XDirection;
//					//YLocation += YDirection;
//
//					/*
//					* Using Location & Extent for Box Creation would be simpler. 
//					* If you convert FloatRelativeLoc to relative space you could use Location == (InGridSpace)FloatRelativeLoc
//					* 
//					* 
//					* Logic needs Comment.
//					* I have no clue what any of this is doing and why.
//					* 
//					* Logic Issue 
//					* Location should be muiltipled by grid scale
//					* - Liam Suggestion
//					*/ 
//					if (BoxContainsLocation(FVector2D(float(XLocation) + GridScale / 2, float(YLocation) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation) + GridScale / 2), FVector2D(FloatRelativeLoc)))
//					{
//						if (PartGrid.Contains(FIntPoint(XLocation + XDirection, YLocation)))
//						{
//							NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation + XDirection, YLocation))->Part->GetStrength();
//						}
//						StopChecking = true;
//						break;
//					}
//					/*
//					* If you convert FloatRelativeLoc to part grid space you could use Location + Direction == (InGridSpace)FloatRelativeLoc
//					* 
//					* Code is repeaded should be a function or a loop. 
//					* - Liam Suggestion
//					*/
//					if (BoxContainsLocation(FVector2D(float(XLocation + XDirection) + GridScale / 2, float(YLocation) - GridScale / 2), FVector2D(float(XLocation + XDirection) - GridScale / 2, float(YLocation) + GridScale / 2), FVector2D(FloatRelativeLoc)))
//					{
//						if (PartGrid.Contains(FIntPoint(XLocation + XDirection, YLocation)))
//						{
//							NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation + XDirection, YLocation))->Part->GetStrength();
//						}
//						StopChecking = true;
//						break;
//					}
//					else if (YDirection != 0)
//					{
//						if (XDirection != 0)
//						{
//							if (DoesLineIntersectBox(FVector2D(float(XLocation + XDirection) + GridScale / 2, float(YLocation) - GridScale / 2), FVector2D(float(XLocation + XDirection) - GridScale / 2, float(YLocation) + GridScale / 2), SlopeRise, SlopeRun, FVector2D(FloatRelativeLoc)))
//							{
//								XLocation += XDirection;
//								if (PartGrid.Contains(FIntPoint(XLocation, YLocation)))
//								{
//									NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation))->Part->GetStrength();
//								}
//							}
//							else if (BoxContainsLocation(FVector2D(float(XLocation) + GridScale / 2, float(YLocation + YDirection) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation + YDirection) + GridScale / 2), FVector2D(FloatRelativeLoc)))
//							{
//								if (PartGrid.Contains(FIntPoint(XLocation, YLocation + YDirection)))
//								{
//									NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation + YDirection))->Part->GetStrength();
//								}
//								StopChecking = true;
//								break;
//							}
//							else if (DoesLineIntersectBox(FVector2D(float(XLocation) + GridScale / 2, float(YLocation + YDirection) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation + YDirection) + GridScale / 2), SlopeRise, SlopeRun, FVector2D(FloatRelativeLoc)))
//							{
//								YLocation += YDirection;
//								if (PartGrid.Contains(FIntPoint(XLocation, YLocation)))
//								{
//									NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation))->Part->GetStrength();
//								}
//							}
//						}
//						else if (BoxContainsLocation(FVector2D(float(XLocation) + GridScale / 2, float(YLocation + YDirection) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation + YDirection) + GridScale / 2), FVector2D(FloatRelativeLoc)))
//						{
//							if (PartGrid.Contains(FIntPoint(XLocation, YLocation + YDirection)))
//							{
//								NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation + YDirection))->Part->GetStrength();
//							}
//							StopChecking = true;
//							break;
//						}
//						else if (DoesLineIntersectBox(FVector2D(float(XLocation) + GridScale / 2, float(YLocation + YDirection) - GridScale / 2), FVector2D(float(XLocation) - GridScale / 2, float(YLocation + YDirection) + GridScale / 2), SlopeRise, SlopeRun, FVector2D(FloatRelativeLoc)))
//						{
//							YLocation += YDirection;
//							if (PartGrid.Contains(FIntPoint(XLocation, YLocation)))
//							{
//								NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation))->Part->GetStrength();
//							}
//						}
//					}
//					else
//					{
//						if (DoesLineIntersectBox(FVector2D(float(XLocation + XDirection) + GridScale / 2, float(YLocation) - GridScale / 2), FVector2D(float(XLocation + XDirection) - GridScale /2, float(YLocation) + GridScale / 2), FloatRelativeLoc.Y))
//						{
//							XLocation += XDirection;
//							if (PartGrid.Contains(FIntPoint(XLocation, YLocation)))
//							{
//								NewRadius = NewRadius / PartGrid.Find(FIntPoint(XLocation, YLocation))->Part->GetStrength();
//							}
//						}
//					}
//				}
//				//PartGrid.Contains(CheckGridLocation) is unnessary as destory pixel already does that.
//				if (PartGrid.Contains(CheckGridLocation) && CheckX * CheckX + CheckY * CheckY <= NewRadius * NewRadius)
//				{
//					LocationsToBeDestroyed.Emplace(CheckGridLocation);
//				}
//			}
//		}
//
//
//		//This iterates though a square with an extent of ExplosionRadius. Consider changin iterater to iterate a circle. - Liam Suggestion
//		CheckX += 1;
//		if (CheckX > ExplosionRadius)
//		{
//			CheckY += 1;
//			CheckX = -ExplosionRadius;
//		}
//	}
//	for (auto& i : LocationsToBeDestroyed)
//	{
//		DestroyPixel(i, true, true, WorldLocation, ExplosionRadius);
//	}
//
//}
//
//bool UPartGridComponent::BoxContainsLocation(FVector2D TopLeft, FVector2D BottomRight, FVector2D Location)
//{
//	if (TopLeft.X >= Location.X && TopLeft.Y <= Location.Y && BottomRight.X <= Location.X && BottomRight.Y >= Location.Y)
//	{
//		return true;
//	}
//	return false;
//}
//
//int UPartGridComponent::GetQuadrantFromLocation(FVector2D Location, FVector2D Origin)
//{
//
//	//Check if the location is the origin.
//	if (Location == Origin)
//	{
//		return 0;
//	}
//
//	//Check if the location is on the Y axis.
//	if (Location.X == Origin.X)
//	{
//		if (Location.Y > Origin.Y)
//		{
//			return 5;
//		}
//		else
//		{
//			return 7;
//		}
//	}
//
//	//Check if the location is on the X axis.
//	if (Location.Y == Origin.Y)
//	{
//		if (Location.X > Origin.X)
//		{
//			return 6;
//		}
//		else
//		{
//			return 8;
//		}
//	}
//
//	//Check if the location is in the first or second quadrants.
//	if (Location.X > Origin.X)
//	{
//		//Check if the location is in the first quadrant.
//		if (Location.Y > Origin.Y)
//		{
//			return 1;
//		}
//		else
//		{
//			return 2;
//		}
//	}
//	else
//	{
//		//Check if the location is in the fourth quadrant.
//		if (Location.Y > Origin.Y)
//		{
//			return 4;
//		}
//		else
//		{
//			return 3;
//		}
//	}
//}
//
//bool UPartGridComponent::DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float SlopeRise, float SlopeRun, FVector2D origin)
//{
//	float XIntercept = 0;
//	FVector2D LocalTopLeft = TopLeft - origin;
//	FVector2D LocalBottomRight = BottomRight - origin;
//
//	if (SlopeRise == 0)
//	{
//		if (LocalTopLeft.X > XIntercept && LocalBottomRight.X < XIntercept)
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//	else if (SlopeRise < 0 || SlopeRun < 0)
//	{
//		if (!(SlopeRise < 0 && SlopeRun < 0))
//		{
//			FVector2D LocalBottomLeft = FVector2D(LocalBottomRight.X, LocalTopLeft.Y);
//			FVector2D LocalTopRight = FVector2D(LocalTopLeft.X, LocalBottomRight.Y);
//
//			if (LocalBottomLeft.X <= (SlopeRise / SlopeRun) * (LocalBottomLeft.Y) + XIntercept && LocalTopRight.X >= (SlopeRise/SlopeRun) * (LocalTopRight.Y) + XIntercept)
//			{
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		}
//	}
//	if (LocalTopLeft.X >= (SlopeRise / SlopeRun) * (LocalTopLeft.Y) + XIntercept && LocalBottomRight.X <= (SlopeRise / SlopeRun) * (LocalBottomRight.Y) + XIntercept)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
//bool UPartGridComponent::DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float YIntercept)
//{
//	return TopLeft.Y < YIntercept&& BottomRight.Y > YIntercept;
//}
//
////Comment -Mabel Suggestion
//FPartInstanceData UPartGridComponent::GetPartDataAtGridLocation(FIntPoint Location)
//{
//	return PartGrid.FindRef(Location);
//}
//
////Comment -Mabel Suggestion
//FPartInstanceData UPartGridComponent::GetPartDataAtRelativeLocation(FVector Location)
//{
//	//You need to factor in the part grid scale. Maybe the center of mass too, although not sure about that one. -Mabel Suggestion
//	return GetPartDataAtGridLocation(FVector2D(Location).RoundToVector().IntPoint());
//}
//
////Comment -Mabel Suggestion
//FPartInstanceData UPartGridComponent::GetPartDataAtWorldLocation(FVector Location)
//{
//	return GetPartDataAtRelativeLocation(Ship->GetActorQuat().UnrotateVector(Location - Ship->GetActorLocation()));
//}
//
////Comment -Mabel Suggestion
//void UPartGridComponent::DistrubuteHeat()
//{
//	TMap<FIntPoint, float> NewHeatMap = TMap<FIntPoint, float>();
//	NewHeatMap.Reserve(PartGrid.Num());
//
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" -Mabel Suggestion
//	for (int j = 0; j < PartGrid.Num(); j++)
//	{
//		float NewHeat = 0;
//
//		//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" -Mabel Suggestion
//		for (int i = 0; i < 4; i++)
//		{
//			FIntPoint TargetPoint = ((i % 2 == 1) ? FIntPoint((i > 1) ? 1 : -1, 0) : FIntPoint(0, (i > 1) ? 1 : -1));
//			
//			if (PartGrid.Contains(TargetPoint + PartGrid.LocationAtIndex(j)))
//			{
//				//4 is borderline magic number. I understand why you used it but still -Mabel Suggestion
//				NewHeat += PartGrid.FindRef(TargetPoint + PartGrid.LocationAtIndex(j)).GetTemperature() * HeatPropagationFactor / (4);
//			}
//		}
//
//		//Math is occuring that needs to be commented. Why is the pixels current temperature mutiplied by 1 - the heat propagation factor? -Mabel Suggestion
//		NewHeat = PartGrid.ValueAtIndex(j).GetTemperature() * (1-HeatPropagationFactor) + NewHeat;
//
//
//		//Why 0.5? comment plz (also, is it.... magic number?) -Mabel Suggestion
//		NewHeatMap.Emplace(PartGrid.LocationAtIndex(j), NewHeat > .05 ? NewHeat : 0);
//	}
//
//	TArray<FIntPoint> KeysToDestroy = TArray<FIntPoint>();
//
//	//You iterate through the part grid not once, but twice in this function. Big oof if I'm being honest. Honestly, it'd be better to just have heat not spread
//	//if this is the way that we're doing it. This is so bad for large ships. 
//	//I wonder if each part could handle it's own heat and distribute to the places around it instead of doing this on the part grid? I don't know how much it would help but 
//	//it might help a little bit. Not sure, that might be just as laggy. -Mabel Suggestion
//	for (int i = 0; i < PartGrid.Num(); i++)
//	{
//		if (NewHeatMap.FindRef(PartGrid.LocationAtIndex(i)) > PartGrid.ValueAtIndex(i).Part->GetHeatResistance())
//		{
//			KeysToDestroy.Emplace(PartGrid.LocationAtIndex(i));
//		}
//		else
//		{
//			PartGrid.ValueAtIndex(i).SetTemperature(NewHeatMap.FindRef(PartGrid.LocationAtIndex(i)));
//		}
//	}
//
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" 
//	//"Val" is just as bad as i. Just saying. -Mabel Suggestion
//	for (FIntPoint Val : KeysToDestroy)
//	{
//		DestroyPixel(Val);
//	}
//}
//
//
////Why is this destroy pixel not next to the other destroy pixel? Also, comment -Mabel Suggestion
//bool UPartGridComponent::DestroyPixel(FIntPoint Location, class UPart*& DamagedPart, bool CheckForBreaks, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius)
//{
//	//Check if pixel is valid
//	if (PartGrid.Contains(Location))
//	{
//		//Remove from grid
//		//I don't understand. Why is damaged part even a parameter? I'm confused. If it's supposed to be like "Here's a variable that I'll input to see what part is damaged",
//		//then name the paramater "OutPartDamaged" or "OutDamagedPart". That falls in line with Unreal and is less confusing -Mabel Suggestion
//		DamagedPart = PartGrid.FindRef(Location).Part;
//	}
//	return DestroyPixel(Location, CheckForBreaks, FromExplosion, ExplosionLocation, ExplosionRadius);
//}
//
////Comment -Mabel Suggestion
//void UPartGridComponent::BuildShip(TArray<FSavePartInfo> Parts)
//{
//	TArray<FIntPoint> AllParts = PartGrid.GetKeyArray();
//
//	for (auto& i : AllParts)
//	{
//		RemovePart(i, false);
//	}
//	for (int i = 0; i < Parts.Num(); i++)
//	{
//		//Debug
//		//UE_LOG(LogTemp, Warning, TEXT("build ship part class %s"), *Parts[i].PartClass.Get()->GetDisplayNameText().ToString())
//		AddPart(Parts[i].PartClass, Parts[i].PartLocation, Parts[i].PartRotation, false);
//	}
//}
//
////Comment -Mabel Suggestion
//void UPartGridComponent::SaveShip(FString ShipName)
//{
//	
//	TArray<FPartInstanceData> Parts = PartGrid.GetValueArray();
//
//	
//	
//	USaveGame* SaveGameInstance = UGameplayStatics::CreateSaveGameObject(USaveShip::StaticClass());
//
//	for (int i = 0; i < Parts.Num(); i++)
//	{
//		Cast<USaveShip>(SaveGameInstance)->SavedShip.Add(FSavePartInfo(Parts[i].Part->GetClass(), Parts[i].Part->GetPartGridLocation(), Parts[i].Part->GetRelativeRotation()));
//	}
//	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, ShipName, 0);
//
//}
//
////Comment -Mabel Suggestion
//bool UPartGridComponent::LoadSavedShip(FString ShipName)
//{
//	USaveGame* SaveGameInstance = UGameplayStatics::LoadGameFromSlot(ShipName, 0);
//	if (IsValid(SaveGameInstance))
//	{
//		BuildShip(Cast<USaveShip>(SaveGameInstance)->SavedShip);
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
////Gets the center of mass of the PartGrid
////Change comment: Calculates the center of mass of the PartGrid -Mabel Suggestion
//const FVector2D UPartGridComponent::CalcCenterOfMass()
//{
//	//Initalize Variables
//	FVector2D Center = FVector2D(0,0);
//	float Mass = CalcMass();
//
//	//Insert "What should we do with debug?" comment here -Mabel Suggestion
//	//UE_LOG(LogTemp, Warning, TEXT("PRE cofmass?? x=%f, y=%f, Mass=%f"), Center.X, Center.Y, Mass);
//
//	//Iterate though Pixels and adjust center of mass
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" -Mabel Suggestion
//
//	//NOOOO another iteration through the part grid! Sadge. This is unfortanate because, doesn't this get called every time destroy pixel is called? Yet another reason
//	//DestroyPixel cannot be optimized. We really need a better way to do this. I mean really, imagine you blow up an huge ship, and you destroy each pixel one by one.
//	//Because you're blowing up that ship's entire part grid and removing each pixel, that's one iteration through the part grid. Then... each time you remove a pixel, that's 
//	//another iteration through the whole part grid for calculating the mass. Then you do another iteration through the part grid for the center of mass.
//	//Then you do *yet another* iteration through the part grid for the moment of inertia.
//	// ... For each pixel destroyed, you iterate through the part grid 3 separate times. At least do center of mass and moment of inertia in the same iteration!
//	// Also, you don't need to calculate mass the way that you are -- you can just += or -= mass whenever you gain or lose mass.
//	// That way you wouldn't need to iterate through the part grid to calculate mass.
//	//-Mabel Suggestion
//	for (int i = 0; i < PartGrid.Num(); i++)
//	{
//		Center += FVector2D(PartGrid.LocationAtIndex(i)) * PartGrid.ValueAtIndex(i).Part->GetMass() / Mass;
//	}
//
//	//DEbuG? whAt shall we ever do??? -Mabel Suggestion
//	//UE_LOG(LogTemp, Warning, TEXT("cofmass?? x=%f, y=%f"), Center.X, Center.Y);
//	
//	
//	return Center;
//}
//
////Comment -Mabel Suggestion
//const float UPartGridComponent::CalcMomentOfInertia()
//{
//	float ReturnValue = 0;
//	FVector2D CenterOfMass = Ship->PhysicsComponent->GetCenterOfMass();
//
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion"  -Mabel Suggestion
//	//i cry... not the part grid! spare me!
//	for (int i = 0; i < PartGrid.Num(); i++)
//	{
//		//Comment your math plz -Mabel Suggestion
//		float PartMass = PartGrid.ValueAtIndex(i).Part->GetMass();
//		ReturnValue += (1 / 12) + PartMass * (FVector2D(PartGrid.ValueAtIndex(i).Part->GetPartRelativeLocation())).SizeSquared();
//	}
//	return ReturnValue;
//}
//
////Calculates the mass of the PartGrid
//const float UPartGridComponent::CalcMass()
//{
//	//As mentioned before, this theoretically shouldn't be a function that's needed. You can just += or -= whenever
//	//mass is gained or lost.
//	
//	//Initialize Variable
//	float Mass = 0;
//
//	//Interate though Pixels and summate their mass
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion"
//	//Waaah! Waluigi!  -Mabel Suggestion
//	for (int i = 0; i < PartGrid.Num(); i++)
//	{
//		Mass += PartGrid.ValueAtIndex(i).Part->GetMass();
//	}
//
//	//The horrible, most devious, most destructive, COMMENTED OUT DEBUG!!!! -Mabel Suggestion
//	//UE_LOG(LogTemp, Warning, TEXT("other mass = %f"), Mass);
//
//	//Now, I may understand what's going on here, but you still should comment it -Mabel Suggestion
//	return Mass == 0 ? 1 : Mass;
//}
//
////Gets the PartGrid Map
//TGridMap<FPartInstanceData> UPartGridComponent::GetPartGrid()
//{
//	return PartGrid;
//}
//
////Comment -Mabel Suggestion
//const float UPartGridComponent::GetPartGridScale()
//{
//	return GridScale;
//}
//
////Comment -Mabel Suggestion
//const FIntBoxBounds UPartGridComponent::GetPartGridBounds()
//{
//	return GridBounds;
//}
//
////Comment -Mabel Suggestion
//void UPartGridComponent::UpdateMaterials(FIntPoint Location, TSubclassOf<UPart> PartType)
//{
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" -Mabel Suggestion
//	for (int i = -1; i < 2; i++)
//	{
//		//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" -Mabel Suggestion
//		for (int j = -1; j < 2; j++)
//		{
//
//			//You could probably combine a couple of these if statements. -Mabel Suggestion
//			if (FIntPoint(i + Location.X, j + Location.Y) != Location)
//			{
//				if (PartGrid.Contains(FIntPoint(i + Location.X, j + Location.Y)))
//				{
//					if (PartGrid.Contains(FIntPoint(Location)) && PartGrid.Find(FIntPoint(i + Location.X, j + Location.Y))->Part->GetClass() == PartType)
//					{
//						PartGrid.FindRef(FIntPoint(i + Location.X, j + Location.Y)).SetAdjacencyIndex(UFunctionLibrary::GetBitNumberFromLocation(FIntPoint(i * -1, j * -1)) + PartGrid.FindRef(FIntPoint(i + Location.X, j + Location.Y)).GetAdjacencyIndex());
//					}
//					else if (PartGrid.Find(FIntPoint(i + Location.X, j + Location.Y))->Part->GetClass() == PartType)
//					{
//						PartGrid.FindRef(FIntPoint(i + Location.X, j + Location.Y)).SetAdjacencyIndex(PartGrid.FindRef(FIntPoint(i + Location.X, j + Location.Y)).GetAdjacencyIndex() - UFunctionLibrary::GetBitNumberFromLocation(FIntPoint(i * -1, j * -1)));
//					}
//				}
//			}
//		}
//	}
//}
//
////Detect if a Part can fit in the PartGrid
//bool const UPartGridComponent::CanShapeFit(FIntPoint Loc, TSet<FIntPoint> DesiredShape)
//{
//	//There's a clever solution that replaces all this iteration. I do not know what it is. But in any case, this is 
//	//slightly oof for larger parts. -Mabel Suggestion
//	//Iterate through desired shape
//	//"Iterator should have a name that tells what it actualy is and what its iterating through - Liam Suggestion" -Mabel Suggestion
//	for (FIntPoint PixelToCheck : DesiredShape)
//	{
//		//Cheak if there is already a pixel in the grid
//		if (PartGrid.Contains(Loc + PixelToCheck))
//		{
//			return false;
//		}
//	}
//	return true;
//}
//
////Returns true if StartPoint and EndPoint are connected via PartGrid
//bool UPartGridComponent::PointsConnected(FIntPoint StartPoint, FIntPoint EndPoint, bool TestForFunctionality)
//{
//	if (TestForFunctionality)
//	{
//		return PartGrid.PointsConnected(StartPoint, EndPoint, IsPixelFunctional);
//	}
//	else
//	{
//		return PartGrid.PointsConnected(StartPoint, EndPoint, AlwaysConnect<FPartInstanceData>);
//	}
//}
//
////Comment -Mabel Suggestion
//TSet<FIntPoint> UPartGridComponent::FindConnectedShape(TSet<FIntPoint> Shape, TGridMap<FPartInstanceData> ConnectedPartsMap, bool CheckFunctionality)
//{
//
//	//New shape will return the entire connected shape, indcluding the starting shape
//	TSet<FIntPoint> NewShape = Shape;
//
//	//Yup, yup, this is bad, use better for loop instead of magic numbering, better iterator name, no auto... yup.
//	for (auto& i : Shape)
//	{
//		//Repeated code should be put in a loop or a function.
//		//If the new shape does NOT contain the checked location
//		if (!NewShape.Contains(FIntPoint(i.X + 1, i.Y)))
//		{
//			//And the connected parts ARE at that location
//			if (ConnectedPartsMap.Contains(FIntPoint(i.X + 1, i.Y)))
//			{
//				if (CheckFunctionality)
//				{
//					//And the pixel at that location is functional
//
//					if (IsValid(ConnectedPartsMap.Find(FIntPoint(i.X + 1, i.Y))->Part))
//					{
//						UE_LOG(LogTemp, Warning, TEXT("the part is valid"))
//						if (ConnectedPartsMap.Find(FIntPoint(i.X + 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X + 1, i.Y)))
//						{
//							//Add that location to the new shape, because it is connected
//							NewShape.Emplace(FIntPoint(i.X + 1, i.Y));
//						}
//					}
//					else
//					{
//						UE_LOG(LogTemp, Warning, TEXT("why is the part not valid? I do not have understanding"));
//					}
//				}
//				else
//				{
//					NewShape.Emplace(FIntPoint(i.X + 1, i.Y));
//				}
//			}
//		}
//
//		//Do the same thing done for X + 1 for X - 1
//		if (!NewShape.Contains(FIntPoint(i.X - 1, i.Y)))
//		{
//			if (ConnectedPartsMap.Contains(FIntPoint(i.X - 1, i.Y)))
//			{
//				if (CheckFunctionality)
//				{
//					if (ConnectedPartsMap.Find(FIntPoint(i.X - 1, i.Y))->Part->IsPixelFunctional(FIntPoint(i.X - 1, i.Y)))
//					{
//						NewShape.Emplace(FIntPoint(i.X - 1, i.Y));
//					}
//				}
//				else
//				{
//					NewShape.Emplace(FIntPoint(i.X - 1, i.Y));
//				}
//			}
//		}
//
//		//Do the same thing done for X + 1 for Y + 1
//		if (!NewShape.Contains(FIntPoint(i.X, i.Y + 1)))
//		{
//			if (ConnectedPartsMap.Contains(FIntPoint(i.X, i.Y + 1)))
//			{
//				if (CheckFunctionality)
//				{
//					if (ConnectedPartsMap.Find(FIntPoint(i.X, i.Y + 1))->Part->IsPixelFunctional(FIntPoint(i.X, i.Y + 1)))
//					{
//						NewShape.Emplace(FIntPoint(i.X, i.Y + 1));
//					}
//				}
//				else
//				{
//					NewShape.Emplace(FIntPoint(i.X, i.Y + 1));
//				}
//			}
//		}
//
//		//Do the same thing done for X + 1 for Y - 1
//		if (!NewShape.Contains(FIntPoint(i.X, i.Y - 1)))
//		{
//			if (ConnectedPartsMap.Contains(FIntPoint(i.X, i.Y - 1)))
//			{
//				if (CheckFunctionality)
//				{
//					if (ConnectedPartsMap.Find(FIntPoint(i.X, i.Y - 1))->Part->IsPixelFunctional(FIntPoint(i.X, i.Y - 1)))
//					{
//						NewShape.Emplace(FIntPoint(i.X, i.Y - 1));
//					}
//				}
//				else
//				{
//					NewShape.Emplace(FIntPoint(i.X, i.Y - 1));
//				}
//			}
//		}
//	}
//
//	//If the new shape has changed at all
//	if (NewShape.Difference(Shape).Num() != 0)
//	{
//		//This could be replaced with: return FindConnectedShape(NewShape, ConnectedPartsMap, CheckFunctionality); - Liam Suggestion
//		//Continue to check for connections by calling the function recursively.
//		NewShape = FindConnectedShape(NewShape, ConnectedPartsMap, CheckFunctionality);
//	}
//
//	//Once everything has figured itself out, return the New Shape
//	return NewShape;
//}
//
////Comment -Mabel Suggestion
//bool UPartGridComponent::IsPixelFunctional(FPartInstanceData PixelValue, FIntPoint Loc)
//{
//	return PixelValue.Part->IsPixelFunctional(Loc);
//}
