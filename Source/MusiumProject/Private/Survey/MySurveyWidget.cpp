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

	// 2. ��� ������ ��ȸ�ϸ� UQuestionGroupPanel Ÿ������ ���� Cast�� Ȯ���մϴ�.
	for (UWidget* Widget : AllWidgetsInTree)
	{
		// 3. Cast�� ���� Ÿ���� Ȯ���մϴ�. �����ϸ� FoundPanel�� ��ȿ�� �����Ͱ� �˴ϴ�.
		if (UQuestionGroupPanel* FoundPanel = Cast<UQuestionGroupPanel>(Widget))
		{
			// 4. Ÿ���� �´ٸ�, �迭�� �ٷ� �߰��մϴ�.
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
	TArray<FString> SelectedIDs;

	// ĳ�õ� ���� �׷� �гε��� ��ȸ
	for (UPanelWidget* Panel : CachedQuestionPanels)
	{
		UMyRadioButton* SelectedButton = FindSelectedRadioButtonInPanel(Panel);
		if (SelectedButton && !SelectedButton->RadioButtonID.IsEmpty())
		{
			SelectedIDs.Add(SelectedButton->RadioButtonID);
		}
	}

	if (SelectedIDs.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SubmitSurvey: No radio buttons were selected."));
		return;
	}

	// CSV ���� ����
	FString CSVHeader = TEXT("UserID,Timestamp,Age,Gender,VisitFrequency,Education\n");
	FString CSVRow = FString::Printf(TEXT("%s,%s,"), *InSaveSlot, *FDateTime::Now().ToIso8601());
	CSVRow += FString::Join(SelectedIDs, TEXT(","));
	CSVRow += TEXT("\n");
	FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("Surveys/");
	FString FullPath = SaveDirectory + InFileName;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	bool bFileExists = PlatformFile.FileExists(*FullPath);
	if (!PlatformFile.DirectoryExists(*SaveDirectory)) { PlatformFile.CreateDirectory(*SaveDirectory); }
	FString ContentToSave = bFileExists ? CSVRow : CSVHeader + CSVRow;
	FFileHelper::SaveStringToFile(ContentToSave, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), FILEWRITE_Append);

	UE_LOG(LogTemp, Log, TEXT("Survey saved to: %s"), *FullPath);


	UWorld* World = GetWorld();
	if (World)
	{
		// �̵��ϰ� ���� ������ �̸��� FName Ÿ������ �����մϴ�.
		// "MainMenuLevel"�� �����̸�, ���� ������Ʈ�� ���� �̸����� �����ؾ� �մϴ�.
		// ������ �������� �ִ� ���� �ּ��� �̸��� ��Ȯ�� ��ġ�ؾ� �մϴ�.
		FName LevelToLoad = FName("MainLevel");

		// ������ ������ ���ϴ�.
		UGameplayStatics::OpenLevel(World, LevelToLoad);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMySurveyWidget::SubmitSurvey - World is null. Cannot open new level."));
	}

}


