// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InteractionComponent.h"
#include "InteractionInterface/IInteractiveTarget.h"
#include <Components/BoxComponent.h>
#include "InteractiveActor.generated.h"


// 델리게이트 타입 정의
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHoverSignature, int32, ActorID, float, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractSignature, int32, ActorID);

class UWidgetComponent;

UCLASS()
class AInteractiveActor : public AActor, public IIInteractiveTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveActor();

	// 에디터에서 조정 가능한 ACTOR ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ArtifactID")
	int32 ActorID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FInteractionData InteractionData;

	/** 씬에 보이는 메쉬 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	/** 씬 상의 루트용 빈 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRootComponent;

	/** 충돌/트리거 범위용 박스 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* InteractionWidget;

	/** Interaction 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractionComponent* InteractionComponent;

	// 이벤트 디스패처
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnHoverSignature OnHoverStart;
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnHoverSignature OnHoverEnd;
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractSignature OnInteract;

	// IInteractiveTarget 구현
	virtual void OnHoverStart_Implementation() override;
	virtual void OnHoverEnd_Implementation() override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FInteractionData GetInteractionData_Implementation() override;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:


public:

	// Overlap 이벤트 핸들러
	UFUNCTION()
	void HandleOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void HandleOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	bool isHovering = false; // 현재 호버 상태
	
};
