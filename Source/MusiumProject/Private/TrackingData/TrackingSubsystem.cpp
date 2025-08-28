// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "CurioConnectSaveGame.h" // SaveGame 클래스 포함
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UTrackingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTrackingSubsystem::Deinitialize()
{
	// 게임이 종료
	SaveTrackingDataToSlot();
	Super::Deinitialize();
}

void UTrackingSubsystem::StartNewSession(const FSurveyData& InSurveyData)
{
	//cleanup 과정
	CurrentTrackingLogs.Empty();
	CurrentSessionID = "";
	bHasDataToSave = false;

	CurrentUserSurveyData = InSurveyData;
	CurrentSessionID = FGuid::NewGuid().ToString(); // 고유 세션 ID 생성

	UE_LOG(LogTemp, Warning, TEXT("New Tracking Session Started. Session ID: %s"), *CurrentSessionID);
}

// --- 로그 함수 구현 ---

void UTrackingSubsystem::LogZoneEnter(const FString& ArtifactID)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::ZoneEnter;
	NewLog.ArtifactID = ArtifactID;
	LogEvent(NewLog);
	UE_LOG(LogTemp, Display, TEXT("LogZoneEnter!"));
}

void UTrackingSubsystem::LogZoneExit(const FString& ArtifactID)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::ZoneExit;
	NewLog.ArtifactID = ArtifactID;
	LogEvent(NewLog);
	UE_LOG(LogTemp, Display, TEXT("LogZoneExit!"));
}

void UTrackingSubsystem::LogPageView(const FString& ArtifactID)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::PageView;
	NewLog.ArtifactID = ArtifactID;
	LogEvent(NewLog);
	UE_LOG(LogTemp, Display, TEXT("LogPageView!"));
}

void UTrackingSubsystem::LogPageLeave()
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::PageLeave;
	LogEvent(NewLog);
	UE_LOG(LogTemp, Display, TEXT("Logleave!"));
}

void UTrackingSubsystem::LogClick(const FString& ClickedElementID)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::Click;
	NewLog.ClickedElementID = ClickedElementID;
	LogEvent(NewLog);
	UE_LOG(LogTemp, Display, TEXT("LogClick!"));
}

void UTrackingSubsystem::LogScroll(const FString& ArtifactID, float ScrollPercentage)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::Scroll;
	NewLog.ArtifactID = ArtifactID;
	NewLog.ScrollDepthPercentage = ScrollPercentage;
	LogEvent(NewLog);
}

void UTrackingSubsystem::LogVideoPlay(float PlaybackSeconds)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::VideoPlay;
	NewLog.VideoPlaybackSeconds = PlaybackSeconds;
	LogEvent(NewLog);
}

void UTrackingSubsystem::LogVideoPause(const FString& ArtifactID, float PlaybackSeconds)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::VideoPause;
	NewLog.ArtifactID = ArtifactID;
	NewLog.VideoPlaybackSeconds = PlaybackSeconds;
	LogEvent(NewLog);
}

void UTrackingSubsystem::LogVideoEnd(FString& ArtifactID)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::VideoEnd;
	NewLog.ArtifactID = ArtifactID;
	LogEvent(NewLog);
}

void UTrackingSubsystem::LogHoverStart(const FString& ArtifactID)
{
	FUserTrackingLog NewLog;
	NewLog.EventType = ETrackingEventType::Hover;
	NewLog.ArtifactID = ArtifactID;
	LogEvent(NewLog);
	UE_LOG(LogTemp, Display, TEXT("LogHover!"));
}



// --- 핵심 로직: 이벤트 생성 및 저장 ---



void UTrackingSubsystem::LogEvent(FUserTrackingLog& Log)
{
	if (CurrentSessionID.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("TrackingSubsystem::LogEvent - SessionID is not set. Call StartNewSession first."));
		return;
	}

	FUserTrackingLog FinalLog = Log;

	// 공통 정보 채우기
	FinalLog.SessionID = CurrentSessionID;
	FinalLog.Timestamp = FDateTime::UtcNow();
	FinalLog.Age = CurrentUserSurveyData.Age;
	FinalLog.Gender = CurrentUserSurveyData.Gender;
	FinalLog.VisitFrequency = CurrentUserSurveyData.VisitFrequency;
	FinalLog.Education = CurrentUserSurveyData.Education;
	FinalLog.DominantPropensity = CurrentUserSurveyData.DominantPropensity;

	CurrentTrackingLogs.Add(FinalLog);
	bHasDataToSave = true;
}



void UTrackingSubsystem::SaveTrackingDataToSlot()
{
	if (!bHasDataToSave || CurrentSessionID.IsEmpty() || CurrentTrackingLogs.Num() == 0)
	{
		return;
	}

	// SaveGame 객체 생성
	UCurioConnectSaveGame* SaveGameObject = Cast<UCurioConnectSaveGame>(UGameplayStatics::CreateSaveGameObject(UCurioConnectSaveGame::StaticClass()));
	if (!SaveGameObject)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SaveGameObject."));
		return;
	}

	// 데이터 복사
	SaveGameObject->SessionID = CurrentSessionID;
	SaveGameObject->TrackingLogs = CurrentTrackingLogs;

	// 파일에 저장. 슬롯 이름은 세션 ID를 사용하여 고유하게 만듭니다.
	if (UGameplayStatics::SaveGameToSlot(SaveGameObject, CurrentSessionID, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Tracking data for session %s saved successfully."), *CurrentSessionID);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save tracking data for session %s."), *CurrentSessionID);
	}

	// 저장 후 현재 로그 데이터 초기화
	CurrentTrackingLogs.Empty();
	bHasDataToSave = false;
}

