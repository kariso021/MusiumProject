// Fill out your copyright notice in the Description page of Project Settings.


#include "Survey/MySurveyWidget.h"
#include "Widget/MyRadioButton.h"
#include "Components/PanelWidget.h"
#include "QuestionGroupPanel.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

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

void UMySurveyWidget::SubmitSurvey(const FString& InFileName, const FString& InSaveSlot)
{
	// 1. 데이터를 분류하여 저장할 변수들
    TArray<FString> DemographicData; // 사전 정보 데이터 (age, sex 등)
    TMap<FString, int32> FrequencyCounts; // 성향 조사 응답 빈도수 (Key: 응답(1,2,3...), Value: 횟수)

    // 2. 캐시된 모든 질문 패널 순회
    for (UQuestionGroupPanel* Panel : CachedQuestionPanels)
    {
        UMyRadioButton* SelectedButton = FindSelectedRadioButtonInPanel(Panel);
        if (SelectedButton && !SelectedButton->RadioButtonID.IsEmpty())
        {
            // 3. 패널 타입에 따라 데이터 분리 처리
            if (Panel->QuestionType == EQuestionPanelType::Demographic)
            {
                // 사전 정보 패널이면, ID를 그대로 배열에 추가
                DemographicData.Add(SelectedButton->RadioButtonID);
            }
            else if (Panel->QuestionType == EQuestionPanelType::Frequency)
            {
                // 성향 조사 패널이면, 맵(Map)을 이용해 빈도수 계산
                const FString& SelectedID = SelectedButton->RadioButtonID;
                // FindOrAdd: 해당 ID가 맵에 있으면 기존 값을 가져오고, 없으면 0으로 새로 추가
                // ++: 가져온 값 또는 새로 추가된 0을 1 증가시킴
                FrequencyCounts.FindOrAdd(SelectedID)++;
            }
        }
    }

    // 4. 가장 많이 선택된 성향 조사 응답 찾기
    FString MostFrequentID = TEXT("N/A"); // 결과가 없을 경우를 대비
    int32 MaxCount = 0;
    for (const TPair<FString, int32>& Pair : FrequencyCounts)
    {
        if (Pair.Value > MaxCount)
        {
            MaxCount = Pair.Value;
            MostFrequentID = Pair.Key;
        }
    }

    // 5. 모든 데이터를 취합하여 CSV 한 줄로 만들기
    // CSV 헤더도 새로운 데이터 구조에 맞게 수정합니다.
    FString CSVHeader = TEXT("UserID,Timestamp,Age,Gender,VisitFrequency,Education,DominantPropensity\n");
    FString CSVRow = FString::Printf(TEXT("%s,%s,"), *InSaveSlot, *FDateTime::Now().ToIso8601());
    
    // 사전 정보 추가
    CSVRow += FString::Join(DemographicData, TEXT(","));
    CSVRow += TEXT(","); // 사전 정보와 성향 분석 결과 사이에 쉼표 추가
    
    // 가장 빈도가 높았던 성향 추가
    CSVRow += MostFrequentID;
    CSVRow += TEXT("\n");

    // 6. 파일 저장 및 레벨 이동 (기존 코드와 동일)
    if (DemographicData.Num() == 0 && MaxCount == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SubmitFullSurvey: No radio buttons were selected."));
        return;
    }

    FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("Surveys/");
    FString FullPath = SaveDirectory + InFileName;
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    if (!PlatformFile.DirectoryExists(*SaveDirectory)) { PlatformFile.CreateDirectory(*SaveDirectory); }
    FString ContentToSave = PlatformFile.FileExists(*FullPath) ? CSVRow : CSVHeader + CSVRow;
    FFileHelper::SaveStringToFile(ContentToSave, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), FILEWRITE_Append);

    UE_LOG(LogTemp, Log, TEXT("Survey saved to: %s"), *FullPath);
    UE_LOG(LogTemp, Log, TEXT("Dominant Propensity: %s (Count: %d)"), *MostFrequentID, MaxCount);



	UWorld* World = GetWorld();
	if (World)
	{
		// 이동하고 싶은 레벨의 이름을 FName 타입으로 지정
		// "MainMenuLevel"은 예시이며, 실제 프로젝트의 레벨 이름으로 변경
		// 콘텐츠 브라우저에 있는 레벨 애셋의 이름과 정확히 일치해야 함
		FName LevelToLoad = FName("MainLevel");

		// 지정된 레벨을 엽니다.
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


