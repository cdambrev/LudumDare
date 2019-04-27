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
	AEnemiesManager* enemies = GetEnemyManager();

	for ( )

	return true;
}

AEnemiesManager* UCombatComponent::GetEnemyManager() const
{
	APossessTheBabyGameMode* gameMode = GetWorld()->GetAuthGameMode<APossessTheBabyGameMode>();
	return gameMode->GetEnemyManager();
}

