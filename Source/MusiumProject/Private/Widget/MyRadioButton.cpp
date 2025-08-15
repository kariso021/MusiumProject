// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MyRadioButton.h"
#include "MyRadioButton.h"


TSharedRef<SWidget> UMyRadioButton::RebuildWidget()
{
    // 1. �θ� Ŭ����(UCheckBox)�� RebuildWidget�� ���� ȣ���մϴ�.
    // �� ȣ���� MyCheckbox��� �̸��� Slate ����(SCheckBox)�� �����ϰ� �ʱ�ȭ�մϴ�.
    // �� ���� ������ ���ĺ��� 'MyCheckbox' ������ ����� �� �ֽ��ϴ�.
    TSharedRef<SWidget> Widget = Super::RebuildWidget();

    // 2. ���� MyCheckbox�� �����Ǿ����Ƿ�, ��������Ʈ ���ε��� ���⼭ �����մϴ�.
    if (!OnCheckStateChanged.IsBound())
    {
        OnCheckStateChanged.AddDynamic(this, &UMyRadioButton::OnRadioCheckStateChanged);
    }

    // 3. �θ� ������ Slate ������ �״�� ��ȯ�մϴ�.
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
