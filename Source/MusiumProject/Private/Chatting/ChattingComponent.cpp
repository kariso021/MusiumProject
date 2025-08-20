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
    // Ŭ���̾�Ʈ���� ���� RPC�� ȣ��
    Server_SendChatMessage(Message);
}

bool UChattingComponent::Server_SendChatMessage_Validate(const FString& Message)
{
    // �޽����� ������� �ʰ� �ʹ� ���� ������ ����
    return !Message.IsEmpty() && Message.Len() < 200;
}

void UChattingComponent::Server_SendChatMessage_Implementation(const FString& Message)
{
    // �� �ڵ�� ���������� ����˴ϴ�.
    // �� ������Ʈ�� ������ PlayerController�� �����ɴϴ�.
    APlayerController* OwningPC = Cast<APlayerController>(GetOwner());
    if (!OwningPC) return;

    // PlayerController�� PlayerState���� �÷��̾� �̸��� �����ɴϴ�.
    APlayerState* PlayerState = OwningPC->PlayerState;
    if (!PlayerState) return;

    const FString PlayerName = PlayerState->GetPlayerName();

    // GameState�� ���� ��� Ŭ���̾�Ʈ���� �޽����� ��ε�ĳ��Ʈ�մϴ�.
    AMusiumProjectState* MyGameState = GetWorld()->GetGameState<AMusiumProjectState>();
    if (MyGameState)
    {
        MyGameState->Multicast_BroadcastChatMessage(PlayerName, Message);
    }
}

void UChattingComponent::ReceiveChatMessage(const FString& PlayerName, const FString& Message)
{
    // ��� Ŭ���̾�Ʈ���� �� �Լ��� ȣ��˴ϴ�.
    // UI�� �޽����� �����ϱ� ���� ��������Ʈ�� ȣ��(Broadcast)�մϴ�.
    OnMessageReceived.Broadcast(PlayerName, Message);
}