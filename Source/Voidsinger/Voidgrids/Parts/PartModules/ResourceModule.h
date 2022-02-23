// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModule.h"
#include "Voidsinger/Voidgrids/ResourceType.h"
#include "ResourceModule.generated.h"

/**
 * Allows a part to consume and create resources
 */
UCLASS()
class VOIDSINGER_API UResourceModule : public UActivatablePartModule
{
	GENERATED_BODY()
	
private:

	virtual void OnActivate_Implementation(const FPartActivationData Data) override;

protected:

	//The resources that this part will use up and how much of each it will use
	UPROPERTY()
	TMap<EResourceType, float> ResourcesToUse;

	//The resourcses that this part will add and how much of each it will add
	UPROPERTY()
	TMap<EResourceType, float> ResourcesToAdd;
	
};
