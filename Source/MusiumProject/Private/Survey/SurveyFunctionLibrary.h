// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SurveyFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class USurveyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Survey Functions", meta = (WorldContext = "WorldContextObject"))
	static void SaveSelectedRadioTagsToCSV(UWidget* InParentWidget, const FString& InFileName, const FString& InSaveSlot);
	
};
