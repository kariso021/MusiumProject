// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/ArtifactInfoWidget.h"
#include "Data/ArtifactData.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "MediaSoundComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/WidgetSwitcher.h"
#include "TrackingData/TrackingSubsystem.h"
#include <Components/Button.h>
#include "MusiumPlayerController.h"
#include "..\Components\InteractionComponent.h"
#include "Sound/SoundManagerSubsystem.h"

void UArtifactInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	

	// 비디오 탐색 슬라이더 설정
	if (Slider_Video)
	{
		Slider_Video->OnMouseCaptureBegin.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderMouseCaptureBegin);
		Slider_Video->OnMouseCaptureEnd.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderMouseCaptureEnd);
		Slider_Video->OnValueChanged.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderValueChanged);
	}

	if(Slider_EmotionVideo)
	{
		Slider_EmotionVideo->OnMouseCaptureBegin.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderMouseCaptureBegin);
		Slider_EmotionVideo->OnMouseCaptureEnd.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderMouseCaptureEnd);
		Slider_EmotionVideo->OnValueChanged.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderValueChanged);
	}

	if (Btn_PlayPause_EmotionVideo)
	{
		Btn_PlayPause_EmotionVideo->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnPlayPauseClicked);
	}

	if(Btn_PlayPause_Video)
	{
		Btn_PlayPause_Video->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnPlayPauseClicked);
	}
	

	// ===== 핵심 변경 사항: MediaPlayer 이벤트에 함수 바인딩 =====
	// MediaPlayer의 상태가 바뀔 때마다 해당 함수들이 자동으로 호출됩니다.
	if (MediaPlayer)
	{
		MediaPlayer->OnPlaybackResumed.AddUniqueDynamic(this, &UArtifactInfoWidget::HandlePlaybackResumed);
		MediaPlayer->OnPlaybackSuspended.AddUniqueDynamic(this, &UArtifactInfoWidget::HandlePlaybackPaused);
		MediaPlayer->OnEndReached.AddUniqueDynamic(this, &UArtifactInfoWidget::HandlePlaybackEnded);
	}


	if(HoverButton_EmotionVideo && HoverPanel_Emotion)
	{
		// HoverButton_EmotionVideo에 마우스 오버 시 HoverPanel_Emotion 표시
		HoverButton_EmotionVideo->OnHovered.AddDynamic(this, &UArtifactInfoWidget::ShowEmotionHoverPanel);
		HoverButton_EmotionVideo->OnUnhovered.AddDynamic(this, &UArtifactInfoWidget::HideEmotionHoverPanel);
	}

	if(HoverButton_Video && HoverPanel_Video)
	{
		// HoverButton_Video에 마우스 오버 시 HoverPanel_Video 표시
		HoverButton_Video->OnHovered.AddDynamic(this, &UArtifactInfoWidget::ShowVideoHoverPanel);
		HoverButton_Video->OnUnhovered.AddDynamic(this, &UArtifactInfoWidget::HideVideoHoverPanel);
	}

	if (!GlobalSoundManager)
	{
		UGameInstance* GameInstance = GetGameInstance();
		if (GameInstance)
		{
			USoundManagerSubsystem* SoundSubsystem = GameInstance->GetSubsystem<USoundManagerSubsystem>();
			if (SoundSubsystem)
			{
				GlobalSoundManager = SoundSubsystem->GetGlobalSoundManager();
			}
		}
	}

}

void UArtifactInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 유저가 슬라이더를 조작하지 않을 때만 재생 시간에 맞춰 슬라이더 위치 업데이트
	if (MediaPlayer && MediaPlayer->IsPlaying() && !bIsSeeking)
	{
		const FTimespan Duration = MediaPlayer->GetDuration();
		if (Duration.GetTotalSeconds() > 0)
		{
			const FTimespan CurrentTime = MediaPlayer->GetTime();
			const float NewSliderValue = CurrentTime.GetTotalSeconds() / Duration.GetTotalSeconds();

			// ===== 핵심 수정: 현재 활성화된 패널에 맞는 슬라이더를 업데이트 =====
			if (Switch_Content) // WidgetSwitcher가 유효한지 확인
			{
				UWidget* ActiveWidget = Switch_Content->GetActiveWidget();
				if (ActiveWidget == card_Video && Slider_Video)
				{
					Slider_Video->SetValue(NewSliderValue);
				}
				else if (ActiveWidget == card_Emotion && Slider_EmotionVideo)
				{
					Slider_EmotionVideo->SetValue(NewSliderValue);
				}
			}
		}
	}
}

void UArtifactInfoWidget::SetData(const FArtifactData& Data)
{
	if (NameText) NameText->SetText(Data.Name);
	if (DiscriptionNameText) DiscriptionNameText->SetText(Data.Name);


	ThumbnailSource = Data.Thumbnail;
	DescriptionImageSource = Data.Description;
	TechImageSource = Data.TechniqueDescription;
	PatternMeaningImageSource = Data.PatternMeaning;
	SimilarItemImageSource = Data.SimularItem;
	DescriptionVideoSource = Data.DecriptionVideoSource;
	EmotionVideoSource = Data.EmotionVideoSource;


	// 비동기로 각자 불러오기
	if (!ThumbnailSource.IsNull())
	{
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(ThumbnailSource.ToSoftObjectPath(),
			[this]()
			{
				UTexture2D* LoadedTexture = ThumbnailSource.Get();
				if (Img_main && LoadedTexture)
				{
					Img_main->SetBrushFromTexture(LoadedTexture, true);
				}

				if (Img_DiscriptionThumbnail && LoadedTexture)
				{
					Img_DiscriptionThumbnail->SetBrushFromTexture(LoadedTexture, true);
				}
			});
	}

	if (!DescriptionImageSource.IsNull())
	{
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(DescriptionImageSource.ToSoftObjectPath(),
			[this]()
			{
				UTexture2D* LoadedTexture = DescriptionImageSource.Get();
				if (DiscriptionImage && LoadedTexture)
				{
					DiscriptionImage->SetBrushFromTexture(LoadedTexture, true);
				}
			});
	}


	if(!Data.TechniqueDescription.IsNull())
	{
		TechImageSource = Data.TechniqueDescription;
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(TechImageSource.ToSoftObjectPath(),
			[this]()
			{
				UTexture2D* LoadedTexture = TechImageSource.Get();
				if (TechImage && LoadedTexture)
				{
					TechImage->SetBrushFromTexture(LoadedTexture, true);
				}
			});
	}

	if(!Data.PatternMeaning.IsNull())
	{
		PatternMeaningImageSource = Data.PatternMeaning;
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(PatternMeaningImageSource.ToSoftObjectPath(),
			[this]()
			{
				UTexture2D* LoadedTexture = PatternMeaningImageSource.Get();
				if (PatternMeaningImage && LoadedTexture)
				{
					PatternMeaningImage->SetBrushFromTexture(LoadedTexture, true);
				}
			});
	}

	if(!Data.SimularItem.IsNull())
	{
		SimilarItemImageSource = Data.SimularItem;
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(SimilarItemImageSource.ToSoftObjectPath(),
			[this]()
			{
				UTexture2D* LoadedTexture = SimilarItemImageSource.Get();
				if (SimilarItemImage && LoadedTexture)
				{
					SimilarItemImage->SetBrushFromTexture(LoadedTexture, true);
				}
			});
	}
}

// ===== 변경됨: SwitchTo 함수 단순화 =====
void UArtifactInfoWidget::SwitchTo(UWidget* Panel)
{
	if (!Switch_Content || !Panel) return;

	UWidget* CurrentPanel = Switch_Content->GetActiveWidget();
	// 비디오 패널을 떠나는 경우, 재생 중이던 비디오를 일시정지시킵니다.
	// StopPlaybackTracking()은 OnPlaybackPaused 이벤트가 발생하여 자동으로 처리하므로 직접 호출할 필요가 없습니다.
	if ((CurrentPanel == card_Video || CurrentPanel == card_Emotion) &&
		(Panel != card_Video && Panel != card_Emotion))
	{
		if (MediaPlayer && MediaPlayer->IsPlaying())
		{
			MediaPlayer->Pause();
		}
	}

	const int32 Index = Switch_Content->GetChildIndex(Panel);
	if (Index != INDEX_NONE)
	{
		Switch_Content->SetActiveWidgetIndex(Index);
	}
}

void UArtifactInfoWidget::OnBtnBasicClicked()
{
	SwitchTo(card_Basic);
	// 클릭 로그 남기기
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UTrackingSubsystem* TrackingSubsystem = GameInstance->GetSubsystem<UTrackingSubsystem>();
		if (TrackingSubsystem)
		{
			// 어떤 버튼 클릭했는지 로그 남기기
			TrackingSubsystem->LogClick("BasicButton");
		}
	}
}

void UArtifactInfoWidget::OnBtnTechClicked()
{
	SwitchTo(card_Tech);
	// 클릭 로그 남기기
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UTrackingSubsystem* TrackingSubsystem = GameInstance->GetSubsystem<UTrackingSubsystem>();
		if (TrackingSubsystem)
		{
			// 어떤 버튼 클릭했는지 로그 남기기
			TrackingSubsystem->LogClick("ProductionTechButton");
		}
	}
}

void UArtifactInfoWidget::OnBtnMeaningClicked()
{
	SwitchTo(card_Meaning);
	// 클릭 로그 남기기
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UTrackingSubsystem* TrackingSubsystem = GameInstance->GetSubsystem<UTrackingSubsystem>();
		if (TrackingSubsystem)
		{
			// 어떤 버튼 클릭했는지 로그 남기기
			TrackingSubsystem->LogClick("MeaingButton");
		}
	}
}

void UArtifactInfoWidget::OnBtnSimilarClicked()
{
	SwitchTo(card_Similar);
	// 클릭 로그 남기기
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UTrackingSubsystem* TrackingSubsystem = GameInstance->GetSubsystem<UTrackingSubsystem>();
		if (TrackingSubsystem)
		{
			// 어떤 버튼 클릭했는지 로그 남기기
			TrackingSubsystem->LogClick("SimilarButton");
		}
	}
}

void UArtifactInfoWidget::OnBtnVideoClicked()
{
	ResetPlaybackTracking(DescriptionVideoSource.Get());
	SwitchTo(card_Video);

	ResetPlaybackTracking(DescriptionVideoSource.Get());
	SwitchTo(card_Video);

	if (MediaPlayer && !DescriptionVideoSource.IsNull())
	{
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(DescriptionVideoSource.ToSoftObjectPath(),
			[this]()
			{
				UMediaSource* LoadedSource = DescriptionVideoSource.Get();
				if (MediaPlayer && LoadedSource)
				{
					MediaPlayer->OnMediaOpened.AddUniqueDynamic(this, &UArtifactInfoWidget::OnMediaOpened_AttachSound);
					MediaPlayer->OpenSource(LoadedSource);

					if (Img_PlayPauseIcon_Video && PauseIcon)
					{
						Img_PlayPauseIcon_Video->SetBrushFromTexture(PauseIcon);
					}


				}
			});
	}

	// 트래킹 로그
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UTrackingSubsystem* TrackingSubsystem = GameInstance->GetSubsystem<UTrackingSubsystem>();
		if (TrackingSubsystem)
		{
			TrackingSubsystem->LogClick("VideoButton");
		}
	}
}

void UArtifactInfoWidget::OnBtnEmotionClicked()
{
	ResetPlaybackTracking(EmotionVideoSource.Get());
	SwitchTo(card_Emotion);

	if (MediaPlayer && !EmotionVideoSource.IsNull())
	{
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(EmotionVideoSource.ToSoftObjectPath(),
			[this]()
			{
				if (GlobalSoundManager && GlobalSoundManager->GetMediaSoundComponent())
				{
					GlobalSoundManager->GetMediaSoundComponent()->SetMediaPlayer(MediaPlayer);
				}

				UMediaSource* LoadedSource = EmotionVideoSource.Get();
				if (MediaPlayer && LoadedSource)
				{
					MediaPlayer->OpenSource(LoadedSource);
					if (Img_PlayPauseIcon_EmotionVideo && PauseIcon)
					{
						Img_PlayPauseIcon_EmotionVideo->SetBrushFromTexture(PauseIcon);
					}
				}
			});
	}
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UTrackingSubsystem* TrackingSubsystem = GameInstance->GetSubsystem<UTrackingSubsystem>();
		if (TrackingSubsystem)
		{
			// 어떤 버튼 클릭했는지 로그 남기기
			TrackingSubsystem->LogClick("EmotionVideoButton");
		}
	}
}

void UArtifactInfoWidget::OnBtnExplainClicked()
{
	SwitchTo(card_Explain);
	// 클릭 로그 남기기
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UTrackingSubsystem* TrackingSubsystem = GameInstance->GetSubsystem<UTrackingSubsystem>();
		if (TrackingSubsystem)
		{
			// 어떤 버튼 클릭했는지 로그 남기기
			TrackingSubsystem->LogClick("ExplainButton");
		}
	}
}

// ===== 변경됨: CloseWidget 함수 순서 수정 =====
void UArtifactInfoWidget::CloseWidget()
{
	// 1. 마지막 재생 시간을 먼저 계산하고 누적합니다.
	StopPlaybackTracking();

	// 2. 그 다음에 MediaPlayer를 닫아서 리소스를 정리합니다.
	if (MediaPlayer)
	{
		if (MediaPlayer->IsPlaying() || MediaPlayer->IsPaused())
		{
			MediaPlayer->Close();
		}
	}

	// 3. 누적된 최종 시청 시간을 로그로 남깁니다.
	if (CurrentMediaSource && AccumulatedPlaybackTime > 0.1f)
	{
		UGameInstance* GI = GetGameInstance();
		if (GI)
		{
			UTrackingSubsystem* TrackingSubsystem = GI->GetSubsystem<UTrackingSubsystem>();
			if (TrackingSubsystem)
			{
				TrackingSubsystem->LogVideoPlay(AccumulatedPlaybackTime);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No significant playback time to log."));
	}

	// 4. 추적 관련 변수들을 초기화합니다.
	CurrentMediaSource = nullptr;
	AccumulatedPlaybackTime = 0.f;
	PlaybackStartTime = 0.f;

	// 5. 위젯 숨기기를 요청합니다.
	AMusiumPlayerController* PC = Cast<AMusiumPlayerController>(GetOwningPlayer());
	if (PC)
	{
		UInteractionUIComponent* InteractionComp = PC->GetInteractionUIComponent();
		if (InteractionComp)
		{
			InteractionComp->HideArtifactUI();
		}
	}
}

void UArtifactInfoWidget::ShowEmotionHoverPanel()
{
	HoverPanel_Emotion->SetVisibility(ESlateVisibility::Visible);
}

void UArtifactInfoWidget::HideEmotionHoverPanel()
{
	HoverPanel_Emotion->SetVisibility(ESlateVisibility::Hidden);
}

void UArtifactInfoWidget::ShowVideoHoverPanel()
{
	HoverPanel_Video->SetVisibility(ESlateVisibility::Visible);
}

void UArtifactInfoWidget::HideVideoHoverPanel()
{
	HoverPanel_Video->SetVisibility(ESlateVisibility::Hidden);
}

// ===== 변경됨: OnPlayPauseClicked 함수 대폭 단순화 =====
void UArtifactInfoWidget::OnPlayPauseClicked()
{
	if (!MediaPlayer || !Switch_Content) return;

	UWidget* ActiveWidget = Switch_Content->GetActiveWidget();

	if (MediaPlayer->IsPlaying())
	{
		MediaPlayer->Pause();

		// 활성화된 패널에 따라 올바른 '재생' 아이콘으로 변경합니다.
		if (ActiveWidget == card_Video && Img_PlayPauseIcon_Video && PlayIcon)
		{
			Img_PlayPauseIcon_Video->SetBrushFromTexture(PlayIcon);
		}
		else if (ActiveWidget == card_Emotion && Img_PlayPauseIcon_EmotionVideo && PlayIcon)
		{
			Img_PlayPauseIcon_EmotionVideo->SetBrushFromTexture(PlayIcon);
		}
	}
	else
	{
		MediaPlayer->Play();

		if (ActiveWidget == card_Video && Img_PlayPauseIcon_Video && PauseIcon)
		{
			Img_PlayPauseIcon_Video->SetBrushFromTexture(PauseIcon);
		}
		else if (ActiveWidget == card_Emotion && Img_PlayPauseIcon_EmotionVideo && PauseIcon)
		{
			Img_PlayPauseIcon_EmotionVideo->SetBrushFromTexture(PauseIcon);
		}
	}
}


// ===== 변경됨: 슬라이더 조작 함수 단순화 =====
void UArtifactInfoWidget::OnVideoSliderMouseCaptureBegin()
{
	bIsSeeking = true;
	if (MediaPlayer && MediaPlayer->IsPlaying())
	{
		// Pause만 호출하면 이벤트가 알아서 StopPlaybackTracking을 처리합니다.
		MediaPlayer->Pause();
	}
}

void UArtifactInfoWidget::OnVideoSliderMouseCaptureEnd()
{
	bIsSeeking = false;
	if (MediaPlayer)
	{
		const FTimespan Duration = MediaPlayer->GetDuration();
		const FTimespan SeekTime = Duration * LatestSliderValue;
		MediaPlayer->Seek(SeekTime);

		// Play만 호출하면 이벤트가 알아서 StartPlaybackTracking을 처리합니다.
		MediaPlayer->Play();
	}
}

void UArtifactInfoWidget::OnVideoSliderValueChanged(float Value)
{
	LatestSliderValue = Value;

	if (MediaPlayer && bIsSeeking)
	{
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - LastSeekTime > SeekThrottleInterval)
		{
			LastSeekTime = CurrentTime;
			const FTimespan Duration = MediaPlayer->GetDuration();
			const FTimespan SeekTime = Duration * Value;
			MediaPlayer->Seek(SeekTime);
		}
	}
}

// ===== 신규: MediaPlayer 이벤트 처리 함수들 =====
void UArtifactInfoWidget::HandlePlaybackResumed()
{
	// 재생이 시작/재개되면 추적을 시작합니다.
	StartPlaybackTracking();
}

void UArtifactInfoWidget::HandlePlaybackPaused()
{
	// 일시정지되면 추적을 멈춥니다.
	StopPlaybackTracking();
}

void UArtifactInfoWidget::HandlePlaybackEnded()
{
	// 재생이 완료되면 추적을 멈추고 UI를 리셋합니다.
	StopPlaybackTracking();
	if (MediaPlayer)
	{
		MediaPlayer->Pause();
		MediaPlayer->Seek(FTimespan::Zero());
	}

	if (Switch_Content)
	{
		UWidget* ActiveWidget = Switch_Content->GetActiveWidget();
		if (ActiveWidget == card_Video)
		{
			if (Img_PlayPauseIcon_Video && PlayIcon)
			{
				Img_PlayPauseIcon_Video->SetBrushFromTexture(PlayIcon);
			}
			if (Slider_Video)
			{
				Slider_Video->SetValue(0.0f);
			}
		}
		else if (ActiveWidget == card_Emotion)
		{
			if (Img_PlayPauseIcon_EmotionVideo && PlayIcon)
			{
				Img_PlayPauseIcon_EmotionVideo->SetBrushFromTexture(PlayIcon);
			}
			if (Slider_EmotionVideo)
			{
				Slider_EmotionVideo->SetValue(0.0f);
			}
		}
	}
	
}

void UArtifactInfoWidget::OnMediaOpened_AttachSound(FString OpenedUrl)
{
	UE_LOG(LogTemp, Warning, TEXT("EVENT: OnMediaOpened Fired! Attaching sound now. Url: %s"), *OpenedUrl);

	if (GlobalSoundManager && GlobalSoundManager->GetMediaSoundComponent())
	{
		// 이제 MediaPlayer가 완전히 준비되었으므로, 안전하게 사운드 컴포넌트에 연결합니다.
		GlobalSoundManager->GetMediaSoundComponent()->SetMediaPlayer(MediaPlayer);
	}

	// 이 이벤트는 한번만 필요하므로, 실행된 후에는 바로 바인딩을 해제하여 다음 영상 재생 시 중복 실행을 방지합니다.
	if (MediaPlayer)
	{
		MediaPlayer->OnMediaOpened.RemoveDynamic(this, &UArtifactInfoWidget::OnMediaOpened_AttachSound);
	}
}




// ===== 시청 시간 추적 핵심 함수들 =====


void UArtifactInfoWidget::StartPlaybackTracking()
{

	if (PlaybackStartTime == 0.f && MediaPlayer)
	{
		PlaybackStartTime = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Log, TEXT("Playback tracking STARTED."));
	}
}

void UArtifactInfoWidget::StopPlaybackTracking()
{
	if (PlaybackStartTime > 0.f)
	{
		const float ElapsedTime = GetWorld()->GetTimeSeconds() - PlaybackStartTime;
		AccumulatedPlaybackTime += ElapsedTime;
		PlaybackStartTime = 0.f; // 추적 중지 상태임을 의미
		UE_LOG(LogTemp, Log, TEXT("Playback tracking STOPPED. Elapsed: %.2f sec, Total: %.2f sec"), ElapsedTime, AccumulatedPlaybackTime);
	}
}


void UArtifactInfoWidget::ResetPlaybackTracking(UMediaSource* NewSource)
{
	
	StopPlaybackTracking();
	if (CurrentMediaSource && AccumulatedPlaybackTime > 0.1f)
	{
		UGameInstance* GI = GetGameInstance();
		if (GI)
		{
			UTrackingSubsystem* TrackingSubsystem = GI->GetSubsystem<UTrackingSubsystem>();
			if (TrackingSubsystem)
			{
				TrackingSubsystem->LogVideoPlay(AccumulatedPlaybackTime);
			}
		}
	}

	CurrentMediaSource = NewSource;
	AccumulatedPlaybackTime = 0.f;
	PlaybackStartTime = 0.f;
}