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

    // C++11 ���� ��� for ����
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
    // 0) �÷��̾� ��Ʈ�ѷ� ����
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return;

    // 1) DataTable���� Row ã��
    //static const FString Context = TEXT("Lookup");
    //FArtifactData* Row = ArtifactTable->FindRow<FArtifactData>(
    //    *FString::FromInt(ArtifactID), Context);
    //if (!Row) return;
    

    // 2) Widget ����(���ʿ���)
    if (!ArtifactWidget)
    {
        ArtifactWidget = CreateWidget<UUserWidget>(
            PC, ArtifactWidgetClass);
        ArtifactWidget->AddToViewport();
    }

    // 3) SetData ȣ��
 /*   if (UFunction* Fn = ArtifactWidget->FindFunction(TEXT("SetData")))
    {
        struct FPayload { FArtifactData Data; };
        FPayload Payload{ *Row };
        ArtifactWidget->ProcessEvent(Fn, &Payload);
    }*/

    // ���������������������������� ���⼭���� �Է�/Ŀ�� ���� ����������������������������

    // 4) ���콺 Ŀ�� ���̱�
    PC->bShowMouseCursor = true;

    // 5) UI ���� �Է� ���� ��ȯ
    FInputModeUIOnly InputMode;
    InputMode.SetWidgetToFocus(ArtifactWidget->TakeWidget());
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);

    // 6) ĳ���� �̵���ȸ�� �Է� ���� (WASD, ���콺 ȸ��)
    PC->SetIgnoreMoveInput(true);
    PC->SetIgnoreLookInput(true);
}

void UInteractionUIComponent::HideArtifactUI()
{
    if (!ArtifactWidget) return;

    // 1) ���� ����
    ArtifactWidget->RemoveFromParent();
    ArtifactWidget = nullptr;

    // 2) ��Ʈ�ѷ� ����
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return;

    PC->bShowMouseCursor = false;
    PC->SetIgnoreMoveInput(false);
    PC->SetIgnoreLookInput(false);

    FInputModeGameOnly GameMode;
    PC->SetInputMode(GameMode);
}

