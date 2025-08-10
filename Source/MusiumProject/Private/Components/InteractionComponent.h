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


	// 인터렉션 Hover
	void HoverStart();
	void HoverEnd();

	// TODO -> UI 연결하기
	void DoInteract(AActor* Interactor);

protected:

	// MeshComponent 캐시 & Dynamic Materials 생성용 다중 머티리얼을 염려해서 배열 형태로
	UStaticMeshComponent* GetMeshComponent();

	/** Emissive 파라미터 이름 */
	UPROPERTY(EditAnywhere, Category = "Highlight")
	FName EmissiveStrengthParam = TEXT("EmissiveStrength");

	/** Hover 시 발광 세기 */
	UPROPERTY(EditAnywhere, Category = "Highlight")
	float HoverEmissiveStrength = 10.0f;

	// 오버레이 마티리얼
	UPROPERTY(EditAnywhere, Category = "Overlay")
	UMaterialInterface* OverlayMaterial = nullptr;

private:
	UStaticMeshComponent* CachedMesh = nullptr;

	UWidgetComponent* CachedWidget = nullptr;

	TArray<UMaterialInterface*> OriginalMaterials;

	/** 오버레이 적용 (인덱스 지정) */
	void ApplyOverlay();
	/** 오버레이 해제 (None 상태로 복원) */
	void ClearOverlay();


		
};
