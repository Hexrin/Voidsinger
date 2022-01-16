#include "NPCShip.h"


ANPCShip::ANPCShip()
{
	if (IsValid(ShipTypeData))
	{
		SetPixelMold(ShipTypeData->Mold);
	}
}

/**
 * Gets the faction of the Voidgrid.
 *
 * @return The faction of the Voidgrid.
 */
EFaction ANPCShip::GetFaction() const
{
	if (IsValid(ShipTypeData))
	{
		return ShipTypeData->Faction;
	}

	//Return default faction.
	return (EFaction)0;
}