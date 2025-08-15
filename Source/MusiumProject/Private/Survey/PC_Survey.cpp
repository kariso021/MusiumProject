// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Survey.h"
#include "Blueprint/UserWidget.h"

void APC_Survey::BeginPlay()
{
    // 1. �ݵ�� �θ� Ŭ������ BeginPlay�� ���� ȣ���մϴ�.
    Super::BeginPlay();

    // 2. ����� ������ SurveyWidgetClass ������ ���� Ŭ������ ��ȿ�ϰ� �����Ǿ����� Ȯ���մϴ�.
    // (�����Ϳ��� �������� ������ �� �κ��� ������� �ʽ��ϴ�.)
    if (SurveyWidgetClass)
    {
        // 3. ������ Ŭ������ ���� �ν��Ͻ��� �����մϴ�. 'this'�� �� PlayerController�� ������ ���������� �ǹ��մϴ�.
        SurveyWidgetInstance = CreateWidget<UUserWidget>(this, SurveyWidgetClass);

        if (SurveyWidgetInstance)
        {
            // 4. ������ ������ ȭ��(Viewport)�� �߰��մϴ�.
            SurveyWidgetInstance->AddToViewport();

            // 5. ���콺 Ŀ���� ���̵��� �����մϴ�.
            bShowMouseCursor = true;

            // 6. �Է� ��带 'UI ����'���� �����մϴ�.
            // �̷��� �ؾ� ���� ����� Ŭ������ �ʰ� UI�� ������ �� �ֽ��ϴ�.
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(SurveyWidgetInstance->TakeWidget()); // ��Ŀ���� �츮 ������ �ݴϴ�.
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // ���콺�� ����Ʈ ������ ���� �� �ְ� �մϴ�.

            SetInputMode(InputModeData);
        }
    }
}
