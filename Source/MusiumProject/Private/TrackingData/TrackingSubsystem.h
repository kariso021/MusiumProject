// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TrackingData/TrackingDataDefs.h" // FSurveyData, ETrackingEventType 가 정의된 파일
#include "TrackingData/UserTrackingData.h" // FUserTrackingLog 가 정의된 파일
#include "TrackingSubsystem.generated.h"



UCLASS()
class UTrackingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	// 세션 시작 후 사용할 함수들
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void StartNewSession(const FSurveyData& InSurveyData);

	// --- 유물 조회 관련 로그 ---
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogZoneEnter(const FString& ArtifactID); // 사용함

	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogZoneExit(const FString& ArtifactID); // 사용함

	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogPageView(const FString& ArtifactID); // 사용함

	UFUNCTION(BlueprintCallable, Category = "Tracking")//사용함
	void LogPageLeave(); // 사용함

	// --- 상호작용 관련 로그 ---
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogClick(const FString& ClickedElementID);//사용함

	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogScroll(const FString& ArtifactID, float ScrollPercentage);

	// --- 비디오 관련 로그 ---
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogVideoPlay(float PlaybackSeconds);

	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogVideoPause(const FString& ArtifactID, float PlaybackSeconds);

	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogVideoEnd(FString& ArtifactID);

	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void LogHoverStart(const FString& ArtifactID); //사용함


	// Save 파일 저장
	UFUNCTION(BlueprintCallable, Category = "Tracking")
	void SaveTrackingDataToSlot();



private:
	/** 실제 이벤트 로그를 생성하고 배열에 추가하는 내부 헬퍼 함수 */
	void LogEvent(FUserTrackingLog& Log);

	/** 현재 세션의 고유 ID */
	FString CurrentSessionID;

	/** 현재 세션의 사용자 설문 데이터 */
	FSurveyData CurrentUserSurveyData;

	/** 현재 세션에서 기록된 모든 추적 데이터 */
	TArray<FUserTrackingLog> CurrentTrackingLogs;

	/** 저장할 데이터가 있는지 확인하는 플래그 */
	bool bHasDataToSave = false;


};
