// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/WidgetSwitcher.h>
#include "MySurveyWidget.generated.h"

/**
 * 
 */

class UQuestionGroupPanel;
class UMyRadioButton;

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

private:
	UMyRadioButton* FindSelectedRadioButtonInPanel(UPanelWidget* PanelToSearch);


public:
	UFUNCTION(BlueprintCallable, Category = "Survey")
	void SubmitSurvey(const FString& InFileName, const FString& InSaveSlot);

	UFUNCTION(BlueprintCallable, Category = "Survey")
	void GoToNextPanel();

};
