// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MySurveyWidget.generated.h"

/**
 * 
 */

class UMyRadioButton;

UCLASS()
class UMySurveyWidget : public UUserWidget
{
	GENERATED_BODY()
	


protected:
	// 찾아낸 질문 그룹 패널들을 순서에 상관없이 저장할 배열
	UPROPERTY()
	TArray<TObjectPtr<UPanelWidget>> CachedQuestionPanels;

	virtual void NativeConstruct() override;

private:
	UMyRadioButton* FindSelectedRadioButtonInPanel(UPanelWidget* PanelToSearch);

public:
	UFUNCTION(BlueprintCallable, Category = "Survey")
	void SubmitSurvey(const FString& InFileName, const FString& InSaveSlot);

};
