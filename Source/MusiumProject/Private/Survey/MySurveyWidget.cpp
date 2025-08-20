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
	// 1. �����͸� �з��Ͽ� ������ ������
    TArray<FString> DemographicData; // ���� ���� ������ (age, sex ��)
    TMap<FString, int32> FrequencyCounts; // ���� ���� ���� �󵵼� (Key: ����(1,2,3...), Value: Ƚ��)

    // 2. ĳ�õ� ��� ���� �г� ��ȸ
    for (UQuestionGroupPanel* Panel : CachedQuestionPanels)
    {
        UMyRadioButton* SelectedButton = FindSelectedRadioButtonInPanel(Panel);
        if (SelectedButton && !SelectedButton->RadioButtonID.IsEmpty())
        {
            // 3. �г� Ÿ�Կ� ���� ������ �и� ó��
            if (Panel->QuestionType == EQuestionPanelType::Demographic)
            {
                // ���� ���� �г��̸�, ID�� �״�� �迭�� �߰�
                DemographicData.Add(SelectedButton->RadioButtonID);
            }
            else if (Panel->QuestionType == EQuestionPanelType::Frequency)
            {
                // ���� ���� �г��̸�, ��(Map)�� �̿��� �󵵼� ���
                const FString& SelectedID = SelectedButton->RadioButtonID;
                // FindOrAdd: �ش� ID�� �ʿ� ������ ���� ���� ��������, ������ 0���� ���� �߰�
                // ++: ������ �� �Ǵ� ���� �߰��� 0�� 1 ������Ŵ
                FrequencyCounts.FindOrAdd(SelectedID)++;
            }
        }
    }

    // 4. ���� ���� ���õ� ���� ���� ���� ã��
    FString MostFrequentID = TEXT("N/A"); // ����� ���� ��츦 ���
    int32 MaxCount = 0;
    for (const TPair<FString, int32>& Pair : FrequencyCounts)
    {
        if (Pair.Value > MaxCount)
        {
            MaxCount = Pair.Value;
            MostFrequentID = Pair.Key;
        }
    }

    // 5. ��� �����͸� �����Ͽ� CSV �� �ٷ� �����
    // CSV ����� ���ο� ������ ������ �°� �����մϴ�.
    FString CSVHeader = TEXT("UserID,Timestamp,Age,Gender,VisitFrequency,Education,DominantPropensity\n");
    FString CSVRow = FString::Printf(TEXT("%s,%s,"), *InSaveSlot, *FDateTime::Now().ToIso8601());
    
    // ���� ���� �߰�
    CSVRow += FString::Join(DemographicData, TEXT(","));
    CSVRow += TEXT(","); // ���� ������ ���� �м� ��� ���̿� ��ǥ �߰�
    
    // ���� �󵵰� ���Ҵ� ���� �߰�
    CSVRow += MostFrequentID;
    CSVRow += TEXT("\n");

    // 6. ���� ���� �� ���� �̵� (���� �ڵ�� ����)
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
		// �̵��ϰ� ���� ������ �̸��� FName Ÿ������ ����
		// "MainMenuLevel"�� �����̸�, ���� ������Ʈ�� ���� �̸����� ����
		// ������ �������� �ִ� ���� �ּ��� �̸��� ��Ȯ�� ��ġ�ؾ� ��
		FName LevelToLoad = FName("MainLevel");

		// ������ ������ ���ϴ�.
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


