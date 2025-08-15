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

	// 2. 모든 위젯을 순회하며 UQuestionGroupPanel 타입인지 직접 Cast로 확인합니다.
	for (UWidget* Widget : AllWidgetsInTree)
	{
		// 3. Cast를 통해 타입을 확인합니다. 성공하면 FoundPanel은 유효한 포인터가 됩니다.
		if (UQuestionGroupPanel* FoundPanel = Cast<UQuestionGroupPanel>(Widget))
		{
			// 4. 타입이 맞다면, 배열에 바로 추가합니다.
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

	// 캐시된 질문 그룹 패널들을 순회
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

	// CSV 저장 로직
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
		// 이동하고 싶은 레벨의 이름을 FName 타입으로 지정합니다.
		// "MainMenuLevel"은 예시이며, 실제 프로젝트의 레벨 이름으로 변경해야 합니다.
		// 콘텐츠 브라우저에 있는 레벨 애셋의 이름과 정확히 일치해야 합니다.
		FName LevelToLoad = FName("MainLevel");

		// 지정된 레벨을 엽니다.
		UGameplayStatics::OpenLevel(World, LevelToLoad);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMySurveyWidget::SubmitSurvey - World is null. Cannot open new level."));
	}

}


