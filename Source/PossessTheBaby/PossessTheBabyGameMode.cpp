// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyCharacter.h"
#include "PossessTheBabyGameState.h"
#include "EnemiesManager.h"
#include "Engine/World.h"

APossessTheBabyGameMode::APossessTheBabyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set default pawn class to our character
	DefaultPawnClass = APossessTheBabyCharacter::StaticClass();	
	GameStateClass = APossessTheBabyGameState::StaticClass();
	
	FActorSpawnParameters spawnParameters;
	_dreamWorldEnnemiesManager = CreateDefaultSubobject<UEnemiesManager>(TEXT("DreamWorldManager"));
	_nightmareEnnemiesManager = CreateDefaultSubobject<UEnemiesManager>(TEXT("NightmareWorldManager"));
}

void APossessTheBabyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void APossessTheBabyGameMode::Tick(float DeltaSeconds)
{
	APossessTheBabyCharacter* player = GetGameState<APossessTheBabyGameState>()->GetPlayer();
	if (IsValid(player) && player->IsDead())
	{
		FTimerHandle handle;
		FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &APossessTheBabyGameMode::GameOver);
		GetWorldTimerManager().SetTimer(handle, timerDelegate, 3.0f, false);
		SetActorTickEnabled(false);
	}
}

FLevelData APossessTheBabyGameMode::GetLevelData() const
{
	return levelData;
}

UEnemiesManager* APossessTheBabyGameMode::GetEnemyManager(EWorldState worldState) const
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

UEnemiesManager* APossessTheBabyGameMode::GetEnemyManager() const
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return GetEnemyManager(gameState->GetWorldState()->GetWorldState());
}

void APossessTheBabyGameMode::GameOver()
{

}
