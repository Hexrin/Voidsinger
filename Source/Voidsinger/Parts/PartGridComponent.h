// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Voidsinger/SaveShip.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "PartGridComponent.generated.h"

class UBasePart;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class VOIDSINGER_API UPartGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPartGridComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, TEnumAsByte<EPartRotation> Rotation, bool bAlwaysPlace);

	UFUNCTION(BlueprintCallable)
	bool RemovePart(FIntPoint Location);

	UFUNCTION(BlueprintCallable)
	bool DestroyPixel(FIntPoint Location, class UBasePart*& DamagedPart);

	UFUNCTION(BlueprintCallable)
	void BuildShip(TArray<FSavePartInfo> Parts);

	UFUNCTION(BlueprintCallable)
	void SaveShip(FString ShipName);

	UFUNCTION(BlueprintCallable)
	void LoadSavedShip(FString ShipName);

	UFUNCTION(BlueprintPure)
	const FVector2D GetCenterOfMass();

	UFUNCTION(BlueprintPure)
	const float GetMass();

	UFUNCTION(BlueprintPure)
	TMap<FIntPoint, UBasePart*> GetPartGrid();

private:
	UPROPERTY()
	TMap<FIntPoint, UBasePart*> PartGrid;

	UPROPERTY()
	TMap<FIntPoint, UActorComponent*> MeshGrid;

	UPROPERTY()
	FArrayBounds GridBounds;

	UPROPERTY(EditAnywhere)
	FIntPoint GridSize;

	UPROPERTY(EditAnywhere)
	float GridScale; 

	UFUNCTION()
	bool const CanShapeFit(FIntPoint Loc, TArray<FIntPoint> DesiredShape);
};
