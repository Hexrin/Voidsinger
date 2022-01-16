#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "NPCShipData.h"
#include "NPCShip.generated.h"

/**
 * A ship controlable by an AI.
 * Handels ship type and AI input.
 */
UCLASS()
class VOIDSINGER_API ANPCShip : public AShip
{
	GENERATED_BODY()


public:
	/**
	 * Initilizes this using ShipTypeData.
	 */
	ANPCShip();

	/**
	 * Gets the faction of the Voidgrid.
	 *
	 * @return The faction of the Voidgrid.
	 */
	virtual EFaction GetFaction() const override;

protected:
	//Stores the information relating to the type of this ship.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	UNPCShipData* ShipTypeData;

};
