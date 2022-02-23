// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidgrids/Parts/PartModules/ResourceModule.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

void UResourceModule::OnActivate_Implementation(const FPartActivationData Data)
{
	// \/ Adjust the amount of resources to use with effectiveness \/ /
	
	//Tracks the resources to use and the amount to use of each, adjusted with effectiveness
	TMap<EResourceType, float> ResourcesToUseAdjusted;

	if (!Data.Effectiveness == 1)
	{
		for (TPair<EResourceType, float> EachUsedResource : ResourcesToUse)
		{
			ResourcesToUseAdjusted.Emplace(EachUsedResource.Key, EachUsedResource.Value * Data.Effectiveness);
		}
	}
	else
	{
		ResourcesToUseAdjusted = ResourcesToUse;
	}

	// /\ Adjust the amount of resources to use with effectiveness /\ /

	if (Part->GetVoidgrid()->UseResources(ResourcesToUseAdjusted))
	{
		// \/ Adjust the amount of resources to add with effectiveness \/ /

		//Tracks the resources to add and the amount to add of each, adjusted with effectiveness
		TMap<EResourceType, float> ResourcesToAddAdjusted;

		if (!Data.Effectiveness == 1)
		{
			for (TPair<EResourceType, float> EachAddedResource : ResourcesToAdd)
			{
				ResourcesToAddAdjusted.Emplace(EachAddedResource.Key, EachAddedResource.Value * Data.Effectiveness);
			}
		}
		else
		{
			ResourcesToAddAdjusted = ResourcesToAdd;
		}

		// /\ Adjust the amount of resources to add with effectiveness /\ /

		Part->GetVoidgrid()->AddResources(ResourcesToAddAdjusted);
	}
}