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
   // 1) MeshComponent �� ���� ��Ƽ���� ����
    if (UStaticMeshComponent* Mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>())
    {
        CachedMesh = Mesh;
    }

    // 2) WidgetComponent ĳ��
    if (Owner)
    {
        CachedWidget = Owner->FindComponentByClass<UWidgetComponent>();
        if (CachedWidget)
        {
            // ���� �� UI�� ���ܵα�
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
	// 1) MeshComponent�� Overlay �Ķ���͸� �����Ͽ� �߱� ȿ�� ����
    ApplyOverlay();


    // 2) Widget ���̱�
    if (CachedWidget)
        CachedWidget->SetVisibility(true);
}

void UInteractionComponent::HoverEnd()
{
	// 1) MeshComponent�� Overlay �Ķ���͸� None���� �����Ͽ� �߱� ȿ�� ����
	ClearOverlay();

    // 2) Widget �����
    if (CachedWidget)
        CachedWidget->SetVisibility(false);
}

void UInteractionComponent::DoInteract(AActor* Interactor)
{

}

