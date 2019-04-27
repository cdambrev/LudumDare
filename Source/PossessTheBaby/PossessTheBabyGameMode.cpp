// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyCharacter.h"
#include "PossessTheBabyGameState.h"
#include "EnemiesManager.h"

APossessTheBabyGameMode::APossessTheBabyGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = APossessTheBabyCharacter::StaticClass();	
	GameStateClass = APossessTheBabyGameState::StaticClass();
}

void APossessTheBabyGameMode::BeginPlay()
{
	FActorSpawnParameters spawnParameters;
	ennemiesManager = GetWorld()->SpawnActor<AEnemiesManager>(FVector::ZeroVector, FRotator::ZeroRotator);
}
