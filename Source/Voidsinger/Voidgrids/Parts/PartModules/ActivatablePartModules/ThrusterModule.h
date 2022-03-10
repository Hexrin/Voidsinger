// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivatablePartModule.h"
#include "ThrusterModule.generated.h"

class UThrustManager;

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UThrusterModule : public UActivatablePartModule
{
	GENERATED_BODY()

	friend UThrustManager;

public:

	/**
	 * Initializes the part module's protected and private variables
	 *
	 * @param OwningPart - The part that this module is being initialized from
	 */
	virtual void InitializeVariables(UPart* OwningPart) override;

protected:
	//The force of the impulse applied by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0"))
	float ThrustForce{ 100 };

	//The direction of the impulse applied by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPreserveRatio))
	FVector2D ThrustDirection{ FVector2D(1,0) };

	//The location of the impulse applied by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint ThrustLocation{ FIntPoint::ZeroValue };

	//Whether or not to apply the voidsong effect to the thrust location while active.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bApplyVoidsongEffect{ true };


protected:
	/**
	 * Called when the owning part is activated and calls the functionality of this part.
	 * Applies an impulse to the voidgrid in an attempt to get it to move in the vector direction and face in the rotation direction.
	 */
	virtual void OnActivate_Implementation(const FPartActivationData Data) override;

	/* ------------------- *\
	\* \/ Functionality \/ */

protected:
	/**
	 * Updates voidgrid thrust capabilities.
	 */
	virtual void FunctionalityRestored() override;

	/**
	 * Updates voidgrid thrust capabilities.
	 */
	virtual void FunctionalityLost() override;

	/* /\ Functionality /\ *\
	\* ------------------- */
};
