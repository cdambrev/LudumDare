// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyCharacter.h"
#include "PossessTheBabyGameState.h"
#include "EnemiesManager.h"
#include "Engine/World.h"

APossessTheBabyGameMode::APossessTheBabyGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = APossessTheBabyCharacter::StaticClass();	
	GameStateClass = APossessTheBabyGameState::StaticClass();
}

void APossessTheBabyGameMode::BeginPlay()
{
	FActorSpawnParameters spawnParameters;
	_dreamWorldEnnemiesManager = GetWorld()->SpawnActor<AEnemiesManager>(enemiesManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParameters);
	_dreamWorldEnnemiesManager->SetIsForDream(true);
	_nightmareEnnemiesManager = GetWorld()->SpawnActor<AEnemiesManager>(enemiesManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParameters);
	_nightmareEnnemiesManager->SetIsForDream(false);
}

FLevelData APossessTheBabyGameMode::GetLevelData() const
{
	return levelData;
}

AEnemiesManager* APossessTheBabyGameMode::GetEnemyManager(EWorldState worldState) const
{
	if (worldState == EWorldState::Dream)
	{
		return _dreamWorldEnnemiesManager;
	}
	else
	{
		return _nightmareEnnemiesManager;
	}
}

AEnemiesManager* APossessTheBabyGameMode::GetEnemyManager() const
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return GetEnemyManager(gameState->GetWorldState()->GetWorldState());
}
