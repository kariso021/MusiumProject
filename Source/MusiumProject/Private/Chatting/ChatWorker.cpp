#include "ChatWorker.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

FChatWorker::FChatWorker(const FString& InIP, int32 InPort, TQueue<FString, EQueueMode::Spsc>* InMessageQueue)
    : IPAddress(InIP), Port(InPort), MessageQueue(InMessageQueue)
{
}

FChatWorker::~FChatWorker()
{
    // ���ϰ� ����ý��� ����
    if (Socket)
    {
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        Socket = nullptr;
    }
}

bool FChatWorker::Init()
{
    bIsRunning = true;
    UE_LOG(LogTemp, Warning, TEXT("Chat Worker: Initialized."));
    return true;
}

uint32 FChatWorker::Run()
{
    // ���� ����ý��� ��������
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Chat Worker: Failed to get SocketSubsystem."));
        return 1;
    }

    // ���� ���� (TCP)
    Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("ChatSocket"), false);
    if (!Socket)
    {
        UE_LOG(LogTemp, Error, TEXT("Chat Worker: Failed to create socket."));
        return 1;
    }

    // IP �ּ� �Ľ�
    FIPv4Address IP;
    FIPv4Address::Parse(IPAddress, IP);
    TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();
    Addr->SetIp(IP.Value);
    Addr->SetPort(Port);

    // ������ ���� �õ�
    if (!Socket->Connect(*Addr))
    {
        UE_LOG(LogTemp, Error, TEXT("Chat Worker: Failed to connect to server %s:%d"), *IPAddress, Port);
        return 1;
    }

    UE_LOG(LogTemp, Warning, TEXT("Chat Worker: Connected to server."));

    // bIsRunning �÷��װ� true�� ���� ��� �޽��� ���� ���
    while (bIsRunning)
    {
        uint32 PendingDataSize = 0;
        if (Socket->HasPendingData(PendingDataSize) && PendingDataSize > 0)
        {
            TArray<uint8> ReceivedData;
            ReceivedData.SetNumUninitialized(PendingDataSize);

            int32 BytesRead = 0;
            if (Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), BytesRead))
            {
                // ���� �����͸� FString���� ��ȯ
                const FString ReceivedMessage = FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
                // ������ ���� ť�� �޽����� ����
                if (MessageQueue)
                {
                    MessageQueue->Enqueue(ReceivedMessage);
                }
            }
        }
        // CPU �ڿ��� �ʹ� ���� ������� �ʵ��� ��� ���
        FPlatformProcess::Sleep(0.1f);
    }

    return 0;
}

void FChatWorker::Stop()
{
    // Run() ������ �ߴܽ�Ű�� ���� �÷��׸� false�� ����
    bIsRunning = false;
}

void FChatWorker::Exit()
{
    UE_LOG(LogTemp, Warning, TEXT("Chat Worker: Exiting."));
    // �ʿ��ϴٸ� �߰����� ���� �۾� ����
}

bool FChatWorker::SendMessage(const FString& Message)
{
    if (Socket && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
    {
        int32 BytesSent = 0;
        // FString�� UTF-8 ����Ʈ �迭�� ��ȯ�Ͽ� ����
        return Socket->Send(reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*Message)), Message.Len(), BytesSent);
    }
    return false;
}