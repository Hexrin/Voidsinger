// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Parts/CorePart.h"

UCorePart::UCorePart()
{
	DesiredShape.Emplace(FIntPoint(2, 2));
	DesiredShape.Emplace(FIntPoint(1, 2));
	DesiredShape.Emplace(FIntPoint(0, 2));
	DesiredShape.Emplace(FIntPoint(-1, 2));
	DesiredShape.Emplace(FIntPoint(-2, 2));
	DesiredShape.Emplace(FIntPoint(2, 1));
	DesiredShape.Emplace(FIntPoint(1, 1));
	DesiredShape.Emplace(FIntPoint(0, 1));
	DesiredShape.Emplace(FIntPoint(-1, 1));
	DesiredShape.Emplace(FIntPoint(-2, 1));
	DesiredShape.Emplace(FIntPoint(2, 0));
	DesiredShape.Emplace(FIntPoint(1, 0));
	DesiredShape.Emplace(FIntPoint(-1, 0));
	DesiredShape.Emplace(FIntPoint(-2, 0));
	DesiredShape.Emplace(FIntPoint(2, -1));
	DesiredShape.Emplace(FIntPoint(1, -1));
	DesiredShape.Emplace(FIntPoint(0, -1));
	DesiredShape.Emplace(FIntPoint(-1, -1));
	DesiredShape.Emplace(FIntPoint(-2, -1));
	DesiredShape.Emplace(FIntPoint(2, -2));
	DesiredShape.Emplace(FIntPoint(1, -2));
	DesiredShape.Emplace(FIntPoint(0, -2));
	DesiredShape.Emplace(FIntPoint(-1, -2));
	DesiredShape.Emplace(FIntPoint(-2, -2));

}