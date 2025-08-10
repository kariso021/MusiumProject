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

	// --- 기본 정보 ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Era;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Origin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;

	// --- 이미지 ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* Photo;

	// --- 동영상 재생용 ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	UMediaSource* VideoSource;
};
