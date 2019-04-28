// Fill out your copyright notice in the Description page of Project Settings.



#include "EnemiesManager.h"

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyGameState.h"
#include "Engine/World.h"
#include "PossessTheBabyCharacter.h"
#include "BaseEnnemyController.h"
#include "Components/FakePerspectiveComponent.h"
#include "Components/HealthComponent.h"

// Sets default values
UEnemiesManager::UEnemiesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UEnemiesManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeWave();

	GetWorldStateComponent()->OnWorldStateChanged.AddDynamic(this, &UEnemiesManager::OnWorldStateChanged);
}

void UEnemiesManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWorldStateComponent* worldStateComponent = GetWorldStateComponent();
	if (IsValid(worldStateComponent))
	{
		worldStateComponent->OnWorldStateChanged.RemoveAll(this);
	}
	_ennemiesOnScreen.Empty();
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UEnemiesManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	APossessTheBabyCharacter* player = gameState->GetPlayer();
 
	FVector playerPos = player->GetActorLocation();

	for (int i=0; i<_ennemiesOnScreen.Num()-1; ++i)
	{
		if (IsValid(_ennemiesOnScreen[i]))
		{
			float distanceMin = FVector::Dist2D(playerPos, _ennemiesOnScreen[i]->GetActorLocation());
			int min = i;
			for (int j = i; j < _ennemiesOnScreen.Num(); ++j)
			{
				if (IsValid(_ennemiesOnScreen[j]))
				{
					float distancePlayerEnnemy = FVector::Dist2D(playerPos, _ennemiesOnScreen[j]->GetActorLocation());
					if (distancePlayerEnnemy < distanceMin)
					{
						min = j;
					}
				}
			}
			if (min != i)
			{
				_ennemiesOnScreen.Swap(i, min);
			}
		}
	}
	for (int i=0; i<_ennemiesOnScreen.Num(); ++i)
	{
		if (IsValid(_ennemiesOnScreen[i]))
		{
			if (i < _maxEnnemiesAttacking)
			{
				_ennemiesOnScreen[i]->SetAllowedToAttack(true);
			}
			else
			{
				_ennemiesOnScreen[i]->SetAllowedToAttack(false);
			}
		}
	}
}

void UEnemiesManager::SpawnNewEnnemy(bool strong)
{
	if (_enemiesSpawnedThisWave < _currentEnemiesPerWave)
	{
		if (_ennemiesOnScreen.Num() < _maxEnnemiesOnScreen)
		{
			if (strong && _currentStrongEnemiesSpawned < _currentStrongEnemiesPerWave)
			{
				SpawnEnemy(strongMonsterClass);
			}
			else if (!strong && _currentLightEnemiesSpawned < _currentLightEnemiesPerWave)
			{
				SpawnEnemy(lightMonsterClass);
			}
		}
		if (_enemiesSpawnedThisWave == _currentEnemiesPerWave)
		{
			InitializeNewWave();
		}
	}
}

void UEnemiesManager::InitializeWave()
{
	APossessTheBabyGameMode * gameMode = Cast<APossessTheBabyGameMode>(GetWorld()->GetAuthGameMode());
	FLevelData levelData = gameMode->GetLevelData();
	float timeBetweenEnemiesFirstWave = levelData.timeBetweenEnemiesPerWave[0];
	
	_maxEnnemiesOnScreen = levelData._maxEnemiesOnScreenPerWave[0];
	_currentEnemiesPerWave = levelData.enemiesCountPerWave[0];
	_currentStrongEnemiesPerWave = levelData.strongEnnemyCountPerWave[0];
	_currentLightEnemiesPerWave = levelData.lightEnnemyCountPerWave[0];
	
	_enemiesSpawnedThisWave = 0;
	_currentLightEnemiesSpawned = 0;
	_currentStrongEnemiesSpawned = 0;
	
	FTimerDelegate SpawnEnemyTimerDelegate = FTimerDelegate::CreateUObject(this, &UEnemiesManager::SpawnNewEnnemy, false);
	GetOwner()->GetWorldTimerManager().SetTimer(_spawnLightMonsterHandle, SpawnEnemyTimerDelegate, timeBetweenEnemiesFirstWave, true, 4.f);
	
	FTimerDelegate SpawnStrongEnemyTimerDelegate = FTimerDelegate::CreateUObject(this, &UEnemiesManager::SpawnNewEnnemy, true);
	GetOwner()->GetWorldTimerManager().SetTimer(_spawnStrongMonsterHandle, SpawnStrongEnemyTimerDelegate, timeBetweenEnemiesFirstWave, true, 6.f);
		
	if (!_isForDream)
	{
		GetOwner()->GetWorldTimerManager().PauseTimer(_spawnLightMonsterHandle);
		GetOwner()->GetWorldTimerManager().PauseTimer(_spawnStrongMonsterHandle);
	}
}

UWorldStateComponent* UEnemiesManager::GetWorldStateComponent() const
{
	UWorldStateComponent* worldStateComponent = nullptr;
	UWorld* world = GetWorld();
	if (IsValid(world))
	{
		APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
		if (IsValid(gameState))
		{
			worldStateComponent = gameState->GetWorldState();
		}
	}
	return worldStateComponent;
}

void UEnemiesManager::OnWorldStateChanged(EWorldState worldState)
{
	if ((worldState == EWorldState::Dream && !_isForDream) || (worldState == EWorldState::Nightmare && _isForDream))
	{
		GetOwner()->GetWorldTimerManager().PauseTimer(_spawnStrongMonsterHandle);
		GetOwner()->GetWorldTimerManager().PauseTimer(_spawnLightMonsterHandle);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().UnPauseTimer(_spawnStrongMonsterHandle);
		GetOwner()->GetWorldTimerManager().UnPauseTimer(_spawnLightMonsterHandle);
	}
}

void UEnemiesManager::InitializeNewWave()
{
	_enemiesSpawnedThisWave = 0;
	_currentLightEnemiesSpawned = 0;
	_currentStrongEnemiesSpawned = 0;
	
	APossessTheBabyGameMode * gameMode = Cast<APossessTheBabyGameMode>(GetWorld()->GetAuthGameMode());
	FLevelData levelData = gameMode->GetLevelData();
	int32 maxWave = levelData.maxWave;
	if (_currentWave < maxWave)
	{
		_currentWave++;
	}
	
	_maxEnnemiesOnScreen = levelData._maxEnemiesOnScreenPerWave[_currentWave];
	_currentEnemiesPerWave = levelData.enemiesCountPerWave[_currentWave];
	_currentStrongEnemiesPerWave = levelData.strongEnnemyCountPerWave[_currentWave];
	_currentLightEnemiesPerWave = levelData.lightEnnemyCountPerWave[_currentWave];
	
	FTimerManager& timerManager = GetOwner()->GetWorldTimerManager();
	timerManager.ClearTimer(_spawnLightMonsterHandle);
	timerManager.ClearTimer(_spawnStrongMonsterHandle);
	
	float timeBetweenEnemies = levelData.timeBetweenEnemiesPerWave[_currentWave];
	
	FTimerDelegate SpawnEnemyTimerDelegate = FTimerDelegate::CreateUObject(this, &UEnemiesManager::SpawnNewEnnemy, false);
	timerManager.SetTimer(_spawnLightMonsterHandle, SpawnEnemyTimerDelegate, timeBetweenEnemies, true, 6.f);
	
	FTimerDelegate SpawnStrongEnemyTimerDelegate = FTimerDelegate::CreateUObject(this, &UEnemiesManager::SpawnNewEnnemy, true);
	timerManager.SetTimer(_spawnStrongMonsterHandle, SpawnStrongEnemyTimerDelegate, timeBetweenEnemies, true, 8.f);
}

void UEnemiesManager::SetIsForDream(bool forDream)
{
	_isForDream = forDream;
}

const TArray<ABaseEnemy*>& UEnemiesManager::GetEnemiesOnScreen() const
{
	return _ennemiesOnScreen;
}

void UEnemiesManager::OnEnnemyDied(ABaseEnemy* ennemy)
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	if (IsValid(gameState))
	{
		APossessTheBabyCharacter* player = gameState->GetPlayer();
		if (_isForDream)
		{
			player->GetHealth()->AddNightmarePoints(ennemy->GetSoulsRewarded());
		}
		else
		{
			player->GetHealth()->AddDreamPoints(ennemy->GetSoulsRewarded());
		}
	}
	_ennemiesOnScreen.RemoveSingle(ennemy);
}

void UEnemiesManager::SpawnEnemy(TSubclassOf<ABaseEnemy> enemyClass)
{
	FActorSpawnParameters spawnParameters;
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	ABaseEnemy* ennemy = GetWorld()->SpawnActor<ABaseEnemy>(enemyClass, gameState->GetRandomSpawnPoint(), FRotator::ZeroRotator, spawnParameters);
	ABaseEnnemyController* controller = Cast<ABaseEnnemyController>(ennemy->GetController());
	if (IsValid(controller))
	{
		controller->OnEnnemyDied.AddUObject(this, &UEnemiesManager::OnEnnemyDied);
	}
	ennemy->SetIsDreamWorld(_isForDream);
	ennemy->GetFakePerspective()->SetRestrictInX(false);
	_ennemiesOnScreen.Add(ennemy);
	_enemiesSpawnedThisWave++;
}
