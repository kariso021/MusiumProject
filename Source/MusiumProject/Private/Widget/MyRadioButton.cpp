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



    const FLinearColor White(1.f, 1.f, 1.f);





    WidgetStyle.ForegroundColor = White;

    WidgetStyle.HoveredForeground = White;

    WidgetStyle.PressedForeground = White;

    WidgetStyle.CheckedForeground = White;

    WidgetStyle.CheckedHoveredForeground = White;

    WidgetStyle.CheckedPressedForeground = White;

    SetWidgetStyle(WidgetStyle);



    //부모가 생성한 Slate 위젯을 그대로 반환

    return Widget;

}



void UMyRadioButton::OnRadioCheckStateChanged(bool bIsChecked)

{

    UPanelWidget* ParentPanel = GetParent();

    if (!ParentPanel) return;



    switch (SelectionMode)

    {

    case ERadioSelectionMode::Single:

        if (bIsChecked) // 단일 선택은 '선택'될 때만 다른 버튼들을 해제

        {

            // 기존의 단일 선택 로직

            for (UWidget* ChildWidget : ParentPanel->GetAllChildren())

            {

                UMyRadioButton* Sibling = Cast<UMyRadioButton>(ChildWidget);

                if (Sibling && Sibling != this && Sibling->SelectionMode == ERadioSelectionMode::Single)

                {

                    Sibling->SetIsChecked(false);

                }

            }

        }

        break;



    case ERadioSelectionMode::Multiple:

        break;



    case ERadioSelectionMode::LimitedMultiple:

        if (bIsChecked) // 개수 제한은 '선택'될 때만 검사

        {

            TArray<UMyRadioButton*> CheckedSiblings;

            for (UWidget* ChildWidget : ParentPanel->GetAllChildren())

            {

                UMyRadioButton* Sibling = Cast<UMyRadioButton>(ChildWidget);

                if (Sibling && Sibling->IsChecked() && Sibling->SelectionMode == ERadioSelectionMode::LimitedMultiple)

                {

                    CheckedSiblings.Add(Sibling);

                }

            }



            // 현재 선택된 개수가 제한을 초과하면, 방금 한 선택을 되돌림

            if (CheckedSiblings.Num() > MaxSelectionLimit)

            {

                SetIsChecked(false);

                UE_LOG(LogTemp, Warning, TEXT("최대 %d개까지만 선택할 수 있습니다."), MaxSelectionLimit);

            }

        }

        break;

    }

}