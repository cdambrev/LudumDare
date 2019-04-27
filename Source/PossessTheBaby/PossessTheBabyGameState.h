// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PossessTheBabyGameState.generated.h"

class UWorldStateComponent;

/**
 * 
 */
UCLASS()
class POSSESSTHEBABY_API APossessTheBabyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	APossessTheBabyGameState();

	UWorldStateComponent* GetWorldState() const;

private:
	UPROPERTY()
	UWorldStateComponent* WorldState = nullptr;
};
