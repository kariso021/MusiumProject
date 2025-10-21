// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InteractionUIComponent.h"
#include "MusiumPlayerController.generated.h"

/**
 * 
 */

class UUserWdiget;

UCLASS()
class AMusiumPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMusiumPlayerController();

	UInteractionUIComponent* GetInteractionUIComponent() const { return InteractionUIComp; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UInteractionUIComponent* InteractionUIComp;

public:
	UFUNCTION(BlueprintCallable, Category = "Game|Pause")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Game|Level")
	void GoToSurveyExitLevel();

protected:
    virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = "Game|Pause")
	void OnPauseGame();

private:


	//Pause Menu ฐüทร
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY()
	UUserWidget* PauseMenuWidgetInstance;



	
};
