// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Components/WidgetComponent.h>
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// ���ͷ��� Hover
	void HoverStart();
	void HoverEnd();

	// TODO -> UI �����ϱ�
	void DoInteract(AActor* Interactor);

protected:

	// MeshComponent ĳ�� & Dynamic Materials ������ ���� ��Ƽ������ �����ؼ� �迭 ���·�
	UStaticMeshComponent* GetMeshComponent();

	/** Emissive �Ķ���� �̸� */
	UPROPERTY(EditAnywhere, Category = "Highlight")
	FName EmissiveStrengthParam = TEXT("EmissiveStrength");

	/** Hover �� �߱� ���� */
	UPROPERTY(EditAnywhere, Category = "Highlight")
	float HoverEmissiveStrength = 10.0f;

	// �������� ��Ƽ����
	UPROPERTY(EditAnywhere, Category = "Overlay")
	UMaterialInterface* OverlayMaterial = nullptr;

private:
	UStaticMeshComponent* CachedMesh = nullptr;

	UWidgetComponent* CachedWidget = nullptr;

	TArray<UMaterialInterface*> OriginalMaterials;

	/** �������� ���� (�ε��� ����) */
	void ApplyOverlay();
	/** �������� ���� (None ���·� ����) */
	void ClearOverlay();


		
};
