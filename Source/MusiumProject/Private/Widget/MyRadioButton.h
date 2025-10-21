#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "MyRadioButton.generated.h"

// 라디오 버튼의 선택 모드를 정의하는 열거형(Enum)입니다.
UENUM(BlueprintType)
enum class ERadioSelectionMode : uint8
{
	Single,
	Multiple,
	LimitedMultiple
};

UCLASS()
class MUSIUMPROJECT_API UMyRadioButton : public UCheckBox
{
	GENERATED_BODY()

public:
	/** 라디오 버튼의 선택 동작을 결정합니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Button")
	ERadioSelectionMode SelectionMode = ERadioSelectionMode::Single;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Button", meta = (EditCondition = "SelectionMode == ERadioSelectionMode::LimitedMultiple"))
	int32 MaxSelectionLimit = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radio Button")
	FString RadioButtonID;

protected:
	/** 위젯이 처음 생성되거나 재구성될 때 호출되는 함수입니다. */
	virtual TSharedRef<SWidget> RebuildWidget() override;

	/** 체크 상태가 변경될 때 호출되는 델리게이트 함수입니다. */
	UFUNCTION()
	void OnRadioCheckStateChanged(bool bIsChecked);
};