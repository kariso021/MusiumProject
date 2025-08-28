#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UserTrackingData.h" 
#include "CurioConnectSaveGame.generated.h"

UCLASS()
class MUSIUMPROJECT_API UCurioConnectSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    // 이 저장 파일이 어떤 세션의 기록인지 식별하기 위한 ID
    UPROPERTY(VisibleAnywhere, Category = "Tracking Data")
    FString SessionID;

    UPROPERTY(VisibleAnywhere, Category = "Tracking Data")
    TArray<FUserTrackingLog> TrackingLogs;
};