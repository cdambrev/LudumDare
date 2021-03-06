// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "LevelDescriptor.generated.h"

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 maxWave;
	
	UPROPERTY(EditAnywhere)
	TArray<float> timeBetweenEnemiesPerWave;

	UPROPERTY(EditDefaultsOnly)
	TArray<float> enemiesCountPerWave;

	UPROPERTY(EditDefaultsOnly)
	TArray<float> lightEnnemyCountPerWave;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<float> strongEnnemyCountPerWave;

	UPROPERTY(EditDefaultsOnly)
	TArray<float> _maxEnemiesOnScreenPerWave;
};
