// Fill out your copyright notice in the Description page of Project Settings.


#include "MusiumProjectState.h"
#include "Chatting/ChattingComponent.h"

void AMusiumProjectState::Multicast_BroadcastChatMessage_Implementation(const FString& PlayerName, const FString& Message)
{
    // 이 코드는 서버와 모든 클라이언트에서 실행됩니다.
    // 월드에 있는 모든 PlayerController를 순회합니다.
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)
        {
            // 각 PlayerController에 붙어있는 ChattingComponent를 찾습니다.
            UChattingComponent* ChatComp = PC->FindComponentByClass<UChattingComponent>();
            if (ChatComp)
            {
                // 컴포넌트의 함수를 호출하여 메시지를 전달합니다.
                ChatComp->ReceiveChatMessage(PlayerName, Message);
            }
        }
    }
}