// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PossessTheBabyGameState.generated.h"

class UWorldStateComponent;
class APossessTheBabyCharacter;
class UWaveStateComponent;
class UWorldLimitsComponent;

/**
 * 
 */
UCLASS()
class POSSESSTHEBABY_API APossessTheBabyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	APossessTheBabyGameState();

	UFUNCTION(BlueprintGetter, Category="World State")
	UWorldStateComponent* GetWorldState() const;

	APossessTheBabyCharacter* GetPlayer() const;

	void SetPlayer(APossessTheBabyCharacter* player);

	UWaveStateComponent* GetDreamWaveState() const;
	
	UWaveStateComponent* GetNightmareWaveState() const;

	UFUNCTION(BlueprintGetter, Category = "World Limits")
	UWorldLimitsComponent* GetWorldLimits() const;

private:
	UPROPERTY(BlueprintGetter= GetWorldState)
	UWorldStateComponent* WorldState = nullptr;

	UPROPERTY()
	APossessTheBabyCharacter* _player = nullptr;

	UPROPERTY()
	UWaveStateComponent* _dreamWaveState = nullptr;

	UPROPERTY()
	UWaveStateComponent* _nightmareWaveState = nullptr;

	UPROPERTY(BlueprintGetter= GetWorldLimits)
	UWorldLimitsComponent* _worldLimits = nullptr;
};
