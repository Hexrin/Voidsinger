#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "NPCShipData.h"
#include "NPCShip.generated.h"

/**
 * A ship controlable by an AI.
 * Handels instancation and AI input.
 */
UCLASS()
class VOIDSINGER_API ANPCShip : public AShip
{
	GENERATED_BODY()


public:
	//Inilizes the camrea and posesses the player.
	APlayerShip();

protected:
	//Stores the information relating to the type of this ship.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNPCShipData* ShipTypeData;

};
