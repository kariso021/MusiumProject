// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Containers/Queue.h"

class FSocket;

// FRunnable�� ��ӹ޾� ��Ŀ �����带 ����
class FChatWorker : public FRunnable
{
public:
    // ������: ���ϰ� �޽��� ť�� �ܺο��� �޾� �ʱ�ȭ
    FChatWorker(const FString& InIP, int32 InPort, TQueue<FString, EQueueMode::Spsc>* InMessageQueue);
    ~FChatWorker();

    // FRunnable �������̽� ����
    virtual bool Init() override;
    virtual uint32 Run() override;
    virtual void Stop() override;
    virtual void Exit() override;

    // �޽��� ���� �Լ�
    bool SendMessage(const FString& Message);

private:
    // ������ ��� ���� �÷���
    FThreadSafeBool bIsRunning = false;

    // ��Ʈ��ũ ����
    FString IPAddress;
    int32 Port;

    // ���� ��ſ� ���� ����
    FSocket* Socket = nullptr;

    // ���� ������� ����ϱ� ���� �޽��� ť (�����ͷ� ����)
    TQueue<FString, EQueueMode::Spsc>* MessageQueue = nullptr;
};