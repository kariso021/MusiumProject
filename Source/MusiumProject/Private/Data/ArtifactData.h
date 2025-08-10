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

	// --- �⺻ ���� ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Era;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Origin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;

	// --- �̹��� ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* Photo;

	// --- ������ ����� ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
	UMediaSource* VideoSource;
};
