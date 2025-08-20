// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRadioButton.h"


TSharedRef<SWidget> UMyRadioButton::RebuildWidget()
{
    TSharedRef<SWidget> Widget = Super::RebuildWidget();

   //델리게이트 바인딩을 여기서 수행
    if (!OnCheckStateChanged.IsBound())
    {
        OnCheckStateChanged.AddDynamic(this, &UMyRadioButton::OnRadioCheckStateChanged);
    }

    //부모가 생성한 Slate 위젯을 그대로 반환
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
