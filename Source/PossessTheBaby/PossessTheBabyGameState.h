// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PossessTheBabyGameState.generated.h"

class UWorldStateComponent;
class APossessTheBabyCharacter;
class UWaveStateComponent

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

	APossessTheBabyCharacter* GetPlayer() const;

	void SetPlayer(APossessTheBabyCharacter* player);

	UWaveStateComponent* GetDreamWaveState() const;
	
	UWaveStateComponent* GetNightmareWaveState() const;

private:
	UPROPERTY()
	UWorldStateComponent* WorldState = nullptr;

	UPROPERTY()
	APossessTheBabyCharacter* _player = nullptr;

	UPROPERTY()
	UWaveStateComponent* _dreamWaveState = nullptr;

	UPROPERTY()
	UWaveStateComponent* _nightmareWaveState = nullptr;
};
