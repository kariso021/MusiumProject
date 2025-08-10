// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionData/InteractionData.h"
#include "IInteractiveTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIInteractiveTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIInteractiveTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    /** 호버 시작 */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnHoverStart();

    /** 호버 종료 */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnHoverEnd();

    /** 실제 인터랙트 호출 */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact(AActor* Interactor);

    /** 액터에 저장된 데이터 반환 */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    FInteractionData GetInteractionData();
};
