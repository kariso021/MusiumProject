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
	// ã�Ƴ� ���� �׷� �гε��� ������ ������� ������ �迭
	UPROPERTY()
	TArray<TObjectPtr<UPanelWidget>> CachedQuestionPanels;

	virtual void NativeConstruct() override;

private:
	UMyRadioButton* FindSelectedRadioButtonInPanel(UPanelWidget* PanelToSearch);

public:
	UFUNCTION(BlueprintCallable, Category = "Survey")
	void SubmitSurvey(const FString& InFileName, const FString& InSaveSlot);

};
