#include "SoundManagerSubsystem.h"
#include "GlobalSoundManager.h"

void USoundManagerSubsystem::RegisterGlobalSoundManager(AGlobalSoundManager* Manager)
{
    if (Manager)
    {
        RegisteredSoundManager = Manager;
        UE_LOG(LogTemp, Display, TEXT("Registered!!"));
    }
}

AGlobalSoundManager* USoundManagerSubsystem::GetGlobalSoundManager() const
{
    return RegisteredSoundManager;
}