// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivatablePartModule.h"
#include "ThrusterModule.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UThrusterModule : public UActivatablePartModule
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintPure)
	static float TimeToLinearVelocity(AVoidgrid* Target, FVector2D Velocity);

	UFUNCTION(BlueprintPure)
	static float TimeToLocation(AVoidgrid* Target, FVector2D Location);

	UFUNCTION(BlueprintPure)
	static float TimeToAngularVelocity(AVoidgrid* Target, float Velocity);

	UFUNCTION(BlueprintPure)
	static float TimeToOrientation(AVoidgrid* Target, float Orientation);

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


	/**
	 * Called when the owning part is activated and calls the functionality of this part.
	 * Applies an impulse to the voidgrid in an atempt to get it to move in teh vector dirction and face in the roatation direction.
	 */
	virtual void OnActivate_Implementation(const FPartActivationData Data) override;

	/* ------------------- *\
	\* \/ Functionality \/ */

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


private:
	static TMap<AVoidgrid*, TSet<UThrusterModule*>>& GetThrusterModules();
};
