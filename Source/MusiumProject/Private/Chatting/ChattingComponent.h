#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChattingComponent.generated.h"

// �޽��� ���� ��������Ʈ ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageReceivedDelegate, const FString&, PlayerName, const FString&, Message);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MUSIUMPROJECT_API UChattingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UChattingComponent();

    // �������Ʈ UI�� ���ε��� ��������Ʈ
    UPROPERTY(BlueprintAssignable, Category = "Chatting")
    FOnMessageReceivedDelegate OnMessageReceived;

    // �������Ʈ UI���� ȣ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Chatting")
    void SendChatMessage(const FString& Message);

    // GameState�� ��� Ŭ���̾�Ʈ���� ȣ���� �� �Լ�
    void ReceiveChatMessage(const FString& PlayerName, const FString& Message);

protected:
    // ������ ���� �޽��� ������ ��û�ϴ� RPC
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SendChatMessage(const FString& Message);
};