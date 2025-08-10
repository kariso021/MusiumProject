// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionData.generated.h"

USTRUCT(BlueprintType)
struct FInteractionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FString Description;

    // �ʿ��ϴٸ� �߰� �ʵ塦
};

