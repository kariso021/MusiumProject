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

void UMySurveyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    CachedQuestionPanels.Empty();

    if (!WidgetTree)
    {
        return;
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



void UMySurveyWidget::GoToNextPanel()
{
    if (!SurveySwitcher)
    {
        UE_LOG(LogTemp, Error, TEXT("GoToNextPanel: SurveySwitcher is not bound!"));
        return;
    }

    const int32 CurrentIndex = SurveySwitcher->GetActiveWidgetIndex();
    const int32 NumPanels = SurveySwitcher->GetNumWidgets();

    if (NumPanels == 0)
    {
        return;
    }

    if (CurrentIndex < NumPanels - 1)
    {
        UE_LOG(LogTemp, Log, TEXT("GoToNextPanel!"));
        const int32 NextIndex = CurrentIndex + 1;
        SurveySwitcher->SetActiveWidgetIndex(NextIndex);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("GoToNextPanel: Reached the last panel. Submitting survey..."));

        const FString FileName = TEXT("SurveyResult.csv");
        const FString SaveSlotName = TEXT("DefaultUser");
        SubmitSurvey(FileName, SaveSlotName);

    }
}



void UMySurveyWidget::SubmitSurvey(const FString& InFileName, const FString& InSaveSlot)
{
	// --- 1단계: 데이터 수집 (이 부분은 기존 코드와 동일하게 매우 잘 작성되어 있습니다) ---

	// 1a. 데이터를 분류하여 저장할 변수들
	TArray<FString> DemographicData;
	TMap<FString, int32> FrequencyCounts;

	// 1b. 캐시된 모든 질문 패널 순회하여 데이터 수집
	for (UQuestionGroupPanel* Panel : CachedQuestionPanels)
	{
		UMyRadioButton* SelectedButton = FindSelectedRadioButtonInPanel(Panel);
		if (SelectedButton && !SelectedButton->RadioButtonID.IsEmpty())
		{
			if (Panel->QuestionType == EQuestionPanelType::Demographic)
			{
				DemographicData.Add(SelectedButton->RadioButtonID);
			}
			else if (Panel->QuestionType == EQuestionPanelType::Frequency)
			{
				FrequencyCounts.FindOrAdd(SelectedButton->RadioButtonID)++;
			}
		}
	}

	// 1c. 응답이 하나도 없을 경우 함수 종료
	if (DemographicData.Num() == 0 && FrequencyCounts.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SubmitSurvey: No radio buttons were selected. Nothing to submit."));
		return;
	}

	// 1d. 가장 많이 선택된 성향 조사 응답 찾기
	FString MostFrequentID = TEXT("N/A");
	int32 MaxCount = 0;
	for (const TPair<FString, int32>& Pair : FrequencyCounts)
	{
		if (Pair.Value > MaxCount)
		{
			MaxCount = Pair.Value;
			MostFrequentID = Pair.Key;
		}
	}

	// --- 2단계: UTrackingSubsystem과 연동 (이 부분이 핵심 변경 사항입니다) ---

	// 2a. 트래킹 서브시스템 인스턴스 가져오기
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("SubmitSurvey: GameInstance is null."));
		return;
	}

	UTrackingSubsystem* TrackingSubsystem = GameInstance->GetSubsystem<UTrackingSubsystem>();
	if (!TrackingSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("SubmitSurvey: TrackingSubsystem is not available."));
		return;
	}

	// 2b. FSurveyData 구조체 생성 및 수집된 데이터로 채우기
	FSurveyData SurveyResultData;
	SurveyResultData.SessionID = InSaveSlot; // 사용자 ID 설정

	// 중요: DemographicData 배열의 순서가 UI 질문 순서와 일치한다고 가정합니다.
	// 예를 들어, [0]: Age, [1]: Gender, [2]: VisitFrequency, [3]: Education 순서
	if (DemographicData.Num() >= 4)
	{
		SurveyResultData.Age = DemographicData[0];
		SurveyResultData.Gender = DemographicData[1];
		SurveyResultData.VisitFrequency = DemographicData[2];
		SurveyResultData.Education = DemographicData[3];
	}

	SurveyResultData.DominantPropensity = MostFrequentID; // 가장 많이 선택된 성향 설정

	// 2c. 트래킹 서브시스템으로 데이터 전송 및 새 세션 시작 요청!
	TrackingSubsystem->StartNewSession(SurveyResultData);

	// 2d. (제거됨) 기존의 수동 CSV 파일 저장 로직은 모두 삭제합니다.
	// FString CSVHeader = ...
	// FString CSVRow = ...
	// FFileHelper::SaveStringToFile(...)

	// --- 3단계: 후처리 (레벨 이동 및 UI 정리, 기존 코드와 동일) ---

	UE_LOG(LogTemp, Log, TEXT("Survey Submitted for User: %s. A new tracking session has started."), *InSaveSlot);

	UWorld* World = GetWorld();
	if (World)
	{
		FName LevelToLoad = FName("MainLevel");
		UGameplayStatics::OpenLevel(World, LevelToLoad);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMySurveyWidget::SubmitSurvey - World is null. Cannot open new level."));
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		FInputModeGameOnly InputModeData;
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;
	}
}


