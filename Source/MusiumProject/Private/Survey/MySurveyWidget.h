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
	// ã�Ƴ� ���� �׷� �гε��� ������ ������� ������ �迭
	UPROPERTY()
	TArray<TObjectPtr<UQuestionGroupPanel>> CachedQuestionPanels;

	virtual void NativeConstruct() override;

	// ���� ����Ī
	UPROPERTY(meta = (BindWidgetOptional)) UWidgetSwitcher* SurveySwitcher;

private:
	UMyRadioButton* FindSelectedRadioButtonInPanel(UPanelWidget* PanelToSearch);


public:
	UFUNCTION(BlueprintCallable, Category = "Survey")
	void SubmitSurvey(const FString& InFileName, const FString& InSaveSlot);

	UFUNCTION(BlueprintCallable, Category = "Survey")
	void GoToNextPanel();

};
