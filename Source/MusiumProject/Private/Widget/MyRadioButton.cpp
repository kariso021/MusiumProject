// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRadioButton.h"


TSharedRef<SWidget> UMyRadioButton::RebuildWidget()
{
    TSharedRef<SWidget> Widget = Super::RebuildWidget();

   //��������Ʈ ���ε��� ���⼭ ����
    if (!OnCheckStateChanged.IsBound())
    {
        OnCheckStateChanged.AddDynamic(this, &UMyRadioButton::OnRadioCheckStateChanged);
    }

    //�θ� ������ Slate ������ �״�� ��ȯ
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
