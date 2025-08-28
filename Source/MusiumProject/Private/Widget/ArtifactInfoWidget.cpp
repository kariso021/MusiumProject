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

void UArtifactInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯들 바인딩
	if (Btn_Basic) Btn_Basic->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnBasicClicked);
	if (Btn_Tech) Btn_Tech->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnTechClicked);
	if (Btn_Meaning) Btn_Meaning->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnMeaningClicked);
	if (Btn_Similar) Btn_Similar->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnSimilarClicked);
	if (Btn_Video) Btn_Video->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnVideoClicked);
	if (Btn_Emotion) Btn_Emotion->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnEmotionClicked);
	if (Btn_PlayPause) Btn_PlayPause->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnPlayPauseClicked);

	// 볼륨 슬라이더 설정
	if (Slider_Volume)
	{
		Slider_Volume->SetValue(1.0f);
		Slider_Volume->OnValueChanged.AddDynamic(this, &UArtifactInfoWidget::OnVolumeChanged);
		if (MediaSoundComponent)
		{
			MediaSoundComponent->SetVolumeMultiplier(1.0f);
		}
	}

	// 비디오 탐색 슬라이더 설정
	if (Slider_Video)
	{
		Slider_Video->OnMouseCaptureBegin.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderMouseCaptureBegin);
		Slider_Video->OnMouseCaptureEnd.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderMouseCaptureEnd);
		Slider_Video->OnValueChanged.AddDynamic(this, &UArtifactInfoWidget::OnVideoSliderValueChanged);
	}

	// ===== 핵심 변경 사항: MediaPlayer 이벤트에 함수 바인딩 =====
	// MediaPlayer의 상태가 바뀔 때마다 해당 함수들이 자동으로 호출됩니다.
	if (MediaPlayer)
	{
		MediaPlayer->OnPlaybackResumed.AddUniqueDynamic(this, &UArtifactInfoWidget::HandlePlaybackResumed);
		MediaPlayer->OnPlaybackSuspended.AddUniqueDynamic(this, &UArtifactInfoWidget::HandlePlaybackPaused);
		MediaPlayer->OnEndReached.AddUniqueDynamic(this, &UArtifactInfoWidget::HandlePlaybackEnded);
	}
}

void UArtifactInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 유저가 슬라이더를 조작하지 않을 때만 재생 시간에 맞춰 슬라이더 위치 업데이트
	if (MediaPlayer && MediaPlayer->IsPlaying() && !bIsSeeking && Slider_Video)
	{
		const FTimespan Duration = MediaPlayer->GetDuration();
		if (Duration.GetTotalSeconds() > 0)
		{
			const FTimespan CurrentTime = MediaPlayer->GetTime();
			Slider_Video->SetValue(CurrentTime.GetTotalSeconds() / Duration.GetTotalSeconds());
		}
	}
}

void UArtifactInfoWidget::SetData(const FArtifactData& Data)
{
	if (NameText) NameText->SetText(Data.Name);
	if (EraText) EraText->SetText(Data.Era);
	if (OriginText) OriginText->SetText(Data.Origin);
	if (DescriptionText) DescriptionText->SetText(Data.Description);
	if (TechText) TechText->SetText(Data.TechniqueDescription);
	if (PatternMeaningText) PatternMeaningText->SetText(Data.PatternMeaning);

	ThumbnailSource = Data.Thumbnail;
	DescriptionVideoSource = Data.DecriptionVideoSource;
	EmotionVideoSource = Data.EmotionVideoSource;

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
	if ((CurrentPanel == Panel_Video || CurrentPanel == Panel_Emotion) &&
		(Panel != Panel_Video && Panel != Panel_Emotion))
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
	SwitchTo(Panel_Basic);
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
	SwitchTo(Panel_Tech);
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
	SwitchTo(Panel_Meaning);
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
	SwitchTo(Panel_Similar);
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

// ===== 변경됨: 비디오 버튼 클릭 핸들러 =====
void UArtifactInfoWidget::OnBtnVideoClicked()
{
	// 1. 이전 영상의 시청 기록이 있다면 저장하고, 새 영상 기록을 위해 리셋합니다.
	ResetPlaybackTracking(DescriptionVideoSource.Get());

	// 2. 비디오 패널로 전환합니다.
	SwitchTo(Panel_Video);

	// 3. 비디오 소스를 비동기 로드하고 엽니다.
	// StartPlaybackTracking()을 직접 호출하지 않습니다. OnPlaybackResumed 이벤트가 자동으로 처리합니다.
	if (MediaPlayer && !DescriptionVideoSource.IsNull())
	{
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(DescriptionVideoSource.ToSoftObjectPath(),
			[this]()
			{
				UMediaSource* LoadedSource = DescriptionVideoSource.Get();
				if (MediaPlayer && LoadedSource)
				{
					MediaPlayer->OpenSource(LoadedSource);
					if (Img_PlayPauseIcon && PauseIcon)
					{
						Img_PlayPauseIcon->SetBrushFromTexture(PauseIcon);
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
			TrackingSubsystem->LogClick("VideoButton");
		}
	}
}

void UArtifactInfoWidget::OnBtnEmotionClicked()
{
	ResetPlaybackTracking(EmotionVideoSource.Get());
	SwitchTo(Panel_Emotion);

	if (MediaPlayer && !EmotionVideoSource.IsNull())
	{
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(EmotionVideoSource.ToSoftObjectPath(),
			[this]()
			{
				UMediaSource* LoadedSource = EmotionVideoSource.Get();
				if (MediaPlayer && LoadedSource)
				{
					MediaPlayer->OpenSource(LoadedSource);
					if (Img_PlayPauseIcon && PauseIcon)
					{
						Img_PlayPauseIcon->SetBrushFromTexture(PauseIcon);
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

// ===== 변경됨: OnPlayPauseClicked 함수 대폭 단순화 =====
void UArtifactInfoWidget::OnPlayPauseClicked()
{
	if (!MediaPlayer) return;

	// Start/StopPlaybackTracking을 직접 호출할 필요가 없습니다.
	// Play/Pause 명령에 따라 이벤트가 발생하여 자동으로 처리됩니다.
	if (MediaPlayer->IsPlaying())
	{
		MediaPlayer->Pause();
		if (Img_PlayPauseIcon && PlayIcon)
		{
			Img_PlayPauseIcon->SetBrushFromTexture(PlayIcon);
		}
	}
	else
	{
		MediaPlayer->Play();
		if (Img_PlayPauseIcon && PauseIcon)
		{
			Img_PlayPauseIcon->SetBrushFromTexture(PauseIcon);
		}
	}
}

void UArtifactInfoWidget::OnVolumeChanged(float Value)
{
	if (MediaSoundComponent)
	{
		MediaSoundComponent->SetVolumeMultiplier(Value);
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
	if (Img_PlayPauseIcon && PlayIcon)
	{
		Img_PlayPauseIcon->SetBrushFromTexture(PlayIcon);
	}
	if (Slider_Video)
	{
		Slider_Video->SetValue(0.0f);
	}
	
}


// ===== 시청 시간 추적 핵심 함수들 =====

// 변경됨: StartPlaybackTracking 함수에서 IsPlaying() 체크 제거
void UArtifactInfoWidget::StartPlaybackTracking()
{
	// 이 함수를 호출한다는 것은 재생을 시작하려는 의도가 명확하므로 IsPlaying 체크는 불필요합니다.
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

// 이 함수의 역할은 여전히 중요합니다 (영상 세션 관리)
void UArtifactInfoWidget::ResetPlaybackTracking(UMediaSource* NewSource)
{
	// 1. 이전 영상의 추적을 멈추고 기록을 최종 저장합니다.
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

	// 2. 새 영상을 위해 모든 변수를 초기화합니다.
	CurrentMediaSource = NewSource;
	AccumulatedPlaybackTime = 0.f;
	PlaybackStartTime = 0.f;
	UE_LOG(LogTemp, Log, TEXT("Playback tracking RESET for new source: %s"), *GetNameSafe(NewSource));
}