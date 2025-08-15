// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "MediaSource.h"
#include "CoreMinimal.h"
#include "ArtifactData.generated.h"

USTRUCT(BlueprintType)
struct FArtifactData : public FTableRowBase
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Era;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Origin;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true)) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true)) FText TechniqueDescription;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true)) FText PatternMeaning;

    // --- 이미지 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite)  TSoftObjectPtr<UTexture2D> Thumbnail = nullptr;

    // --- 동영상 재생용 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    TSoftObjectPtr<UMediaSource> DecriptionVideoSource;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    TSoftObjectPtr<UMediaSource> EmotionVideoSource;





};
