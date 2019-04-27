// Fill out your copyright notice in the Description page of Project Settings.



#include "EnemiesManager.h"

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Components/WorldStateComponent.h"
#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyGameState.h"
#include "Engine/World.h"
#include "PossessTheBabyCharacter.h"

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

	GetWorldStateComponent()->OnWorldStateChanged.AddUObject(this, &UEnemiesManager::OnWorldStateChanged);
}

void UEnemiesManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldStateComponent()->OnWorldStateChanged.RemoveAll(this);
}

// Called every frame
void UEnemiesManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	APossessTheBabyCharacter* player = gameState->GetPlayer();
 
	FVector playerPos = player->GetActorLocation();

	for (int i=1; i<_ennemiesOnScreen.Num()-1; ++i)
	{
		float distanceMin = FVector::Dist2D(playerPos, _ennemiesOnScreen[i]->GetActorLocation());
		int min = i;
		for (int j=i; j<_ennemiesOnScreen.Num(); ++j)
		{
			float distancePlayerEnnemy = FVector::Dist2D(playerPos, _ennemiesOnScreen[j]->GetActorLocation());
			if (distancePlayerEnnemy < distanceMin)
			{
				min = j;
			}
		}
		if (min != i)
		{
			_ennemiesOnScreen.Swap(i, min);
		}
	}
	for (int i=0; i<_ennemiesOnScreen.Num(); ++i)
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

void UEnemiesManager::SpawnNewEnnemy(bool strong)
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

void UEnemiesManager::InitializeWave()
{
	APossessTheBabyGameMode * gameMode = Cast<APossessTheBabyGameMode>(GetWorld()->GetAuthGameMode());
	float timeBetweenEnemiesFirstWave = gameMode->GetLevelData().timeBetweenEnemiesPerWave[0];
	
	FTimerDelegate SpawnEnemyTimerDelegate = FTimerDelegate::CreateUObject(this, &UEnemiesManager::SpawnNewEnnemy, false);
	GetOwner()->GetWorldTimerManager().SetTimer(_spawnLightMonsterHandle, SpawnEnemyTimerDelegate, timeBetweenEnemiesFirstWave, true, 5.f);
	
	FTimerDelegate SpawnStrongEnemyTimerDelegate = FTimerDelegate::CreateUObject(this, &UEnemiesManager::SpawnNewEnnemy, true);
	GetOwner()->GetWorldTimerManager().SetTimer(_spawnStrongMonsterHandle, SpawnEnemyTimerDelegate, timeBetweenEnemiesFirstWave, true, 7.f);
}

UWorldStateComponent* UEnemiesManager::GetWorldStateComponent() const
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return gameState->GetWorldState();
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

void UEnemiesManager::SetIsForDream(bool forDream)
{
	_isForDream = forDream;
}

const TArray<ABaseEnemy*>& UEnemiesManager::GetEnemiesOnScreen() const
{
	return _ennemiesOnScreen;
}
