#include "CombatComponent.h"

#include "EnemiesManager.h"
#include "PossessTheBabyGameMode.h"
#include "PossessTheBabyGameState.h"
#include "PossessTheBabyCharacter.h"
#include "BaseEnemy.h"
#include "Components/HealthComponent.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UCombatComponent::TestAttackEnemy() const
{
	UEnemiesManager* enemies = GetEnemyManager();
	return true;
}

bool UCombatComponent::TestAttackHero() const
{
	bool canAttack = false;
	ABaseEnemy* enemy = Cast<ABaseEnemy>(GetOwner());
	if (IsValid(enemy))
	{
		APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
		APossessTheBabyCharacter* player = gameState->GetPlayer();
		FVector playerLocation = player->GetActorLocation();
		FVector ennemyLocation = enemy->GetActorLocation();
		float diffX = ennemyLocation.X - playerLocation.X;
		bool enemyOnRight = diffX > 0.0f;
		float distanceX = FMath::Abs(diffX);
		float distanceZ = FMath::Abs(playerLocation.Z - ennemyLocation.Z);
		if (distanceX < 100.f
			&& distanceZ < _precision
			&& (enemyOnRight != enemy->GetFacingRight()))
		{
			canAttack = true;
		}
	}
	return canAttack;
}

UEnemiesManager* UCombatComponent::GetEnemyManager() const
{
	APossessTheBabyGameMode* gameMode = GetWorld()->GetAuthGameMode<APossessTheBabyGameMode>();
	return gameMode->GetEnemyManager();
}

void UCombatComponent::AttackHero()
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	APossessTheBabyCharacter* player = gameState->GetPlayer();
	ABaseEnemy* enemy = Cast<ABaseEnemy>(GetOwner());
	player->OnHit(enemy->GetHitPoints());
}

void UCombatComponent::AttackEnemy(ABaseEnemy* ennemy)
{

}
