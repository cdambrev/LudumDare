// Fill out your copyright notice in the Description page of Project Settings.



#include "EnemiesManager.h"

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "Components/WorldStateComponent.h"
#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyGameState.h"
#include "Engine/World.h"
#include "PossessTheBabyCharacter.h"
#include "BaseEnnemyController.h"
#include "Components/FakePerspectiveComponent.h"

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
	if (_ennemiesOnScreen.Num() < _maxEnnemiesOnScreen)
	{
		if (strong)
		{
			FActorSpawnParameters spawnParameters;
			APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
			ABaseEnemy* ennemy = GetWorld()->SpawnActor<ABaseEnemy>(strongMonsterClass, gameState->GetRandomSpawnPoint(), FRotator::ZeroRotator, spawnParameters);
			ABaseEnnemyController* controller = Cast<ABaseEnnemyController>(ennemy->GetController());
			if (IsValid(controller))
			{
				controller->OnEnnemyDied.AddUObject(this, &UEnemiesManager::OnEnnemyDied);
			}
			ennemy->SetIsDreamWorld(_isForDream);
			ennemy->getFakePerspective()->SetRestrictInX(false);
			_ennemiesOnScreen.Add(ennemy);
		}
		else
		{
			FActorSpawnParameters spawnParameters;
			APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
			ABaseEnemy* ennemy = GetWorld()->SpawnActor<ABaseEnemy>(lightMonsterClass, gameState->GetRandomSpawnPoint(), FRotator::ZeroRotator, spawnParameters);
			ABaseEnnemyController* controller = Cast<ABaseEnnemyController>(ennemy->GetController());
			if (IsValid(controller))
			{
				controller->OnEnnemyDied.AddUObject(this, &UEnemiesManager::OnEnnemyDied);
			}
			ennemy->SetIsDreamWorld(_isForDream);
			ennemy->getFakePerspective()->SetRestrictInX(false);
			_ennemiesOnScreen.Add(ennemy);
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
	GetOwner()->GetWorldTimerManager().SetTimer(_spawnStrongMonsterHandle, SpawnEnemyTimerDelegate, timeBetweenEnemiesFirstWave, true, 12.f);
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
	_ennemiesOnScreen.RemoveSingle(ennemy);
}
