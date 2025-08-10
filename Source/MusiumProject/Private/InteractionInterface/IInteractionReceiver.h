// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionData/InteractionData.h"
#include "IInteractionReceiver.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInteractionReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIInteractionReceiver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    /** ����Ʈ���̽� ����/�Ұ� ��� */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void EnableLineTrace(bool bEnable);

    /** ���ͷ��� UI ǥ�� */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void ShowInteractionUI(const FInteractionData& Data);
};
