// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionUIComponent.h"
#include "InteractiveActor/InteractiveActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Widget/ArtifactInfoWidget.h"
#include "Data/ArtifactData.h"
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

// 아티팩트 아이디 1,2,3,4,5 이렇게~
void UInteractionUIComponent::OnActorInteracted(int32 ArtifactID)
{
	//uelog 로 ArtifactID 출력

    UE_LOG(LogTemp, Display, TEXT("Interacted!! Artifact Id is %d"), ArtifactID);
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !ArtifactTable || !ArtifactWidgetClass) return;

    // 1) 위젯 생성(최초 1회)
    if (!ArtifactWidget)
    {
        ArtifactWidget = CreateWidget<UArtifactInfoWidget>(PC, ArtifactWidgetClass);
        if (!ArtifactWidget) return;
        ArtifactWidget->AddToViewport(100);
    }

    // 2) DataTable 직조회 -> SetData
    const FName RowName(*LexToString(ArtifactID));
    const FArtifactData* Row = ArtifactTable->FindRow<FArtifactData>(RowName, TEXT("Artifact Lookup"));
    if (!Row) return;

    ArtifactWidget->SetData(*Row);


    // 3) 입력/커서 전환
    PC->bShowMouseCursor = true;
    FInputModeUIOnly Mode;
    Mode.SetWidgetToFocus(ArtifactWidget->TakeWidget());
    Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(Mode);
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

