// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackingData/TrackingDataUtility.h"
#include "CurioConnectSaveGame.h"

#include "HAL/FileManager.h"
#include "HAL/PlatformFileManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"



TArray<FString> UTrackingDataUtility::GetAllSessionIDs()
{
	// 기존 TrackingSubsystem에 있던 코드와 동일
	TArray<FString> FoundSessionIDs;
	const FString SaveGameDirectory = FPaths::ProjectSavedDir() + TEXT("SaveGames/");
	IFileManager::Get().FindFiles(FoundSessionIDs, *SaveGameDirectory, TEXT("*.sav"));

	for (FString& FileName : FoundSessionIDs)
	{
		FileName.RemoveFromEnd(TEXT(".sav"));
	}
	return FoundSessionIDs;
}

bool UTrackingDataUtility::ExportSessionToCsv(const FString& SessionID)
{
	// 1. 저장된 .sav 파일 로드
	UCurioConnectSaveGame* LoadedGame = Cast<UCurioConnectSaveGame>(UGameplayStatics::LoadGameFromSlot(SessionID, 0));
	if (!LoadedGame)
	{
		UE_LOG(LogTemp, Error, TEXT("ExportSessionToCsv: Failed to load session data from slot: %s"), *SessionID);
		return false;
	}

	// 2. CSV 파일 경로 및 이름 설정
	const FString SaveDirectory = FPaths::ProjectSavedDir() + TEXT("TrackingDataExport/");
	const FString FileName = FString::Printf(TEXT("ExportedData_%s.csv"), *SessionID);
	const FString FilePath = SaveDirectory + FileName;

	TArray<FString> CsvLines;
	// 3. CSV 헤더 추가 (데이터베이스 테이블의 컬럼명과 일치)
	CsvLines.Add(TEXT("SessionID,Timestamp,EventType,ArtifactID,ClickedElementID,ScrollDepthPercentage,VideoPlaybackSeconds,Age,Gender,VisitFrequency,Education,DominantPropensity"));

	// 4. Enum을 문자열로 변환하기 위한 UEnum 객체 찾기
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETrackingEventType"), true);
	if (!EnumPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("ExportSessionToCsv: Failed to find ETrackingEventType enum."));
		return false;
	}

	// 5. 로드된 모든 로그를 한 줄씩 CSV 형식으로 변환하여 배열에 추가
	for (const FUserTrackingLog& Log : LoadedGame->TrackingLogs)
	{
		// Enum 값을 DisplayName 문자열로 변환 (예: ETrackingEventType::ZoneEnter -> "Zone Enter")
		FString EventTypeStr = EnumPtr->GetDisplayNameTextByValue((int64)Log.EventType).ToString();

		FString Line = FString::Printf(TEXT("%s,%s,\"%s\",%s,%s,%.4f,%.2f,%s,%s,%s,%s,%s"),
			*Log.SessionID,
			*Log.Timestamp.ToIso8601(),
			*EventTypeStr, // 공백이 있을 수 있으므로 따옴표로 감싸줌
			*Log.ArtifactID,
			*Log.ClickedElementID,
			Log.ScrollDepthPercentage,
			Log.VideoPlaybackSeconds,
			*Log.Age,
			*Log.Gender,
			*Log.VisitFrequency,
			*Log.Education,
			*Log.DominantPropensity
		);
		CsvLines.Add(Line);
	}

	// 6. 파일로 저장
	// 저장할 폴더가 없다면 생성
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*SaveDirectory))
	{
		PlatformFile.CreateDirectoryTree(*SaveDirectory);
	}

	// 문자열 배열을 파일에 한 번에 쓰기
	if (FFileHelper::SaveStringArrayToFile(CsvLines, *FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Session data from slot '%s' exported to CSV successfully: %s"), *SessionID, *FilePath);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ExportSessionToCsv: Failed to save CSV file for slot: %s"), *SessionID);
		return false;
	}
}


