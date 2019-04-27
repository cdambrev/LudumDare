// Fill out your copyright notice in the Description page of Project Settings.



#include "EnemiesManager.h"

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Components/WorldStateComponent.h"
#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyGameState.h"
#include "Engine/World.h"

// Sets default values
AEnemiesManager::AEnemiesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemiesManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeWave();

	GetWorldStateComponent()->OnWorldStateChanged.AddUObject(this, &AEnemiesManager::OnWorldStateChanged);
}

void AEnemiesManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldStateComponent()->OnWorldStateChanged.RemoveAll(this);
}

// Called every frame
void AEnemiesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for (int i=0; i<_ennemiesOnScreen.Num(); ++i)
	{
		int truc = 1;
	}
}

void AEnemiesManager::SpawnNewEnnemy(bool strong)
{
	if (_ennemiesOnScreen.Num() < _maxEnnemiesOnScreen)
	{
		if (strong)
		{
			FActorSpawnParameters spawnParameters;
			_ennemiesOnScreen.Add(GetWorld()->SpawnActor<ABaseEnemy>(strongMonsterClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, spawnParameters));
		}
		else
		{
			FActorSpawnParameters spawnParameters;
			_ennemiesOnScreen.Add(GetWorld()->SpawnActor<ABaseEnemy>(lightMonsterClass, FVector(0.f, 0.f, 0.f), FRotator::ZeroRotator, spawnParameters));
		}
	}
}

void AEnemiesManager::InitializeWave()
{
	APossessTheBabyGameMode * gameMode = Cast<APossessTheBabyGameMode>(GetWorld()->GetAuthGameMode());
	float timeBetweenEnemiesFirstWave = gameMode->GetLevelData().timeBetweenEnemiesPerWave[0];
	
	FTimerDelegate SpawnEnemyTimerDelegate = FTimerDelegate::CreateUObject(this, &AEnemiesManager::SpawnNewEnnemy, false);
	GetWorldTimerManager().SetTimer(_spawnLightMonsterHandle, SpawnEnemyTimerDelegate, timeBetweenEnemiesFirstWave, true, 5.f);
	
	FTimerDelegate SpawnStrongEnemyTimerDelegate = FTimerDelegate::CreateUObject(this, &AEnemiesManager::SpawnNewEnnemy, true);
	GetWorldTimerManager().SetTimer(_spawnStrongMonsterHandle, SpawnEnemyTimerDelegate, timeBetweenEnemiesFirstWave, true, 7.f);
}

UWorldStateComponent* AEnemiesManager::GetWorldStateComponent() const
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return gameState->GetWorldState();
}

void AEnemiesManager::OnWorldStateChanged(EWorldState worldState)
{
	if ((worldState == EWorldState::Dream && !_isForDream) || (worldState == EWorldState::Nightmare && _isForDream))
	{
		GetWorldTimerManager().PauseTimer(_spawnStrongMonsterHandle);
		GetWorldTimerManager().PauseTimer(_spawnLightMonsterHandle);
	}
	else
	{
		GetWorldTimerManager().UnPauseTimer(_spawnStrongMonsterHandle);
		GetWorldTimerManager().UnPauseTimer(_spawnLightMonsterHandle);
	}
}

void AEnemiesManager::SetIsForDream(bool forDream)
{
	_isForDream = forDream;
}
