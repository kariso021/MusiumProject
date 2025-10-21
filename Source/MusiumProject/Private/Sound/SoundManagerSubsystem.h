#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundManagerSubsystem.generated.h"

class AGlobalSoundManager;

UCLASS()
class MUSIUMPROJECT_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    void RegisterGlobalSoundManager(AGlobalSoundManager* Manager);
    AGlobalSoundManager* GetGlobalSoundManager() const;

private:
    UPROPERTY()
    TObjectPtr<AGlobalSoundManager> RegisteredSoundManager;
};