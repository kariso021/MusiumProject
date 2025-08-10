// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Containers/Queue.h"

class FSocket;

// FRunnable을 상속받아 워커 스레드를 정의
class FChatWorker : public FRunnable
{
public:
    // 생성자: 소켓과 메시지 큐를 외부에서 받아 초기화
    FChatWorker(const FString& InIP, int32 InPort, TQueue<FString, EQueueMode::Spsc>* InMessageQueue);
    ~FChatWorker();

    // FRunnable 인터페이스 구현
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    // 메시지 전송 함수
    bool SendMessage(const FString& Message);

private:
    // 스레드 제어를 위한 플래그
    FThreadSafeBool bIsRunning = false;

    // 네트워크 정보
    FString IPAddress;
    int32 Port;

    // 실제 통신에 사용될 소켓
    FSocket* Socket = nullptr;

    // 게임 스레드와 통신하기 위한 메시지 큐 (포인터로 받음)
    TQueue<FString, EQueueMode::Spsc>* MessageQueue = nullptr;
};