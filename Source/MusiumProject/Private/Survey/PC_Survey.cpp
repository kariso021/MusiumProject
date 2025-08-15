// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Survey.h"
#include "Blueprint/UserWidget.h"

void APC_Survey::BeginPlay()
{
    // 1. 반드시 부모 클래스의 BeginPlay를 먼저 호출합니다.
    Super::BeginPlay();

    // 2. 헤더에 선언한 SurveyWidgetClass 변수에 위젯 클래스가 유효하게 지정되었는지 확인합니다.
    // (에디터에서 설정하지 않으면 이 부분은 실행되지 않습니다.)
    if (SurveyWidgetClass)
    {
        // 3. 지정된 클래스로 위젯 인스턴스를 생성합니다. 'this'는 이 PlayerController가 위젯의 소유자임을 의미합니다.
        SurveyWidgetInstance = CreateWidget<UUserWidget>(this, SurveyWidgetClass);

        if (SurveyWidgetInstance)
        {
            // 4. 생성된 위젯을 화면(Viewport)에 추가합니다.
            SurveyWidgetInstance->AddToViewport();

            // 5. 마우스 커서가 보이도록 설정합니다.
            bShowMouseCursor = true;

            // 6. 입력 모드를 'UI 전용'으로 변경합니다.
            // 이렇게 해야 게임 월드는 클릭되지 않고 UI만 조작할 수 있습니다.
            FInputModeUIOnly InputModeData;
            InputModeData.SetWidgetToFocus(SurveyWidgetInstance->TakeWidget()); // 포커스를 우리 위젯에 줍니다.
            InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스가 뷰포트 밖으로 나갈 수 있게 합니다.

            SetInputMode(InputModeData);
        }
    }
}
