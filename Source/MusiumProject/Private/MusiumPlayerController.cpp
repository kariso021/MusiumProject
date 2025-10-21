// Fill out your copyright notice in the Description page of Project Settings.


#include "MusiumPlayerController.h"
#include "InputCoreTypes.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/GameViewportClient.h"
#include "Widget/SPauseMenuWidget.h"
#include <Kismet/GameplayStatics.h>

AMusiumPlayerController::AMusiumPlayerController()
{
	InteractionUIComp = CreateDefaultSubobject<UInteractionUIComponent>(
		TEXT("InteractionUIComp"));
}

void AMusiumPlayerController::ResumeGame()
{
    if (PauseMenuWidgetInstance)
    {
        PauseMenuWidgetInstance->RemoveFromParent();

        UGameplayStatics::SetGamePaused(GetWorld(), false);

        bShowMouseCursor = false;

        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
        PauseMenuWidgetInstance = nullptr;
    }
}

void AMusiumPlayerController::GoToSurveyExitLevel()
{
    if (UWorld* World = GetWorld())
    {
        UGameplayStatics::OpenLevel(World, TEXT("ExitSurveyMap"));
    }
}

void AMusiumPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
    if (InputComponent)
    {
        InputComponent->BindAction("PauseGame", IE_Pressed, this, &AMusiumPlayerController::OnPauseGame);
    }
}


void AMusiumPlayerController::OnPauseGame()
{
    if (PauseMenuWidgetInstance)
    {
        ResumeGame();
        return;
    }

    if (PauseMenuWidgetClass)
    {
        PauseMenuWidgetInstance = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
        if (PauseMenuWidgetInstance)
        {
            PauseMenuWidgetInstance->AddToViewport();
            UGameplayStatics::SetGamePaused(GetWorld(), true);
            bShowMouseCursor = true;
            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스가 뷰포트 밖으로 나갈 수 있도록 설정
            SetInputMode(InputMode);
        }
    }
}
