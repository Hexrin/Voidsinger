// Fill out your copyright notice in the Description page of Project Settings.

#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "ResourceStorageModule.h"

/* \/ ====================== \/ *\
|  \/ UResourceStorageModule \/  |
\* \/ ====================== \/ */

/* ------------------- *\
\* \/ Functionality \/ */

/**
 * Adds storage capacity when the part is functional.
 * 
 * @param bApplyChangeEffect - Whether to apply the effects of this change
 */
void UResourceStorageModule::FunctionalityRestored(bool bApplyChangeEffect)
{
	GetVoidgrid()->AddResourceStorageCapacity(ResourceTypesToCapacityAmountsIncreased);
}

/**
 * Removes the storage capacity this added when the part is not functional.
 * 
 * @param bApplyChangeEffect - Whether to apply the effects of this change
 */
void UResourceStorageModule::FunctionalityLost(bool bApplyChangeEffect)
{
	GetVoidgrid()->RemoveResourceStorageCapacity(ResourceTypesToCapacityAmountsIncreased);
}

/* /\ Functionality /\ *\
\* ------------------- */

/* /\ ====================== /\ *\
|  /\ UResourceStorageModule /\  |
\* /\ ====================== /\ */