// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/WidgetSwitcher.h>
#include "MediaPlayer.h"
#include <MediaSoundComponent.h>
#include "MediaSource.h"
#include <Components/Slider.h>
#include "ArtifactInfoWidget.generated.h"



class UButton;
class UImage;

UCLASS()
class UArtifactInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	


public:


	UFUNCTION(BlueprintCallable, Category = "Artifact Info")
    void SetData(const FArtifactData& Data);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Media")
    UMediaPlayer* MediaPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MediaSound")
    UMediaSoundComponent* MediaSoundComponent;


protected:
    virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

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

	// 위젯 종료 버튼 함수
	UFUNCTION(BlueprintCallable, Category = "CloseWidget")
	void CloseWidget();


private:

    TSoftObjectPtr<UTexture2D> ThumbnailSource;
    TSoftObjectPtr<UMediaSource> DescriptionVideoSource;
    TSoftObjectPtr<UMediaSource> EmotionVideoSource;

    // 비디오 관련 변수 함수들

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_PlayPause;

	// 재생/일시정지 아이콘을 바꿀 수 있도록 Image 위젯으로 가정
	UPROPERTY(meta = (BindWidget))
	UImage* Img_PlayPauseIcon;

	UPROPERTY(meta = (BindWidget))
	USlider* Slider_Volume;

	UPROPERTY(meta = (BindWidget))
	USlider* Slider_Video;

	// --- 새로 추가할 텍스처 변수들 (블루프린트에서 설정) ---
	UPROPERTY(EditAnywhere, Category = "Media|Icons")
	UTexture2D* PlayIcon;

	UPROPERTY(EditAnywhere, Category = "Media|Icons")
	UTexture2D* PauseIcon;


	// --- 새로 추가할 함수들 ---
	UFUNCTION()
	void OnPlayPauseClicked();

	UFUNCTION()
	void OnVolumeChanged(float Value);

	// 비디오 슬라이더를 유저가 직접 조작할 때를 위한 함수들
	UFUNCTION()
	void OnVideoSliderMouseCaptureBegin();

	UFUNCTION()
	void OnVideoSliderMouseCaptureEnd();

	UFUNCTION()
	void OnVideoSliderValueChanged(float Value);


	// --- 내부 상태 변수 ---
	// 유저가 슬라이더를 잡고 있는지 여부
	bool bIsSeeking = false;


	private:
		// Seek() 호출을 제한하기 위한 변수들
		float LastSeekTime = 0.f;
		const float SeekThrottleInterval = 0.1f;

		// 사용자가 마우스를 놓았을 때의 최종 슬라이더 위치
		float LatestSliderValue = 0.f;

		//------- 재생 측정용-------
		// 
		// 재생이 시작된 시점의 월드 시간 (GetWorld()->GetTimeSeconds())
		float PlaybackStartTime = 0.f;

		// 현재 비디오의 누적 재생 시간 (초)
		float AccumulatedPlaybackTime = 0.f;

		// 현재 재생/추적 중인 미디어 소스를 가리키는 포인터
		UPROPERTY()
		TObjectPtr<UMediaSource> CurrentMediaSource;

		/** 재생 시간 측정을 위한 헬퍼 함수들 */
		// 재생 시간 추적을 시작
		void StartPlaybackTracking();

		// 재생 시간 추적을 중지하고 시간을 누적합
		void StopPlaybackTracking();

		// 새로운 비디오가 로드될 때 추적 정보를 리셋하고, 이전 비디오의 재생 시간을 로그f
		void ResetPlaybackTracking(UMediaSource* NewSource);

		UFUNCTION()
		void HandlePlaybackResumed();

		UFUNCTION()
		void HandlePlaybackPaused();

		UFUNCTION()
		void HandlePlaybackEnded();

		UFUNCTION()
		void HandleSeekCompleted();

};
