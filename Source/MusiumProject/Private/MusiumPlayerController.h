// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/InteractionUIComponent.h"
#include "MusiumPlayerController.generated.h"

/**
 * 
 */

class SPauseMenuWidget;

UCLASS()
class AMusiumPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMusiumPlayerController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UInteractionUIComponent* InteractionUIComp;

public:
    // 게임 재개 함수
    void ResumeGame();

protected:
    virtual void SetupInputComponent() override;

    // 일시정지 메뉴를 띄우는 함수
    void OnPauseGame();

private:
    // 생성된 Slate 위젯을 가리킬 포인터
    TSharedPtr<SPauseMenuWidget> PauseMenuWidget;


	
};
