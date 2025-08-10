// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "InteractionUIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInteractionUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionUIComponent();


public:
	// 에디터에서 세팅할 필드
	UPROPERTY(EditAnywhere, Category = "Interaction")
	UDataTable* ArtifactTable;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	TSubclassOf<UUserWidget> ArtifactWidgetClass;

private:
	UUserWidget* ArtifactWidget = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 바인딩 콜백
	UFUNCTION()
	void OnActorInteracted(int32 ArtifactID);

	UFUNCTION(BlueprintCallable)
	void HideArtifactUI();

		
};
