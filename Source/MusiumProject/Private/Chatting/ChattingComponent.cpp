#include "ChattingComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MusiumProjectState.h"

UChattingComponent::UChattingComponent()
{
    SetIsReplicated(true);
}

void UChattingComponent::SendChatMessage(const FString& Message)
{
    // 클라이언트에서 서버 RPC를 호출
    Server_SendChatMessage(Message);
}

bool UChattingComponent::Server_SendChatMessage_Validate(const FString& Message)
{
    // 메시지가 비어있지 않고 너무 길지 않은지 검증
    return !Message.IsEmpty() && Message.Len() < 200;
}

void UChattingComponent::Server_SendChatMessage_Implementation(const FString& Message)
{
    // 이 코드는 서버에서만 실행됩니다.
    // 이 컴포넌트를 소유한 PlayerController를 가져옵니다.
    APlayerController* OwningPC = Cast<APlayerController>(GetOwner());
    if (!OwningPC) return;

    // PlayerController의 PlayerState에서 플레이어 이름을 가져옵니다.
    APlayerState* PlayerState = OwningPC->PlayerState;
    if (!PlayerState) return;

    const FString PlayerName = PlayerState->GetPlayerName();

    // GameState를 통해 모든 클라이언트에게 메시지를 브로드캐스트합니다.
    AMusiumProjectState* MyGameState = GetWorld()->GetGameState<AMusiumProjectState>();
    if (MyGameState)
    {
        MyGameState->Multicast_BroadcastChatMessage(PlayerName, Message);
    }
}

void UChattingComponent::ReceiveChatMessage(const FString& PlayerName, const FString& Message)
{
    // 모든 클라이언트에서 이 함수가 호출됩니다.
    // UI에 메시지를 전달하기 위해 델리게이트를 호출(Broadcast)합니다.
    OnMessageReceived.Broadcast(PlayerName, Message);
}