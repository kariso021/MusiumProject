// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/WidgetSwitcher.h>
#include "Data/SurveyFrequencyQuestionRow.h"
#include "Data/SurveyResultRow.h"
#include "MySurveyWidget.generated.h"

/**
 * 
 */


class UImage;
class UTextBlock;
class UQuestionGroupPanel;
class UMyRadioButton;
class UProgressBar;

UCLASS()
class UMySurveyWidget : public UUserWidget
{
	GENERATED_BODY()
	


protected:
	// 찾아낸 질문 그룹 패널들을 순서에 상관없이 저장할 배열
	UPROPERTY()
	TArray<TObjectPtr<UQuestionGroupPanel>> CachedQuestionPanels;

	virtual void NativeConstruct() override;


	// 위젯 스위칭
	UPROPERTY(meta = (BindWidgetOptional)) UWidgetSwitcher* SurveySwitcher;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private:
	UMyRadioButton* FindSelectedRadioButtonInPanel(UPanelWidget* PanelToSearch);

public:

	// TODO : 제출부분 고쳐야함
	UFUNCTION(BlueprintCallable, Category = "Survey")
	void SubmitSurvey();

protected:

	//  그냥 다음 패널로 넘어가는 함수
	UFUNCTION(BlueprintCallable, Category = "Survey")
	void GoToNextPanel();


	UPROPERTY(EditAnywhere, Category = "Survey")
	TObjectPtr<UDataTable> FrequencyQuestionDataTable;


	//빈도수 설문
	UFUNCTION(BlueprintCallable, Category = "Survey")
	void StartFrequencySurvey();

	TArray<FSurveyFrequencyQuestionRow*> FrequencyQuestionRows;
	int32 CurrentFrequencyQuestionIndex = -1;
	TMap<FString, int32> CollectedFrequencyCounts;


	// 다음 성향 조사 질문으로 전환
	void TransitionToNextFrequencyQuestion();
	void OnFadeOutFinished_UpdateFrequencyQuestion();


	// 빈도 수 참고용 텍스트
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> FrequencyQuestionText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> FrequencyAnswerText1;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> FrequencyAnswerText2;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> FrequencyAnswerText3;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> FrequencyAnswerText4;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> FrequencyAnswerText5;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> SurveyProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProgressText;




	//----------결과창 UI 바인딩 --------------------
	UPROPERTY(EditAnywhere, Category = "Survey")
	TObjectPtr<UDataTable> ResultDataTable;
	TArray<FSurveyResultRow*> ResultRows;
	// 결과 집계용 string
	FString MostFrequentID;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> ResultImage;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ResultTitleText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ResultDescriptionText;
	



	// 1,2,3,4,5 번으로 버튼을 나눔.
	UFUNCTION(BlueprintCallable, Category = "Survey")
	void OnFrequencyAnswerClicked(int32 numbers);

	void HandleFrequencyAnswer(const FString& AnswerID);


	// 맨 마지막 결과창

	void ShowResultScreen();


	//질문 애니메이션 함수들
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn_QuestionAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOut_QuestionAnimation;

	float CurrentPercent;
	float TargetPercent;
	float ProgressBarAnimSpeed;

	FTimerHandle TransitionTimerHandle;
};
