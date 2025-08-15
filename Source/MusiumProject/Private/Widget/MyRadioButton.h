// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "MyRadioButton.generated.h"

/**
 * 
 */
UCLASS()
class UMyRadioButton : public UCheckBox
{
	GENERATED_BODY()
	



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Radio Button")
	FString RadioButtonID;
protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;
	UFUNCTION()
	void OnRadioCheckStateChanged(bool bIsChecked);

};
