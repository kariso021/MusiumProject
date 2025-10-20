// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "MyRadioButton.generated.h"

class UPanelWidget;

UCLASS()
class YOURPROJECT_API UMyRadioButton : public UCheckBox
{
    GENERATED_BODY()

public:
    // ... (기존 SelectionMode, MaxSelectionLimit 프로퍼티)

    /** The color to use for the foreground when the button is in a 'dimmed' state. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
    FLinearColor DimmedColor = FLinearColor::Gray;

    /**
     * 그룹으로 묶을 라디오 버튼들을 담고 있는 패널입니다.
     * 이 값을 지정하면 GetParent() 대신 지정된 패널 안에서만 계산하여 성능이 향상됩니다.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Group")
    UPanelWidget* RadioButtonGroupPanel;


protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void SynchronizeProperties() override;

    UFUNCTION()
    void OnRadioCheckStateChanged(bool bIsChecked);

    void UpdateSiblingStates();

private:
    /** Stores the original foreground color to restore it from a dimmed state. */
    FLinearColor NormalForegroundColor;
};