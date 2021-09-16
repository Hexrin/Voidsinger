// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	//---Ship Creation---

	UFUNCTION(BlueprintCallable)
	bool AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, TEnumAsByte<EPartRotation> Rotation, bool bAlwaysPlace = false);
	bool AddPart(TArray<FIntPoint> PartialPartShape, TSubclassOf<UBasePart> PartType, FIntPoint Location, TEnumAsByte<EPartRotation> Rotation, bool bAlwaysPlace = false);

	UFUNCTION(BlueprintCallable)
	bool RemovePart(FIntPoint Location);



	//---Destruction---

	UFUNCTION(BlueprintCallable)
	bool DestroyPixel(FIntPoint Location, class UBasePart*& DamagedPart);
	bool DestroyPixel(FIntPoint Location);

	UFUNCTION(BlueprintCallable)
	void ApplyHeatAtLocation(FVector WorldLocation, float HeatToApply = 1);
	void ApplyHeatAtLocation(FIntPoint RelativeLocation, float HeatToApply = 1);



	//---Save Ship---

	UFUNCTION(BlueprintCallable)
	void BuildShip(TArray<FSavePartInfo> Parts);

	UFUNCTION(BlueprintCallable)
	void SaveShip(FString ShipName);

	UFUNCTION(BlueprintCallable)
	void LoadSavedShip(FString ShipName);



	//---Getters---

	UFUNCTION(BlueprintPure)
	const FVector2D GetCenterOfMass();

	UFUNCTION(BlueprintPure)
	const float GetMass();

	UFUNCTION(BlueprintPure)
	TMap<FIntPoint, FPartData> GetPartGrid();

private:
	UPROPERTY()
	TMap<FIntPoint, FPartData> PartGrid;

	UPROPERTY()
	FArrayBounds GridBounds;

	UPROPERTY(EditAnywhere)
	FIntPoint GridSize;

	UPROPERTY(EditAnywhere)
	float GridScale; 

	UPROPERTY(EditAnywhere)
	class UStaticMesh* PixelMesh;

	UFUNCTION()
	bool const CanShapeFit(FIntPoint Loc, TArray<FIntPoint> DesiredShape);


};
