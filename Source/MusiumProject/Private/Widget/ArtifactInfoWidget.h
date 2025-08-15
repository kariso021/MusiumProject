// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "ArtifactInfoWidget.generated.h"


/**
 * 
 */

UCLASS()
class UArtifactInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	


public:


	UFUNCTION(BlueprintCallable, Category = "Artifact Info")
    void SetData(const FArtifactData& Data);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    UMediaPlayer* MediaPlayer;


protected:
    virtual void NativeConstruct() override;

    // 바인딩
    UPROPERTY(meta = (BindWidget)) UTextBlock* NameText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* EraText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* OriginText;
    UPROPERTY(meta = (BindWidget)) UTextBlock* DescriptionText;

    UPROPERTY(meta = (BindWidget)) UTextBlock* TechText;

	UPROPERTY(meta = (BindWidget)) UTextBlock* PatternMeaningText;

    UPROPERTY(meta = (BindWidget)) UImage* Img_main;




    //스위칭 및 판넬

    UPROPERTY(meta = (BindWidgetOptional)) UWidgetSwitcher* Switch_Content;

    UPROPERTY(meta = (BindWidgetOptional)) UWidget* Panel_Basic;
    UPROPERTY(meta = (BindWidgetOptional)) UWidget* Panel_Meaning;
    UPROPERTY(meta = (BindWidgetOptional)) UWidget* Panel_Tech;
    UPROPERTY(meta = (BindWidgetOptional)) UWidget* Panel_Similar;
    UPROPERTY(meta = (BindWidgetOptional)) UWidget* Panel_Video;
    UPROPERTY(meta = (BindWidgetOptional)) UWidget* Panel_Emotion;




    // 버튼들
    UPROPERTY(meta = (BindWidgetOptional)) UButton* Btn_CloseIcon;
   
    UPROPERTY(meta = (BindWidgetOptional)) UButton* Btn_Basic;
	UPROPERTY(meta = (BindWidgetOptional)) UButton* Btn_Tech;
	UPROPERTY(meta = (BindWidgetOptional)) UButton* Btn_Meaning;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* Btn_Similar;
	UPROPERTY(meta = (BindWidgetOptional)) UButton* Btn_Video;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* Btn_Emotion;
	// 버튼 클릭 이벤트 핸들러
	UFUNCTION()
  
    void SwitchTo(UWidget* Panel);


    UFUNCTION() void OnBtnBasicClicked();
    UFUNCTION() void OnBtnTechClicked();
    UFUNCTION() void OnBtnMeaningClicked();
    UFUNCTION() void OnBtnSimilarClicked();
    UFUNCTION() void OnBtnVideoClicked();
    UFUNCTION() void OnBtnEmotionClicked();


private:

    TSoftObjectPtr<UTexture2D> ThumbnailSource;
    TSoftObjectPtr<UMediaSource> DescriptionVideoSource;
    TSoftObjectPtr<UMediaSource> EmotionVideoSource;
};
