// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionUIComponent.h"
#include "InteractiveActor/InteractiveActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include <EngineUtils.h>

// Sets default values for this component's properties
UInteractionUIComponent::UInteractionUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionUIComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

    UWorld* World = GetWorld();
    if (!World) return;

    // C++11 범위 기반 for 루프
    for (AInteractiveActor* ArtifactActor : TActorRange<AInteractiveActor>(World))
    {
        ArtifactActor->OnInteract.AddDynamic(
            this,
            &UInteractionUIComponent::OnActorInteracted
        );
    }
	
}


// Called every frame
void UInteractionUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionUIComponent::OnActorInteracted(int32 ArtifactID)
{
    // 0) 플레이어 컨트롤러 참조
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return;

    // 1) DataTable에서 Row 찾기
    //static const FString Context = TEXT("Lookup");
    //FArtifactData* Row = ArtifactTable->FindRow<FArtifactData>(
    //    *FString::FromInt(ArtifactID), Context);
    //if (!Row) return;
    

    // 2) Widget 생성(최초에만)
    if (!ArtifactWidget)
    {
        ArtifactWidget = CreateWidget<UUserWidget>(
            PC, ArtifactWidgetClass);
        ArtifactWidget->AddToViewport();
    }

    // 3) SetData 호출
 /*   if (UFunction* Fn = ArtifactWidget->FindFunction(TEXT("SetData")))
    {
        struct FPayload { FArtifactData Data; };
        FPayload Payload{ *Row };
        ArtifactWidget->ProcessEvent(Fn, &Payload);
    }*/

    // ────────────── 여기서부터 입력/커서 제어 ──────────────

    // 4) 마우스 커서 보이기
    PC->bShowMouseCursor = true;

    // 5) UI 전용 입력 모드로 전환
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(ArtifactWidget->TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);

    // 6) 캐릭터 이동·회전 입력 무시 (WASD, 마우스 회전)
    PC->SetIgnoreMoveInput(true);
    PC->SetIgnoreLookInput(true);
}

void UInteractionUIComponent::HideArtifactUI()
{
    if (!ArtifactWidget) return;

    // 1) 위젯 제거
    ArtifactWidget->RemoveFromParent();
    ArtifactWidget = nullptr;

    // 2) 컨트롤러 복원
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return;

    PC->bShowMouseCursor = false;
    PC->SetIgnoreMoveInput(false);
    PC->SetIgnoreLookInput(false);

    FInputModeGameOnly GameMode;
    PC->SetInputMode(GameMode);
}

