// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InteractionComponent.h"
#include "InteractionInterface/IInteractiveTarget.h"
#include <Components/BoxComponent.h>
#include "InteractiveActor.generated.h"


// ��������Ʈ Ÿ�� ����
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

	// �����Ϳ��� ���� ������ ACTOR ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ArtifactID")
	int32 ActorID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FInteractionData InteractionData;

	/** ���� ���̴� �޽� ������Ʈ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	/** �� ���� ��Ʈ�� �� ������Ʈ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRootComponent;

	/** �浹/Ʈ���� ������ �ڽ� ������Ʈ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* InteractionWidget;

	/** Interaction ������Ʈ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInteractionComponent* InteractionComponent;

	// �̺�Ʈ ����ó
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnHoverSignature OnHoverStart;
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnHoverSignature OnHoverEnd;
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractSignature OnInteract;

	// IInteractiveTarget ����
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

	// Overlap �̺�Ʈ �ڵ鷯
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
	bool isHovering = false; // ���� ȣ�� ����
	
};
