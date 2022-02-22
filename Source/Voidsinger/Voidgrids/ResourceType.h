// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/* \/ =============== \/ *\
|  \/ [Resource Type] \/  |
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
|  /\ [Resource Type] /\  |
\* /\ =============== /\ */