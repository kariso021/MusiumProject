// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "MediaSource.h"
#include "CoreMinimal.h"
#include "SurveyResultRow.generated.h"

USTRUCT(BlueprintType)
struct FSurveyResultRow : public FTableRowBase
{
    GENERATED_BODY()

    // 결과창에 표시될 유형 이름 (예: "탐구형 관람객")
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ResultTitle;

    // 결과에 대한 상세 설명 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ResultDescription;

    // 결과 이미지 (경로를 저장하고 나중에 불러올 수 있도록 Soft Object Ptr 사용)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> ResultImage;
};
