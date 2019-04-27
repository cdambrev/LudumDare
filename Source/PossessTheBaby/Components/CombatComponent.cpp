#include "CombatComponent.h"

#include "EnemiesManager.h"
#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyGameState.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


bool UCombatComponent::TestAttackEnemy()
{
	UEnemiesManager* enemies = GetEnemyManager();
	return true;
}

UEnemiesManager* UCombatComponent::GetEnemyManager() const
{
	APossessTheBabyGameMode* gameMode = GetWorld()->GetAuthGameMode<APossessTheBabyGameMode>();
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return gameMode->GetEnemyManager(gameState->GetWorldState()->GetWorldState());
}

