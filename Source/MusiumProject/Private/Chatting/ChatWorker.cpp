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
    // 소켓과 서브시스템 정리
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
    // 소켓 서브시스템 가져오기
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Chat Worker: Failed to get SocketSubsystem."));
        return 1;
    }

    // 소켓 생성 (TCP)
    Socket = SocketSubsystem->CreateSocket(NAME_Stream, TEXT("ChatSocket"), false);
    if (!Socket)
    {
        UE_LOG(LogTemp, Error, TEXT("Chat Worker: Failed to create socket."));
        return 1;
    }

    // IP 주소 파싱
    FIPv4Address IP;
    FIPv4Address::Parse(IPAddress, IP);
    TSharedRef<FInternetAddr> Addr = SocketSubsystem->CreateInternetAddr();
    Addr->SetIp(IP.Value);
    Addr->SetPort(Port);

    // 서버에 연결 시도
    if (!Socket->Connect(*Addr))
    {
        UE_LOG(LogTemp, Error, TEXT("Chat Worker: Failed to connect to server %s:%d"), *IPAddress, Port);
        return 1;
    }

    UE_LOG(LogTemp, Warning, TEXT("Chat Worker: Connected to server."));

    // bIsRunning 플래그가 true인 동안 계속 메시지 수신 대기
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
                // 받은 데이터를 FString으로 변환
                const FString ReceivedMessage = FString(UTF8_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
                // 스레드 안전 큐에 메시지를 넣음
                if (MessageQueue)
                {
                    MessageQueue->Enqueue(ReceivedMessage);
                }
            }
        }
        // CPU 자원을 너무 많이 사용하지 않도록 잠시 대기
        FPlatformProcess::Sleep(0.1f);
    }

    return 0;
}

void FChatWorker::Stop()
{
    // Run() 루프를 중단시키기 위해 플래그를 false로 설정
    bIsRunning = false;
}

void FChatWorker::Exit()
{
    UE_LOG(LogTemp, Warning, TEXT("Chat Worker: Exiting."));
    // 필요하다면 추가적인 정리 작업 수행
}

bool FChatWorker::SendMessage(const FString& Message)
{
    if (Socket && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
    {
        int32 BytesSent = 0;
        // FString을 UTF-8 바이트 배열로 변환하여 전송
        return Socket->Send(reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*Message)), Message.Len(), BytesSent);
    }
    return false;
}