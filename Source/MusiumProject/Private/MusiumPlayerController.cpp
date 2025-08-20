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
    if (PauseMenuWidget.IsValid() && GEngine && GEngine->GameViewport)
    {
        // 1. 뷰포트에서 위젯 제거
        GEngine->GameViewport->RemoveViewportWidgetContent(PauseMenuWidget.ToSharedRef());

        // 2. 게임 재개
        UGameplayStatics::SetGamePaused(GetWorld(), false);

        // 3. 마우스 커서 숨기기
        bShowMouseCursor = false;

        // 4. 입력 모드를 '게임 전용'으로 변경
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);

        // 5. 위젯 포인터 초기화
        PauseMenuWidget.Reset();
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
    // 이미 메뉴가 열려있다면 닫음(게임 재개).
    if (PauseMenuWidget.IsValid())
    {
        ResumeGame();
        return;
    }

    // --- 게임 일시정지 및 메뉴 생성 ---

    // 1. Slate 위젯을 생성하고 포인터에 할당
    SAssignNew(PauseMenuWidget, SPauseMenuWidget)
        .OwningPlayerController(this); // 위젯에 컨트롤러 자신을 전달

    if (PauseMenuWidget.IsValid())
    {
        // 2. 뷰포트에 위젯 추가
        GEngine->GameViewport->AddViewportWidgetContent(
            SNew(SWeakWidget).PossiblyNullContent(PauseMenuWidget.ToSharedRef())
        );

        // 3. 게임 일시정지
        UGameplayStatics::SetGamePaused(GetWorld(), true);

        // 4. 마우스 커서 보이기
        bShowMouseCursor = true;

        // 5. 입력 모드를 '게임 및 UI'로 변경
        FInputModeGameAndUI InputMode;
        SetInputMode(InputMode);
    }
}
