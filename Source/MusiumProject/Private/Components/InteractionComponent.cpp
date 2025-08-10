// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


UStaticMeshComponent* UInteractionComponent::GetMeshComponent()
{
    if (CachedMesh) return CachedMesh;
    if (AActor* Owner = GetOwner())
    {
        CachedMesh = Owner->FindComponentByClass<UStaticMeshComponent>();
    }
    return CachedMesh;
}

void UInteractionComponent::ApplyOverlay()
{
    if (CachedMesh && OverlayMaterial)
    {
		CachedMesh->SetOverlayMaterial(OverlayMaterial);
    }
}

void UInteractionComponent::ClearOverlay()
{
    if (CachedMesh)
    {
        CachedMesh->SetOverlayMaterial(nullptr);
    }
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

    AActor* Owner = GetOwner();
   // 1) MeshComponent 및 원본 머티리얼 저장
    if (UStaticMeshComponent* Mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
    {
        CachedMesh = Mesh;
    }

    // 2) WidgetComponent 캐시
    if (Owner)
    {
        CachedWidget = Owner->FindComponentByClass<UWidgetComponent>();
        if (CachedWidget)
        {
            // 시작 시 UI는 숨겨두기
            CachedWidget->SetVisibility(false);
        }
    }
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionComponent::HoverStart()
{
	// 1) MeshComponent의 Overlay 파라미터를 변경하여 발광 효과 적용
    ApplyOverlay();


    // 2) Widget 보이기
    if (CachedWidget)
        CachedWidget->SetVisibility(true);
}

void UInteractionComponent::HoverEnd()
{
	// 1) MeshComponent의 Overlay 파라미터를 None으로 변경하여 발광 효과 제거
	ClearOverlay();

    // 2) Widget 숨기기
    if (CachedWidget)
        CachedWidget->SetVisibility(false);
}

void UInteractionComponent::DoInteract(AActor* Interactor)
{

}

