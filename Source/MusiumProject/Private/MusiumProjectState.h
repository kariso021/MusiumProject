// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MusiumProjectState.generated.h"

/**
 * 
 */
UCLASS()
class AMusiumProjectState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastChatMessage(const FString& PlayerName, const FString& Message);
};
