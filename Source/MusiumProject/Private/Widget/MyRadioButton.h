// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "MyRadioButton.generated.h"



UENUM(BlueprintType)
enum class ERadioSelectionMode : uint8
{
	// 단일 선택 (기존과 동일)
	Single,
	// 다중 선택 (제한 없음)
	Multiple,
	// 다중 선택 (개수 제한)
	LimitedMultiple
};



UCLASS()
class UMyRadioButton : public UCheckBox
{
	GENERATED_BODY()
	



public:

	// RadioButton의 선택 모드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Radio Button")
	ERadioSelectionMode SelectionMode = ERadioSelectionMode::Single;

	// 'LimitedMultiple' 모드일 때만 활성화되어 최대 선택 개수를 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Radio Button", meta = (EditCondition = "SelectionMode == ERadioSelectionMode::LimitedMultiple"))
	int32 MaxSelectionLimit = 3;

	// 기존의 RadioButtonID (선택사항)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Radio Button")
	FString RadioButtonID;



protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Radio Button")
	FText ButtonText;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	UFUNCTION()
	void OnRadioCheckStateChanged(bool bIsChecked);

};
