#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalSoundManager.generated.h"

class UMediaSoundComponent; 

UCLASS()
class MUSIUMPROJECT_API AGlobalSoundManager : public AActor
{
    GENERATED_BODY()

public:
    AGlobalSoundManager();

    // 위젯에서 MediaSoundComponent에 접근
    UMediaSoundComponent* GetMediaSoundComponent() const { return MediaSoundComponent; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UMediaSoundComponent> MediaSoundComponent;
};