#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "MyExitSurveyWidget.generated.h"


// ===== 위젯 클래스들을 전방 선언합니다 =====
class UWidgetSwitcher;
class UProgressBar;
class UTextBlock;
class UPanelWidget;
class UMyRadioButton;
class UMultiLineEditableTextBox;

/**
 * 
 */
UCLASS()
class UMyExitSurveyWidget : public UUserWidget
{
	GENERATED_BODY()


protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 전체 설문조사 화면 전환 스위처
	UPROPERTY(meta = (BindWidgetOptional)) UWidgetSwitcher* MainSurveySwitcher;

	// 설문조사 선택 화면 전환 스위처
	UPROPERTY(meta = (BindWidgetOptional)) UWidgetSwitcher* SelectSurveySwitcher;


	// 설문조사 이동하는 함수들
	UFUNCTION(BlueprintCallable, Category = "Survey Navigation")
	void GoToNext_MainSurveyStep();

	UFUNCTION(BlueprintCallable, Category = "Survey Navigation")
	void StartSelectSurveyStep();


	UFUNCTION(BlueprintCallable, Category = "Survey Navigation")
	void GoToNext_SelectSurveyStep();

	UFUNCTION(BlueprintCallable, Category = "Survey Navigation")
	void GoToPrevious_SelectSurveyStep();



	// ProgressBar 관련 SelectSurveyWitcher 에서만 적용할것임


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> SurveyProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProgressText;


	float CurrentPercent;
	float TargetPercent;
	float ProgressBarAnimSpeed;

	void UpdateProgress(int32 CurrentIndex, int32 TotalCount);


private:

	UMyRadioButton* FindSelectedRadioButtonInPanel(UPanelWidget* PanelToSearch);
	TArray<FString> FindAllSelectedRadioButtonIDsInPanel(UPanelWidget* PanelToSearch);


	//---------- 패널 바인드용
		// --- 블루프린트 패널 연결용 변수 ---

   // 1~3번 질문 패널 (단일 선택)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ExitQuestionPanel_01;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ExitQuestionPanel_02;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ExitQuestionPanel_03;

	// 4~6번 질문 패널 (복수 선택)
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ExitQuestionPanel_04;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ExitQuestionPanel_05;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ExitQuestionPanel_06;



	UPROPERTY(meta = (BindWidget))
	UButton* PreviousButton;

	UPROPERTY(meta = (BindWidget))
	UButton* NextButton;

	void UpdateNavigationButtons();


	//---------제출함수
	UFUNCTION(BlueprintCallable, Category = "Exit Survey")
	void SubmitExitSurvey(const FString& FinalFeedbackText);



};
