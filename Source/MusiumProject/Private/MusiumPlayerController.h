// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InteractionUIComponent.h"
#include "MusiumPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AMusiumPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMusiumPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UInteractionUIComponent* InteractionUIComp;
	
};
