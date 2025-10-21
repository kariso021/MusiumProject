#include "GlobalSoundManager.h"
#include "MediaSoundComponent.h"
#include "SoundManagerSubsystem.h"

AGlobalSoundManager::AGlobalSoundManager()
{
    PrimaryActorTick.bCanEverTick = false;


    MediaSoundComponent = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaSoundComponent"));
    SetRootComponent(MediaSoundComponent);
}

void AGlobalSoundManager::BeginPlay()
{
    Super::BeginPlay();

    UGameInstance* GameInstance = GetGameInstance();
    if (GameInstance)
    {
        USoundManagerSubsystem* SoundSubsystem = GameInstance->GetSubsystem<USoundManagerSubsystem>();
        if (SoundSubsystem)
        {
            SoundSubsystem->RegisterGlobalSoundManager(this);
            UE_LOG(LogTemp, Display, TEXT("SoundRegisterFin"));
        }
    }
}