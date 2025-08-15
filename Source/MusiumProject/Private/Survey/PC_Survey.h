// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Survey.generated.h"

/**
 * 
 */
UCLASS()
class APC_Survey : public APlayerController
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Survey")
	TSubclassOf<UUserWidget> SurveyWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> SurveyWidgetInstance;

	virtual void BeginPlay() override;



};
