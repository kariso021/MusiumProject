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
        // 1. ����Ʈ���� ���� ����
        GEngine->GameViewport->RemoveViewportWidgetContent(PauseMenuWidget.ToSharedRef());

        // 2. ���� �簳
        UGameplayStatics::SetGamePaused(GetWorld(), false);

        // 3. ���콺 Ŀ�� �����
        bShowMouseCursor = false;

        // 4. �Է� ��带 '���� ����'���� ����
        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);

        // 5. ���� ������ �ʱ�ȭ
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
    // �̹� �޴��� �����ִٸ� ����(���� �簳).
    if (PauseMenuWidget.IsValid())
    {
        ResumeGame();
        return;
    }

    // --- ���� �Ͻ����� �� �޴� ���� ---

    // 1. Slate ������ �����ϰ� �����Ϳ� �Ҵ�
    SAssignNew(PauseMenuWidget, SPauseMenuWidget)
        .OwningPlayerController(this); // ������ ��Ʈ�ѷ� �ڽ��� ����

    if (PauseMenuWidget.IsValid())
    {
        // 2. ����Ʈ�� ���� �߰�
        GEngine->GameViewport->AddViewportWidgetContent(
            SNew(SWeakWidget).PossiblyNullContent(PauseMenuWidget.ToSharedRef())
        );

        // 3. ���� �Ͻ�����
        UGameplayStatics::SetGamePaused(GetWorld(), true);

        // 4. ���콺 Ŀ�� ���̱�
        bShowMouseCursor = true;

        // 5. �Է� ��带 '���� �� UI'�� ����
        FInputModeGameAndUI InputMode;
        SetInputMode(InputMode);
    }
}
