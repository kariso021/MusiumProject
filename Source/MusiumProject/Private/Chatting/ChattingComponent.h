#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChattingComponent.generated.h"

// 메시지 수신 델리게이트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageReceivedDelegate, const FString&, PlayerName, const FString&, Message);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MUSIUMPROJECT_API UChattingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UChattingComponent();

    // 블루프린트 UI가 바인딩할 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Chatting")
    FOnMessageReceivedDelegate OnMessageReceived;

    // 블루프린트 UI에서 호출할 함수
    UFUNCTION(BlueprintCallable, Category = "Chatting")
    void SendChatMessage(const FString& Message);

    // GameState가 모든 클라이언트에서 호출해 줄 함수
    void ReceiveChatMessage(const FString& PlayerName, const FString& Message);

protected:
    // 서버에 실제 메시지 전송을 요청하는 RPC
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SendChatMessage(const FString& Message);
};