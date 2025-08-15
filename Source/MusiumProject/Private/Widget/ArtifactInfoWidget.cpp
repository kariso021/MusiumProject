// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ArtifactInfoWidget.h"
#include "Data/ArtifactData.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "MediaPlayer.h"
#include "MediaSource.h"


void UArtifactInfoWidget::NativeConstruct()
{

	Super::NativeConstruct();

	// 바인딩된 위젯들 초기화
	if (Btn_Basic) Btn_Basic->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnBasicClicked);
	if (Btn_Tech) Btn_Tech->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnTechClicked);
	if (Btn_Meaning) Btn_Meaning->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnMeaningClicked);
	if (Btn_Similar) Btn_Similar->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnSimilarClicked);
	if (Btn_Video) Btn_Video->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnVideoClicked);
	if (Btn_Emotion) Btn_Emotion->OnClicked.AddDynamic(this, &UArtifactInfoWidget::OnBtnEmotionClicked);
}


void UArtifactInfoWidget::SetData(const FArtifactData& Data)
{
    if (NameText)        NameText->SetText(Data.Name);
    if (EraText)         EraText->SetText(Data.Era);
    if (OriginText)      OriginText->SetText(Data.Origin);
    if (DescriptionText) DescriptionText->SetText(Data.Description);
	if (TechText)        TechText->SetText(Data.TechniqueDescription);
	if (PatternMeaningText) PatternMeaningText->SetText(Data.PatternMeaning);

	// 애셋 경로를 멤버 변수에 저장
	ThumbnailSource = Data.Thumbnail;
	DescriptionVideoSource = Data.DecriptionVideoSource;
	EmotionVideoSource = Data.EmotionVideoSource;

	if (!ThumbnailSource.IsNull())
	{
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(ThumbnailSource.ToSoftObjectPath(),
			// 람다 사용: 로드 완료 시 실행할 코드를 여기에 바로 작성
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




// 판넬 스위칭
void UArtifactInfoWidget::SwitchTo(UWidget* Panel)
{
	if (!Switch_Content || !Panel) return;
	const int32 Index = Switch_Content->GetChildIndex(Panel);
	if (Index != INDEX_NONE) Switch_Content->SetActiveWidgetIndex(Index);
}



void UArtifactInfoWidget::OnBtnBasicClicked()
{
	SwitchTo(Panel_Basic);	
}

void UArtifactInfoWidget::OnBtnTechClicked()
{
	SwitchTo(Panel_Tech);
}

void UArtifactInfoWidget::OnBtnMeaningClicked()
{
	SwitchTo(Panel_Meaning);
}

void UArtifactInfoWidget::OnBtnSimilarClicked()
{
	SwitchTo(Panel_Similar);
}

void UArtifactInfoWidget::OnBtnVideoClicked()
{
	SwitchTo(Panel_Video);

	if (MediaPlayer && !DescriptionVideoSource.IsNull())
	{
		// 람다를 사용하여 비동기 로드 요청
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(DescriptionVideoSource.ToSoftObjectPath(),
			[this]()
			{
				UMediaSource* LoadedSource = DescriptionVideoSource.Get();
				if (MediaPlayer && LoadedSource)
				{
					MediaPlayer->OpenSource(LoadedSource);
					UE_LOG(LogTemp, Log, TEXT("Description Video Source Opened via Lambda."));
				}
			});
	}
}

void UArtifactInfoWidget::OnBtnEmotionClicked()
{
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
					UE_LOG(LogTemp, Log, TEXT("Emotion Video Source Opened via Lambda."));
				}
			});
	}
}

