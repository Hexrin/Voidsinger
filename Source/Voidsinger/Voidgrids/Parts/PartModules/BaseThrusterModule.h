// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivatablePartModule.h"
#include "BaseThrusterModule.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseThrusterModule : public UActivatablePartModule
{
	GENERATED_BODY()

protected:
	//The force of the impulse applied by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0"))
	float ThrustForce{ 100 };

	//The direction of the impulse applyed by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPreserveRatio))
	FVector2D ThrustDirection{ FVector2D(1,0) };

	//The location of the impulse applyed by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint ThrustLocation{ FIntPoint::ZeroValue };

	//Whether or not to apply the voidsong effect to the thrust location while active.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bApplyVoidsongEffect{ true };
};
