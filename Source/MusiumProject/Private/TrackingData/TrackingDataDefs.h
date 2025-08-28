#pragma once

#include "CoreMinimal.h"
#include "TrackingDataDefs.generated.h"

// 기록할 이벤트 열거한 것
UENUM(BlueprintType)
enum class ETrackingEventType : uint8
{
    None            UMETA(DisplayName = "None"),

    // --- 유물 조회 관련 ---
    ZoneEnter       UMETA(DisplayName = "Zone Enter"),       // 유물 상호작용 존 진입 (동선 분석의 시작점)
    ZoneExit        UMETA(DisplayName = "Zone Exit"),        // 유물 상호작용 존 이탈 (동선 분석의 끝점)
    Hover           UMETA(DisplayName = "Hover"),            // 유물 호버
	PageView       UMETA(DisplayName = "Page View"),        // 유물 페이지 조회
	PageLeave      UMETA(DisplayName = "Page Leave"),       // 유물 페이지 이탈

    // --- 상호작용 관련 ---
    Click           UMETA(DisplayName = "Click"),            // UI 요소 클릭
    Scroll          UMETA(DisplayName = "Scroll"),           // 콘텐츠 스크롤

    // --- 비디오 관련 ---
    VideoPlay       UMETA(DisplayName = "Video Play"),
    VideoPause      UMETA(DisplayName = "Video Pause"),
    VideoEnd        UMETA(DisplayName = "Video End")
};

// 설문조사 데이터
USTRUCT(BlueprintType)
struct FSurveyData
{
    GENERATED_BODY()

	UPROPERTY() FString SessionID; // 사용자 ID
    UPROPERTY() FString Age;
    UPROPERTY() FString Gender;
    UPROPERTY() FString VisitFrequency;
    UPROPERTY() FString Education;
    UPROPERTY() FString DominantPropensity;
};
