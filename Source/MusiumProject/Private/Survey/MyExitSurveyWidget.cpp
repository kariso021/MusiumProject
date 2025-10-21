// Fill out your copyright notice in the Description page of Project Settings.


#include "Survey/MyExitSurveyWidget.h"

// ===== .h에서 전방 선언한 클래스들의 헤더를 여기에 포함 =====
#include "Components/WidgetSwitcher.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Widget/MyRadioButton.h"

// ===== 파일 저장 등 추가 기능에 필요한 헤더를 포함 =====
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"

void UMyExitSurveyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ProgressBarAnimSpeed = 3.0f;
	TargetPercent = 0.1f;
	CurrentPercent = 0.0f;

	if (SurveyProgressBar)
	{
		SurveyProgressBar->SetPercent(CurrentPercent);
	}
}

void UMyExitSurveyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);



	// 현재 값(CurrentPercent)과 목표 값(TargetPercent)이 다른 경우에만 실행
	if (!FMath::IsNearlyEqual(CurrentPercent, TargetPercent))
	{
		CurrentPercent = FMath::FInterpTo(CurrentPercent, TargetPercent, InDeltaTime, ProgressBarAnimSpeed);

		if (SurveyProgressBar)
		{
			SurveyProgressBar->SetPercent(CurrentPercent);
		}
	}
}

void UMyExitSurveyWidget::GoToNext_MainSurveyStep()
{
	if (MainSurveySwitcher)
	{
		// 현재 활성화된 위젯의 인덱스를 가져옵니다.
		int32 CurrentIndex = MainSurveySwitcher->GetActiveWidgetIndex();
		int32 NextIndex = CurrentIndex + 1;

		// 다음 인덱스가 전체 위젯 개수보다 작은지 확인 (마지막 페이지가 아닌지 체크)
		if (NextIndex < MainSurveySwitcher->GetNumWidgets())
		{
			MainSurveySwitcher->SetActiveWidgetIndex(NextIndex);
		}
	}
}

void UMyExitSurveyWidget::StartSelectSurveyStep()
{
	if (MainSurveySwitcher)
	{
		MainSurveySwitcher->SetActiveWidgetIndex(1); // 설문조사 선택 화면으로 전환
	}
	if (SelectSurveySwitcher)
	{
		SelectSurveySwitcher->SetActiveWidgetIndex(0); // 첫 번째 설문조사 단계로 초기화
		UpdateProgress(0, SelectSurveySwitcher->GetNumWidgets());
		UpdateNavigationButtons();
	}
}



void UMyExitSurveyWidget::GoToNext_SelectSurveyStep()
{
	if (SelectSurveySwitcher)
	{
		int32 CurrentIndex = SelectSurveySwitcher->GetActiveWidgetIndex();
		int32 NextIndex = CurrentIndex + 1;
		int32 TotalWidgets = SelectSurveySwitcher->GetNumWidgets();

		if (NextIndex < TotalWidgets)
		{
			SelectSurveySwitcher->SetActiveWidgetIndex(NextIndex);
			UpdateProgress(NextIndex, TotalWidgets);
			UpdateNavigationButtons();
		}
	}
}

void UMyExitSurveyWidget::GoToPrevious_SelectSurveyStep()
{
	if (SelectSurveySwitcher)
	{
		int32 CurrentIndex = SelectSurveySwitcher->GetActiveWidgetIndex();
		int32 PrevIndex = CurrentIndex - 1;
		int32 TotalWidgets = SelectSurveySwitcher->GetNumWidgets();

		// 이전 인덱스가 0보다 크거나 같은지 확인 (첫 페이지가 아닌지 체크)
		if (PrevIndex >= 0)
		{
			SelectSurveySwitcher->SetActiveWidgetIndex(PrevIndex);
			UpdateProgress(PrevIndex, TotalWidgets);
			UpdateNavigationButtons();
		}
	}
}

void UMyExitSurveyWidget::UpdateProgress(int32 CurrentIndex, int32 TotalCount)
{
	if (!ProgressText) // SurveyProgressBar 체크는 Tick에서 하므로 여기선 없어도 무방
	{
		return;
	}


	TargetPercent = static_cast<float>(CurrentIndex + 1) / static_cast<float>(TotalCount);

	// 2. 텍스트는 즉시 업데이트
	const FString ProgressString = FString::Printf(TEXT("%d / %d"), CurrentIndex + 1, TotalCount);
	ProgressText->SetText(FText::FromString(ProgressString));
}


UMyRadioButton* UMyExitSurveyWidget::FindSelectedRadioButtonInPanel(UPanelWidget* PanelToSearch)
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

TArray<FString> UMyExitSurveyWidget::FindAllSelectedRadioButtonIDsInPanel(UPanelWidget* PanelToSearch)
{
	TArray<FString> SelectedIDs;
	if (!PanelToSearch) return SelectedIDs;
	for (UWidget* ChildWidget : PanelToSearch->GetAllChildren())
	{
		if (UMyRadioButton* RadioButton = Cast<UMyRadioButton>(ChildWidget))
		{
			if (RadioButton->IsChecked())
			{
				SelectedIDs.Add(RadioButton->RadioButtonID); // 찾은 후에도 계속 탐색
			}
		}
		else if (UPanelWidget* ChildPanel = Cast<UPanelWidget>(ChildWidget))
		{
			SelectedIDs.Append(FindAllSelectedRadioButtonIDsInPanel(ChildPanel));
		}
	}
	return SelectedIDs;
}

void UMyExitSurveyWidget::UpdateNavigationButtons()
{
	if (!SelectSurveySwitcher || !PreviousButton || !NextButton)
	{
		return;
	}

	int32 CurrentIndex = SelectSurveySwitcher->GetActiveWidgetIndex();

	PreviousButton->SetVisibility(CurrentIndex == 0 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	NextButton->SetVisibility(CurrentIndex == 5 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UMyExitSurveyWidget::SubmitExitSurvey(const FString& FinalFeedbackText)
{
	// 단일 선택 질문
	FString Answer01, Answer02, Answer03;
	if (UMyRadioButton* SelectedBtn = FindSelectedRadioButtonInPanel(ExitQuestionPanel_01))
	{
		Answer01 = SelectedBtn->RadioButtonID;
	}
	if (UMyRadioButton* SelectedBtn = FindSelectedRadioButtonInPanel(ExitQuestionPanel_02))
	{
		Answer02 = SelectedBtn->RadioButtonID;
	}
	if (UMyRadioButton* SelectedBtn = FindSelectedRadioButtonInPanel(ExitQuestionPanel_03))
	{
		Answer03 = SelectedBtn->RadioButtonID;
	}

	// 복수선택 질문
	TArray<FString> Answers04 = FindAllSelectedRadioButtonIDsInPanel(ExitQuestionPanel_04);
	TArray<FString> Answers05 = FindAllSelectedRadioButtonIDsInPanel(ExitQuestionPanel_05);
	TArray<FString> Answers06 = FindAllSelectedRadioButtonIDsInPanel(ExitQuestionPanel_06);

	FString FeedbackText = FinalFeedbackText;

	// CSV 저장을 위해 줄바꿈 문자 등을 처리하는 것은 동일
	FeedbackText.ReplaceInline(TEXT("\n"), TEXT(" "));
	FeedbackText.ReplaceInline(TEXT("\r"), TEXT(""));

	// CSV 파일로 저장
	// 복수 선택 결과는 쉼표(,)로 구분된 하나의 문자열로
	const FString Answers04_Str = FString::Join(Answers04, TEXT(";")); // 내부 구분자는 세미콜론으로 변경
	const FString Answers05_Str = FString::Join(Answers05, TEXT(";"));
	const FString Answers06_Str = FString::Join(Answers06, TEXT(";"));

	// 파일 경로 및 이름 설정
	const FString FilePath = FPaths::ProjectSavedDir() + TEXT("SurveyResults/ExitSurveyResults.csv");

	// 헤더 행 (파일이 없을 때만 추가)
	const FString HeaderRow = TEXT("Q1,Q2,Q3,Q4_Multi,Q5_Multi,Q6_Multi,Feedback\n");
	if (!FPaths::FileExists(FilePath))
	{
		FFileHelper::SaveStringToFile(HeaderRow, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);
	}

	// 데이터 행 생성
	const FString DataRow = FString::Printf(TEXT("%s,%s,%s,\"%s\",\"%s\",\"%s\",\"%s\"\n"),
		*Answer01,
		*Answer02,
		*Answer03,
		*Answers04_Str,
		*Answers05_Str,
		*Answers06_Str,
		*FeedbackText
	);

	// 파일에 데이터 행 추가
	if (FFileHelper::SaveStringToFile(DataRow, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM, &IFileManager::Get(), FILEWRITE_Append))
	{
		UE_LOG(LogTemp, Log, TEXT("Survey data appended successfully to: %s"), *FilePath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to append survey data to file."));
	}

	// TODO: 설문 완료 후 처리
}
