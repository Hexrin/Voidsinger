// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePart.h"
#include "Voidsinger/Ships/ShipMovementComponent.h"
#include "BaseThrusterPart.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseThrusterPart : public UBasePart
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintPure)
	TArray<FThrusterData> GetThrustSources();

	UFUNCTION(BlueprintCallable)
	bool ThrustInDirection(float ThrustForce, float ThrustDirection);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FThrusterData> ThrustSources;
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FThrusterData
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrustForce = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D RelativeThrustLoaction = FVector2D();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RelativeThrustRotation = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBaseThrusterPart* ThrusterRef = nullptr;

	FThrusterData(float Force, FVector2D RelativeLocation, float RelativeRotation, class UBaseThrusterPart* Thruster)
	{
		ThrustForce = Force;
		RelativeThrustLoaction = RelativeLocation;
		RelativeThrustRotation = RelativeRotation;
		ThrusterRef = Thruster;
	}
};
