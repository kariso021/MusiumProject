// Fill out your copyright notice in the Description page of Project Settings.


#include "Survey/MySurveyWidget.h"
#include "Widget/MyRadioButton.h"
#include "Components/PanelWidget.h"
#include "QuestionGroupPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "TrackingData/TrackingSubsystem.h" 
#include "TrackingData/TrackingDataDefs.h"
#include "Animation/WidgetAnimation.h"
#include "TimerManager.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/ProgressBar.h>
#include "MyExitSurveyWidget.h"

void UMySurveyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    CachedQuestionPanels.Empty();

    if (!WidgetTree)
    {
        return;
    }

	ProgressBarAnimSpeed = 3.0f;

	if (SurveyProgressBar)
	{
		SurveyProgressBar->SetPercent(CurrentPercent);
	}

    TArray<UWidget*> AllWidgetsInTree;
    WidgetTree->GetAllWidgets(AllWidgetsInTree);

    for (UWidget* Widget : AllWidgetsInTree)
    {
        if (UQuestionGroupPanel* FoundPanel = Cast<UQuestionGroupPanel>(Widget))
        {
            CachedQuestionPanels.Add(FoundPanel);
        }
    }

	if (FrequencyQuestionDataTable)
	{
		FrequencyQuestionDataTable->GetAllRows<FSurveyFrequencyQuestionRow>(TEXT(""), FrequencyQuestionRows);
	}
	else
	{
		// 테이블이 할당되지 않았을 경우 경고 로그
		UE_LOG(LogTemp, Error, TEXT("FrequencyQuestionDataTable is NOT ASSIGNED in Blueprint!"));
	}

}



void UMySurveyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!FMath::IsNearlyEqual(CurrentPercent, TargetPercent))
	{
		// FInterpTo를 사용해 현재 값을 목표 값으로 부드럽게 이동
		CurrentPercent = FMath::FInterpTo(CurrentPercent, TargetPercent, InDeltaTime, ProgressBarAnimSpeed);

		// 실제 ProgressBar에 변경된 값을 적용
		if (SurveyProgressBar)
		{
			SurveyProgressBar->SetPercent(CurrentPercent);
		}
	}
}

UMyRadioButton* UMySurveyWidget::FindSelectedRadioButtonInPanel(UPanelWidget* PanelToSearch)
{
	if (!PanelToSearch) return nullptr;

	for (UWidget* ChildWidget : PanelToSearch->GetAllChildren())
	{
		if (UMyRadioButton* RadioButton = Cast<UMyRadioButton>(ChildWidget))
		{
			if (RadioButton->IsChecked())
			{
				return RadioButton;
			}
		}
		else if (UPanelWidget* ChildPanel = Cast<UPanelWidget>(ChildWidget))
		{
			UMyRadioButton* FoundButton = FindSelectedRadioButtonInPanel(ChildPanel);
			if (FoundButton)
			{
				return FoundButton;
			}
		}
	}
	return nullptr;
}






void UMySurveyWidget::SubmitSurvey()
{
	// --- 사전조사 데이터 수집 ---
	TArray<FString> DemographicData;
	for (UQuestionGroupPanel* Panel : CachedQuestionPanels)
	{
		// Panel이 유효하고, QuestionType이 Demographic일 때만 데이터를 수집
		if (Panel && Panel->QuestionType == EQuestionPanelType::Demographic)
		{
			if (UMyRadioButton* SelectedButton = FindSelectedRadioButtonInPanel(Panel))
			{
				if (!SelectedButton->RadioButtonID.IsEmpty())
				{
					DemographicData.Add(SelectedButton->RadioButtonID);
				}
			}
		}
	}

	// 성향조사(Frequency) 데이터는 이미 'CollectedFrequencyCounts'에 집계
	// 최종 결과(MostFrequentID)는 멤버 변수 'MostFrequentID'에 저장

	// ---Tracking Subsystem으로 데이터 전송 ---
	UTrackingSubsystem* TrackingSubsystem = GetGameInstance()->GetSubsystem<UTrackingSubsystem>();
	if (!TrackingSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SubmitSurvey: TrackingSubsystem is not available."));
		return;
	}

	FSurveyData SurveyResultData;
	const FString InSaveSlot = TEXT("DefaultUser"); // 사용자 ID는 여기서 정의하거나 다른 방식으로 가져옵니다.
	SurveyResultData.SessionID = InSaveSlot;

	// 사전조사 데이터 채우기
	if (DemographicData.Num() >= 4)
	{
		SurveyResultData.Age = DemographicData[0];
		SurveyResultData.Gender = DemographicData[1];
		SurveyResultData.VisitFrequency = DemographicData[2];
		SurveyResultData.Education = DemographicData[3];
	}

	// 성향조사 결과 채우기 (이미 계산된 멤버 변수 사용)
	SurveyResultData.DominantPropensity = MostFrequentID;

	TrackingSubsystem->StartNewSession(SurveyResultData);
	UE_LOG(LogTemp, Log, TEXT("Survey Submitted for User: %s. A new tracking session has started."), *InSaveSlot);

	// --- 3단계: 레벨 이동 ---
	UGameplayStatics::OpenLevel(GetWorld(), FName("Main"));

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
	}
}

void UMySurveyWidget::GoToNextPanel()
{
	//다음 패널로 넘어가는 함수
	if (SurveySwitcher)
	{
		int32 CurrentIndex = SurveySwitcher->GetActiveWidgetIndex();
		int32 TotalWidgets = SurveySwitcher->GetNumWidgets();
		if (CurrentIndex + 1 < TotalWidgets)
		{
			SurveySwitcher->SetActiveWidgetIndex(CurrentIndex + 1);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GoToNextPanel: Already at the last panel."));
		}
	}
}

void UMySurveyWidget::StartFrequencySurvey()
{
	if (SurveySwitcher) SurveySwitcher->SetActiveWidgetIndex(4); // 성향조사 패널로 전환 패널이 4번임

	CurrentFrequencyQuestionIndex =-1 ;
	CollectedFrequencyCounts.Empty();
	TransitionToNextFrequencyQuestion();
}

void UMySurveyWidget::TransitionToNextFrequencyQuestion()
{
	// 1. 질문 카운터 증가
	CurrentFrequencyQuestionIndex++;

	const int32 TotalQuestions = FrequencyQuestionRows.Num();
	if (TotalQuestions > 0)
	{
		// 2. ProgressBar 애니메이션 목표값 설정 (NativeTick에서 이 값을 보고 애니메이션 시작)
		TargetPercent = (float)CurrentFrequencyQuestionIndex / (float)TotalQuestions;

		// 진행 텍스트는 즉시 업데이트
		if (ProgressText)
		{
			const int32 DisplayQuestionNumber = FMath::Min(CurrentFrequencyQuestionIndex, TotalQuestions);
			FString ProgressString = FString::Printf(TEXT("%d / %d"), DisplayQuestionNumber, TotalQuestions);
			ProgressText->SetText(FText::FromString(ProgressString));
		}
	}

	// 3. 마지막 질문이었는지 확인 후 결과 화면으로 이동
	if (!FrequencyQuestionRows.IsValidIndex(CurrentFrequencyQuestionIndex))
	{
		// Fade Out 후 결과 화면을 보여주기 위해 타이머 사용
		if (FadeOut_QuestionAnimation)
		{
			if (CurrentFrequencyQuestionIndex != 0)
			{
				PlayAnimation(FadeOut_QuestionAnimation);
				GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &UMySurveyWidget::ShowResultScreen, FadeOut_QuestionAnimation->GetEndTime());
			}
		}
		else
		{
			ShowResultScreen();
		}
		return;
	}

	if (CurrentFrequencyQuestionIndex == 0)
	{
		OnFadeOutFinished_UpdateFrequencyQuestion();
	}
	else
	{
		if (FadeOut_QuestionAnimation)
		{
			PlayAnimation(FadeOut_QuestionAnimation);
			GetWorld()->GetTimerManager().SetTimer(
				TransitionTimerHandle,
				this,
				&UMySurveyWidget::OnFadeOutFinished_UpdateFrequencyQuestion,
				FadeOut_QuestionAnimation->GetEndTime()
			);
		}
		else
		{
			OnFadeOutFinished_UpdateFrequencyQuestion();
		}
	}
}


void UMySurveyWidget::OnFadeOutFinished_UpdateFrequencyQuestion()
{
	// 1. 현재 질문 인덱스에 맞는 데이터 행을 가져옵니다.
	const FSurveyFrequencyQuestionRow* Data = FrequencyQuestionRows[CurrentFrequencyQuestionIndex];
	if (!Data) return;

	// 2. 텍스트 등 위젯 내용을 업데이트합니다. (현재 화면은 투명한 상태)
	FrequencyQuestionText->SetText(FText::FromString(Data->QuestionText));

	auto UpdateTextState = [](UTextBlock* TextBlock, const FString& Text)
		{
			if (!TextBlock) return;
			const bool bIsVisible = !Text.IsEmpty();
			TextBlock->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
			if (bIsVisible)
			{
				TextBlock->SetText(FText::FromString(Text));
			}
		};
	UpdateTextState(FrequencyAnswerText1, Data->Answer1_Text);
	UpdateTextState(FrequencyAnswerText2, Data->Answer2_Text);
	UpdateTextState(FrequencyAnswerText3, Data->Answer3_Text);
	UpdateTextState(FrequencyAnswerText4, Data->Answer4_Text);
	UpdateTextState(FrequencyAnswerText5, Data->Answer5_Text);

	// 3. 내용 업데이트가 끝났으니, Fade In 애니메이션을 재생합니다.
	if (FadeIn_QuestionAnimation)
	{
		PlayAnimation(FadeIn_QuestionAnimation);
	}
}

void UMySurveyWidget::OnFrequencyAnswerClicked(int32 numbers)
{
	if (!FrequencyQuestionRows.IsValidIndex(CurrentFrequencyQuestionIndex)) return;

	const FSurveyFrequencyQuestionRow* CurrentQuestion = FrequencyQuestionRows[CurrentFrequencyQuestionIndex];
	FString SelectedAnswerID;

	// 클릭된 버튼 번호에 따라 적절한 AnswerID를 가져옴
	switch (numbers)
	{
	case 1: SelectedAnswerID = CurrentQuestion->Answer1_ID; break;
	case 2: SelectedAnswerID = CurrentQuestion->Answer2_ID; break;
	case 3: SelectedAnswerID = CurrentQuestion->Answer3_ID; break;
	case 4: SelectedAnswerID = CurrentQuestion->Answer4_ID; break;
	case 5: SelectedAnswerID = CurrentQuestion->Answer5_ID; break;
	default: break;
	}

	if (!SelectedAnswerID.IsEmpty())
	{
		HandleFrequencyAnswer(SelectedAnswerID);
	}
}

void UMySurveyWidget::HandleFrequencyAnswer(const FString& AnswerID)
{
	CollectedFrequencyCounts.FindOrAdd(AnswerID)++;
	TransitionToNextFrequencyQuestion();
}

void UMySurveyWidget::ShowResultScreen()
{
	// --- 1. MostFrequentID 계산 로직 (여기에 추가!) ---
	MostFrequentID = TEXT("N/A"); // 기본값으로 초기화
	int32 MaxCount = 0;
	for (const TPair<FString, int32>& Pair : CollectedFrequencyCounts)
	{
		// 현재까지의 최대 빈도수보다 더 높은 빈도수의 답변을 찾으면
		if (Pair.Value > MaxCount)
		{
			// 최대 빈도수와 ID를 갱신합니다.
			MaxCount = Pair.Value;
			MostFrequentID = Pair.Key; // 멤버 변수에 가장 많이 나온 ID를 저장
		}
	}

	// 현재는 먼저 발견된 최대값만 저장

	// --- 2. 계산된 ID로 결과 데이터 찾기 (기존 로직) ---
	if (ResultDataTable)
	{
		// 이제 MostFrequentID에 올바른 값이 들어있습니다.
		const FName RowName = FName(*MostFrequentID);
		FSurveyResultRow* ResultData = ResultDataTable->FindRow<FSurveyResultRow>(RowName, TEXT(""));

		if (ResultData)
		{
			if (ResultData->ResultImage.IsValid()) // Soft Ptr는 IsValid()로 확인하는 것이 더 안전합니다.
			{
				ResultImage->SetBrushFromTexture(ResultData->ResultImage.LoadSynchronous());
			}

			// 4. 위젯 스위처를 결과 패널로 전환
			if (SurveySwitcher)
			{
				SurveySwitcher->SetActiveWidgetIndex(5); // 결과 패널 인덱스로 설정
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ShowResultScreen: Could not find row with name '%s' in ResultDataTable."), *MostFrequentID);
		}
	}
}


