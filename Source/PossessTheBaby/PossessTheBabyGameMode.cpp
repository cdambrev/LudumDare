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
	
	FActorSpawnParameters spawnParameters;
	_dreamWorldEnnemiesManager = CreateDefaultSubobject<UEnemiesManager>(TEXT("DreamWorldManager"));
	_nightmareEnnemiesManager = CreateDefaultSubobject<UEnemiesManager>(TEXT("NightmareWorldManager"));
}

void APossessTheBabyGameMode::BeginPlay()
{
	_dreamWorldEnnemiesManager->SetIsForDream(true);
	_nightmareEnnemiesManager->SetIsForDream(false);
}

FLevelData APossessTheBabyGameMode::GetLevelData() const
{
	return levelData;
}
