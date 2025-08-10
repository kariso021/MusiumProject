// Fill out your copyright notice in the Description page of Project Settings.


#include "MusiumProjectState.h"
#include "Chatting/ChattingComponent.h"

void AMusiumProjectState::Multicast_BroadcastChatMessage_Implementation(const FString& PlayerName, const FString& Message)
{
    // �� �ڵ�� ������ ��� Ŭ���̾�Ʈ���� ����˴ϴ�.
    // ���忡 �ִ� ��� PlayerController�� ��ȸ�մϴ�.
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)
        {
            // �� PlayerController�� �پ��ִ� ChattingComponent�� ã���ϴ�.
            UChattingComponent* ChatComp = PC->FindComponentByClass<UChattingComponent>();
            if (ChatComp)
            {
                // ������Ʈ�� �Լ��� ȣ���Ͽ� �޽����� �����մϴ�.
                ChatComp->ReceiveChatMessage(PlayerName, Message);
            }
        }
    }
}