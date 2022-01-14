//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Voidsinger/Voidsongs/BaseVoidsong.h"
//#include "Voidsinger/Voidsongs/BaseNounVoidsong.h"
//#include "Voidsinger/Voidsongs/BaseWhoVoidsong.h"
//#include "BaseVerbVoidsong.generated.h"
//
///**
// * 
// */
//
//class UPart;
//
//UCLASS()
//class VOIDSINGER_API UBaseVerbVoidsong : public UBaseVoidsong
//{
//	GENERATED_BODY()
//	
//public:
//
//	/*Voidsong Functions*\
//	\*------------------*/
//
//	/*
//	* Why does this function exist? It sets variables that arent used anywhere and is only called where
//	* it could be replaced with Activate(so long as you make Activate blueprint callable).
//	* - Liam Suggestion
//	*/
//	//Pre-activate calls activate, but it also sets ActorHit, WorldLocation and effectiveness. It's convenient.
//	UFUNCTION(BlueprintCallable)
//	void PreActivate(AActor* NewActorHit, AActor* NewActorThatActivated, TArray<UPart*> NewPartsHit, FVector NewLocationCalledFrom, FVector NewWorldLocation, float NewEffectiveness, float NewDuration);
//
//	/*
//	* Should be blueprintcallable so it can replace PreActivate
//	* 
//	* Weird Parameter Names.
//	* The prefiex New doesn't realy make sense in the context of this functions parameters. What is Old?
//	* NewActorThatActivated should be replaced with Instigator to maintain consitancy with ue5
//	* NewWorldLocation of what?
//	* 
//	* Too many unnessary parmeters.
//	* It should only pass a UPart* Instigator, a FHitResult Hit, and a float Magnitude.
//	* The rest of the infromation could be derived from these three.
//	* 
//	* Weird Name.
//	* Not sure activate is the right word. Maybe PlayAt()
//	* Activate conflicts with the UPart::Activate() which is confusing because they are not similar functions.
//	* - Liam Suggestion
//	*/
//	//This will do the effect of the voidsong. It's called within Activate. Needs to be implemented for each Voidsong.
//	UFUNCTION(BlueprintImplementableEvent)
//	void Activate(AActor* NewActorHit, AActor* NewActorThatActivated, const TArray<UPart*>& NewPartsHit, FVector NewLocationCalledFrom, FVector NewWorldLocation, float NewEffectiveness, float NewDuration);
//
//	//This will undo the effect of the voidsong. It's also called within activate, after a delay for the duration. Needs to be implemented for each Voidsong. 
//	UFUNCTION(BlueprintImplementableEvent)
//	void Deactivate();
//
//	/*Getters*\
//	\--------*/
//
//protected:
//
//	/*Instanced  Variables*\
//	\*--------------------*/
//
//	// All of the varaiibles in this class are never used so they should be deleted - Liam Suggestion
//	UPROPERTY(BlueprintReadOnly)
//	AActor* ActorHit;
//
//	UPROPERTY(BlueprintReadOnly)
//	AActor* ActorThatActivated;
//
//	UPROPERTY(BlueprintReadOnly)
//	TArray<UPart*> PartsHit;
//
//	UPROPERTY(BlueprintReadOnly)
//	FVector LocationCalledFrom;
//
//	UPROPERTY(BlueprintReadOnly)
//	FVector WorldLocation;
//
//	UPROPERTY(BlueprintReadOnly)
//	float Effectiveness;
//
//	UPROPERTY(BlueprintReadOnly)
//	float FullVoidsongLength;
//
//};
