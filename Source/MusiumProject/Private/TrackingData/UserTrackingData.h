#pragma once

#include "CoreMinimal.h"
#include "TrackingDataDefs.h"
#include "UserTrackingData.generated.h"

// 모든 사용자 행동 로그를 담을 통합 구조체 DB 테이블의 각 행에 해당
USTRUCT(BlueprintType)
struct FUserTrackingLog
{
    GENERATED_BODY()

    // --- [사용자 및 세션 정보] ---
    UPROPERTY() FString SessionID;
    UPROPERTY() FString Age;
    UPROPERTY() FString Gender;
    UPROPERTY() FString VisitFrequency;
    UPROPERTY() FString Education;
    UPROPERTY() FString DominantPropensity;

    // --- [이벤트 공통 정보] ---
    UPROPERTY() FDateTime Timestamp;
    UPROPERTY() ETrackingEventType EventType;
    UPROPERTY() FString ArtifactID;

    // --- [이벤트별 상세 정보] ---
    UPROPERTY() FString ClickedElementID;
    UPROPERTY() float ScrollDepthPercentage;
    UPROPERTY() float VideoPlaybackSeconds;

	// 생성자: 기본값 초기화
    FUserTrackingLog()
    {
        Timestamp = FDateTime::Now();
        EventType = ETrackingEventType::None;
        ScrollDepthPercentage = 0.f;
        VideoPlaybackSeconds = 0.f;
    }
};