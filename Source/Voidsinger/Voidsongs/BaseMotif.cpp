// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMotif.h"

//Shouldnt be needed. - Liam Suggestion
/**
 * Sets default values for this object's properties
 */
UBaseMotif::UBaseMotif()
{
	//Load the saved values in the configuration files into this class.
	LoadConfig();
}