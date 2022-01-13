// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "BaseVerbVoidsong.generated.h"

 /* -------------------------- *\
 \* \/ Forward Declarations \/ */

class UBasePart;

/* /\ Foward Declarations /\ *\
\* ------------------------- */

/**
 * The BaseVerbVoidsong stores an overridable fuction on it. It is the base class for all Verb Voidsongs. When this type of Voidsong is performed, an action will be executed when the parts that were activated
 * interact with something. For example, if the Verb Voidsong "Hot" was played along with the "Laser" noun, all lasers would fire with an extra hot laser. Whatever these lasers hit will be heated up.
 */
UCLASS(Abstract, Blueprintable, config = VoidSettings)
class VOIDSINGER_API UBaseVerbVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()
	
public:

	/*Voidsong Functions*\
	\*------------------*/

	/*
	* Should be blueprintcallable so it can replace PreActivate
	* 
	* Weird Parameter Names.
	* The prefiex New doesn't realy make sense in the context of this functions parameters. What is Old?
	* NewActorThatActivated should be replaced with Instigator to maintain consitancy with ue5
	* NewWorldLocation of what?
	* 
	* Too many unnessary parmeters.
	* It should only pass a UBasePart* Instigator, a FHitResult Hit, and a float Magnitude.
	* The rest of the infromation could be derived from these three.
	* 
	* Weird Name.
	* Not sure activate is the right word. Maybe PlayAt()
	* Activate conflicts with the UBasePart::Activate() which is confusing because they are not similar functions.
	* - Liam Suggestion
	*/
	//This will do the effect of the Voidword. Needs to be implemented for each Voidsong.
	void static Play(UBasePart* Instigator, FHitResult Hit, float Effectiveness);

};
