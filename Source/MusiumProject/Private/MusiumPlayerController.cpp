// Fill out your copyright notice in the Description page of Project Settings.


#include "MusiumPlayerController.h"

AMusiumPlayerController::AMusiumPlayerController()
{
	InteractionUIComp = CreateDefaultSubobject<UInteractionUIComponent>(
		TEXT("InteractionUIComp"));
}
