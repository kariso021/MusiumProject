// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once
#include "Engine/DataTable.h"
#include "MediaSource.h"
#include "CoreMinimal.h"
#include "SurveyFrequencyQuestionRow.generated.h"

//SurveyFrequencyQuestionRow

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSurveyFrequencyQuestionRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString QuestionText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Answer1_Text; 


	//각각 1 로 초기화
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Answer1_ID = TEXT("1");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Answer2_Text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Answer2_ID = TEXT("2");
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Answer3_Text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Answer3_ID = TEXT("3");
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Answer4_Text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Answer4_ID = TEXT("4");
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Answer5_Text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Answer5_ID = TEXT("5");;




};
