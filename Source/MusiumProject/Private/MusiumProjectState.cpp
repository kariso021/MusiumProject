// Fill out your copyright notice in the Description page of Project Settings.


#include "MusiumProjectState.h"
#include "Chatting/ChattingComponent.h"

void AMusiumProjectState::Multicast_BroadcastChatMessage_Implementation(const FString& PlayerName, const FString& Message)
{
    // �� �ڵ�� ������ ��� Ŭ���̾�Ʈ���� ����
    // ���忡 �ִ� ��� PlayerController�� ��ȸ
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)
        {

            UChattingComponent* ChatComp = PC->FindComponentByClass<UChattingComponent>();
            if (ChatComp)
            {
                ChatComp->ReceiveChatMessage(PlayerName, Message);
            }
        }
    }
}