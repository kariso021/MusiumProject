// Fill out your copyright notice in the Description page of Project Settings.


#include "SurveyFunctionLibrary.h"
#include "Widget/MyRadioButton.h"
#include "Misc/FileHelper.h" // 파일 저장을 위해 필요
#include "HAL/PlatformFileManager.h" // 파일 경로를 위해 필요
#include "Kismet/KismetSystemLibrary.h" //
#include "Components/PanelWidget.h"

void USurveyFunctionLibrary::SaveSelectedRadioTagsToCSV(UWidget* InParentWidget, const FString& InFileName, const FString& InSaveSlot)
{
    if (!InParentWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveSelectedRadioTagsToCSV: InParentWidget is null."));
        return;
    }

    // 1. 선택된 태그들을 수집할 배열
    TArray<FString> SelectedTags;

    // 2. 부모 위젯의 모든 자식 위젯을 순회하며 체크된 라디오 버튼 찾기
    // UWidget은 GetAllChildren을 직접 지원하지 않으므로, PanelWidget으로 캐스팅 시도
    UPanelWidget* ParentPanel = Cast<UPanelWidget>(InParentWidget);
    if (ParentPanel)
    {
        for (UWidget* ChildWidget : ParentPanel->GetAllChildren())
        {
            // 각 질문 그룹(Vertical Box) 안을 다시 순회
            UPanelWidget* QuestionGroupPanel = Cast<UPanelWidget>(ChildWidget);
            if (QuestionGroupPanel)
            {
                for (UWidget* RadioButtonWidget : QuestionGroupPanel->GetAllChildren())
                {
                    UMyRadioButton* RadioButton = Cast<UMyRadioButton>(RadioButtonWidget);
                    if (RadioButton && RadioButton->IsChecked())
                    {
                        // 태그가 있다면 첫 번째 태그를 가져와 배열에 추가
                        if (!RadioButton->RadioButtonID.IsEmpty())
                        {
                            SelectedTags.Add(RadioButton->RadioButtonID);
                        }
                    }
                }
            }
        }
    }

    // 3. CSV 형식의 문자열 만들기
    if (SelectedTags.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveSelectedRadioTagsToCSV: No radio buttons were selected."));
        return;
    }

    FString CSVHeader = TEXT("UserID,Timestamp,Age,Gender,VisitFrequency,Education\n"); // 예시 헤더
    FString CSVRow = FString::Printf(TEXT("%s,%s,"), *InSaveSlot, *FDateTime::Now().ToIso8601());

    // 태그를 쉼표로 구분하여 한 줄로 합침
    CSVRow += FString::Join(SelectedTags, TEXT(","));
    CSVRow += TEXT("\n"); // 줄바꿈 추가

    // 4. 파일 경로 설정 및 저장
    FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("Surveys/");
    FString FullPath = SaveDirectory + InFileName;

    // 파일이 이미 존재하는지 확인
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    bool bFileExists = PlatformFile.FileExists(*FullPath);

    // 폴더가 없으면 생성
    if (!PlatformFile.DirectoryExists(*SaveDirectory))
    {
        PlatformFile.CreateDirectory(*SaveDirectory);
    }

    // 파일이 없으면 헤더를 추가하고, 있으면 내용만 추가 (Append)
    FString ContentToSave = bFileExists ? CSVRow : CSVHeader + CSVRow;

    FFileHelper::SaveStringToFile(ContentToSave, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), FILEWRITE_Append);

    UE_LOG(LogTemp, Log, TEXT("Survey saved to: %s"), *FullPath);
}
