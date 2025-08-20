// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "QuestionGroupPanel.generated.h"


UENUM(BlueprintType)
enum class EQuestionPanelType : uint8
{
    // 사전 정보 (age, sex 등)
    Demographic UMETA(DisplayName = "Demographic Info"),
    // 빈도 계산용 성향 조사 (1~5번 선택)
    Frequency UMETA(DisplayName = "Frequency Analysis")
};


UCLASS()
class MUSIUMPROJECT_API UQuestionGroupPanel : public UVerticalBox
{
	GENERATED_BODY()


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Survey")
    EQuestionPanelType QuestionType = EQuestionPanelType::Demographic;

};