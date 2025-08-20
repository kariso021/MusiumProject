// Fill out your copyright notice in the Description page of Project Settings.


#include "MusiumProjectState.h"
#include "Chatting/ChattingComponent.h"

void AMusiumProjectState::Multicast_BroadcastChatMessage_Implementation(const FString& PlayerName, const FString& Message)
{
    // 이 코드는 서버와 모든 클라이언트에서 실행
    // 월드에 있는 모든 PlayerController를 순회
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