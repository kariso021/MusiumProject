// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Survey.h"
#include "Blueprint/UserWidget.h"

void APC_Survey::BeginPlay()
{

    Super::BeginPlay();


    if (SurveyWidgetClass)
    {

        SurveyWidgetInstance = CreateWidget<UUserWidget>(this, SurveyWidgetClass);

        if (SurveyWidgetInstance)
        {

            SurveyWidgetInstance->AddToViewport();


            bShowMouseCursor = true;

  
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(SurveyWidgetInstance->TakeWidget()); 
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); 

            SetInputMode(InputModeData);
        }
    }
}
