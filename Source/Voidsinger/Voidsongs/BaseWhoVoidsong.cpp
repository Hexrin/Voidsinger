// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidsongs/BaseWhoVoidsong.h"

TArray<AActor*> UBaseWhoVoidsong::GetWhos()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Who, OutActors);
	return OutActors;
}