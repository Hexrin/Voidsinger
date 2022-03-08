// Fill out your copyright notice in the Description page of Project Settings.


#include "OrderVerbMotif.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

/* \/ =============== \/ *\
|  \/ UOrderVerbMotif \/  |
\* \/ =============== \/ */

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
 * Override of apply effect to apply the effect of this Motif
 *
 * @param Instigator - The object that activated this Voidsong
 * @param Hit - The hit result information
 * @param Effectiveness - The effectiveness of the activation
 */
void UOrderVerbMotif::ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness)
{
	AVoidgrid* HitVoidgrid = Cast<AVoidgrid>(Hit.GetActor());

	TArray<FIntPoint> PixelsInRadius = GetHealablePixelsInRadius(HitVoidgrid, HitVoidgrid->TransformWorldToGrid(Hit.Location), HealingRadius);
	int NumberOfPixelsToRepair = PixelsInRadius.Num() * Effectiveness;

	if (!PixelsInRadius.IsEmpty() && NumberOfPixelsToRepair == 0)
	{
		NumberOfPixelsToRepair = 1;
	}

	for (int EachNumberOfPixelsToRepair = 0; EachNumberOfPixelsToRepair < NumberOfPixelsToRepair; EachNumberOfPixelsToRepair++)
	{
		int RandomIndex = FMath::RandRange(0, PixelsInRadius.Num() - 1);
		HitVoidgrid->RepairPixel(PixelsInRadius[RandomIndex]);
		PixelsInRadius.RemoveAt(RandomIndex);
	}
}

/**
 * Gets all of the pixel locations on the Voidgrid given inside a given radius
 *
 * @param Voidgrid - The voidgrid to find pixels on
 * @param Center - The center of the circle
 * @param Radius - The radius
 */
TArray<FIntPoint> UOrderVerbMotif::GetHealablePixelsInRadius(AVoidgrid* Voidgrid, FIntPoint Center, float Radius)
{
	TArray<FIntPoint> Return;
	int ScanX = -Radius;
	int ScanY = -Radius;

	while (ScanY <= Radius)
	{
		if (ScanX * ScanX + ScanY * ScanY <= Radius * Radius)
		{
			if (Voidgrid->IsPixelHealable(FIntPoint(ScanX, ScanY)))
			{
				Return.Emplace(FIntPoint(ScanX, ScanY));
			}
			ScanX += 1;
		}
		else
		{
			if (ScanX > Radius)
			{
				ScanX = -Radius;
				ScanY += 1;
			}
			else
			{
				ScanX += 1;
			}
		}
	}

	return Return;
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */

/* /\ =============== /\ *\
|  /\ UOrderVerbMotif /\  |
\* /\ =============== /\ */