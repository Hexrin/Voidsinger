#include "NPCShip.h"


/**
 * Initilizes this using ShipTypeData.
 */
ANPCShip::ANPCShip()
{
	SetPixelMold(ShipTypeData->Mold);
}

/**
 * Gets the faction of the Voidgrid.
 *
 * @return The faction of the Voidgrid.
 */
EFaction ANPCShip::GetFaction() const
{
	return ShipTypeData->Faction;
}