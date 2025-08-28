// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TrackingDataUtility.generated.h"

/**
 * 
 */
UCLASS()
class MUSIUMPROJECT_API UTrackingDataUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Tracking Data Utility")
	static TArray<FString> GetAllSessionIDs();

	
	UFUNCTION(BlueprintCallable, Category = "Tracking Data Utility")
	static bool ExportSessionToCsv(const FString& SessionID);
};
