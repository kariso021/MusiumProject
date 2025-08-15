// Fill out your copyright notice in the Description page of Project Settings.


#include "SurveyFunctionLibrary.h"
#include "Widget/MyRadioButton.h"
#include "Misc/FileHelper.h" // ���� ������ ���� �ʿ�
#include "HAL/PlatformFileManager.h" // ���� ��θ� ���� �ʿ�
#include "Kismet/KismetSystemLibrary.h" //
#include "Components/PanelWidget.h"

void USurveyFunctionLibrary::SaveSelectedRadioTagsToCSV(UWidget* InParentWidget, const FString& InFileName, const FString& InSaveSlot)
{
    if (!InParentWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveSelectedRadioTagsToCSV: InParentWidget is null."));
        return;
    }

    // 1. ���õ� �±׵��� ������ �迭
    TArray<FString> SelectedTags;

    // 2. �θ� ������ ��� �ڽ� ������ ��ȸ�ϸ� üũ�� ���� ��ư ã��
    // UWidget�� GetAllChildren�� ���� �������� �����Ƿ�, PanelWidget���� ĳ���� �õ�
    UPanelWidget* ParentPanel = Cast<UPanelWidget>(InParentWidget);
    if (ParentPanel)
    {
        for (UWidget* ChildWidget : ParentPanel->GetAllChildren())
        {
            // �� ���� �׷�(Vertical Box) ���� �ٽ� ��ȸ
            UPanelWidget* QuestionGroupPanel = Cast<UPanelWidget>(ChildWidget);
            if (QuestionGroupPanel)
            {
                for (UWidget* RadioButtonWidget : QuestionGroupPanel->GetAllChildren())
                {
                    UMyRadioButton* RadioButton = Cast<UMyRadioButton>(RadioButtonWidget);
                    if (RadioButton && RadioButton->IsChecked())
                    {
                        // �±װ� �ִٸ� ù ��° �±׸� ������ �迭�� �߰�
                        if (!RadioButton->RadioButtonID.IsEmpty())
                        {
                            SelectedTags.Add(RadioButton->RadioButtonID);
                        }
                    }
                }
            }
        }
    }

    // 3. CSV ������ ���ڿ� �����
    if (SelectedTags.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveSelectedRadioTagsToCSV: No radio buttons were selected."));
        return;
    }

    FString CSVHeader = TEXT("UserID,Timestamp,Age,Gender,VisitFrequency,Education\n"); // ���� ���
    FString CSVRow = FString::Printf(TEXT("%s,%s,"), *InSaveSlot, *FDateTime::Now().ToIso8601());

    // �±׸� ��ǥ�� �����Ͽ� �� �ٷ� ��ħ
    CSVRow += FString::Join(SelectedTags, TEXT(","));
    CSVRow += TEXT("\n"); // �ٹٲ� �߰�

    // 4. ���� ��� ���� �� ����
    FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("Surveys/");
    FString FullPath = SaveDirectory + InFileName;

    // ������ �̹� �����ϴ��� Ȯ��
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    bool bFileExists = PlatformFile.FileExists(*FullPath);

    // ������ ������ ����
    if (!PlatformFile.DirectoryExists(*SaveDirectory))
    {
        PlatformFile.CreateDirectory(*SaveDirectory);
    }

    // ������ ������ ����� �߰��ϰ�, ������ ���븸 �߰� (Append)
    FString ContentToSave = bFileExists ? CSVRow : CSVHeader + CSVRow;

    FFileHelper::SaveStringToFile(ContentToSave, *FullPath, FFileHelper::EEncodingOptions::ForceUTF8, &IFileManager::Get(), FILEWRITE_Append);

    UE_LOG(LogTemp, Log, TEXT("Survey saved to: %s"), *FullPath);
}
