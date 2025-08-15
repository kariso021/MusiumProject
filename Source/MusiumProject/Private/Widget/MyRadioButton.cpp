// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MyRadioButton.h"
#include "MyRadioButton.h"


TSharedRef<SWidget> UMyRadioButton::RebuildWidget()
{
    // 1. 부모 클래스(UCheckBox)의 RebuildWidget을 먼저 호출합니다.
    // 이 호출이 MyCheckbox라는 이름의 Slate 위젯(SCheckBox)을 생성하고 초기화합니다.
    // 이 줄을 실행한 이후부터 'MyCheckbox' 변수를 사용할 수 있습니다.
    TSharedRef<SWidget> Widget = Super::RebuildWidget();

    // 2. 이제 MyCheckbox가 생성되었으므로, 델리게이트 바인딩을 여기서 수행합니다.
    if (!OnCheckStateChanged.IsBound())
    {
        OnCheckStateChanged.AddDynamic(this, &UMyRadioButton::OnRadioCheckStateChanged);
    }

    // 3. 부모가 생성한 Slate 위젯을 그대로 반환합니다.
    return Widget;
}

void UMyRadioButton::OnRadioCheckStateChanged(bool bIsChecked)
{
    if (!bIsChecked)
    {
        return;
    }

    UPanelWidget* ParentPanel = GetParent();
    if (!ParentPanel)
    {
        return;
    }

    for (UWidget* ChildWidget : ParentPanel->GetAllChildren())
    {
        UMyRadioButton* SiblingRadioButton = Cast<UMyRadioButton>(ChildWidget);
        if (SiblingRadioButton && SiblingRadioButton != this)
        {
            SiblingRadioButton->SetIsChecked(false);
        }
    }
}
