// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceType.generated.h"


/* \/ =============== \/ *\
|  \/ [EResourceType] \/  |
\* \/ =============== \/ */

/*
 * Stores the kind of resource something is
 */
UENUM(BlueprintType, meta = (BitMask))
enum class EResourceType : uint8
{
	Fuel			UMETA(DisplayName = "Fuel", Tooltip = "Fuels parts"),
	Hydrogen		UMETA(DisplayName = "Hydrogen", Tooltip = "I don't know what Hydrogen does right now"),
	InertGas		UMETA(DisplayName = "InertGas", Tooltip = "idk what this does either"),
	Coolant			UMETA(DisplayName = "Coolant", Tooltip = "Keeps parts from overheating"),
	Electricity		UMETA(DisplayName = "Electricity", Tooltip = "Powers parts"),
	Ammo			UMETA(DisplayName = "Ammo", Tooltip = "Allows parts to shoot projectiles"),
};

/* /\ =============== /\ *\
|  /\ [EResourceType] /\  |
\* /\ =============== /\ */

/* \/ =============== \/ *\
|  \/ [FResourceCall] \/  |
\* \/ =============== \/ */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResourceRequestCompleted);

/*
 * Stores the resources used and created by this resource request, as well as the priority of the request. A resource request is a request for resources to be used and added.
 */
USTRUCT(BlueprintType)
struct VOIDSINGER_API FResourceRequest
{
	GENERATED_BODY()

public:

	/* ------------------ *\
	\* \/ Constructors \/ */

	/*
	 * Default constructor
	 */
	FResourceRequest()
	{
		FResourceRequest(0, TMap<EResourceType, float>(), TMap<EResourceType, float>());
	}

	/*
	 * Constructor taking in priority and resources 
	 * 
	 * @param PriorityOfRequest - The priority of this resource request
	 * @param ResourceTypesToUsed - A map of resource types to the amount used of each
	 * @param ResourceTypesToCreated - A map of resource types to the amount created of each
	 */
	FResourceRequest(int PriorityOfRequest, TMap<EResourceType, float> ResourceTypesToUsed, TMap<EResourceType, float> ResourceTypesToCreated)
	{
		Priority = PriorityOfRequest;
		ResourceTypesToAmountUsed = ResourceTypesToUsed;
		ResourceTypesToAmountCreated = ResourceTypesToCreated;
	}

	/* /\ Constructors /\ *\
	\* ------------------ */

	/* ------------------------- *\
	\* \/ Resource Management \/ */

	//The priority of this resource call
	UPROPERTY(EditAnywhere, Category = "Resource Management")
	int Priority{ 0 };

	//A map of resource types to the amount used of each
	UPROPERTY(EditAnywhere, Category = "Resource Management")
	TMap<EResourceType, float> ResourceTypesToAmountUsed{ TMap<EResourceType, float>() };

	//A map of resource types to the amount created of each
	UPROPERTY(EditAnywhere, Category = "Resource Management")
	TMap<EResourceType, float> ResourceTypesToAmountCreated{ TMap<EResourceType, float>() };

	//A delegate that is called when the resource call is completed
	UPROPERTY()
	FResourceRequestCompleted OnResourceRequestCompleted;

	/* /\ Resource Management /\ *\
	\* ------------------------- */

};

/* /\ =============== /\ *\
|  /\ [FResourceCall] /\  |
\* /\ =============== /\ */