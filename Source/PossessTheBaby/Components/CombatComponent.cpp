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

ABaseEnemy* UCombatComponent::TestAttackEnemy() const
{
	ABaseEnemy* result = nullptr;
	UEnemiesManager* enemiesManager = GetEnemyManager();
	TArray<ABaseEnemy*> enemies = enemiesManager->GetEnemiesOnScreen();
	result = *enemies.FindByPredicate([&](ABaseEnemy* enemy) {
		APossessTheBabyCharacter* player = Cast<APossessTheBabyCharacter>(GetOwner());
		FVector playerLocation = player->GetActorLocation();
		FVector ennemyLocation = enemy->GetActorLocation();
		return FMath::Abs(playerLocation.X - ennemyLocation.X) < 100.f && FMath::Abs(playerLocation.Z - ennemyLocation.Z) < _precision
			&& (playerLocation.X - ennemyLocation.X > 0 && !player->GetFacingRight() || playerLocation.X - ennemyLocation.X < 0 && player->GetFacingRight());
	});

	return result;
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
		if (FMath::Abs(playerLocation.X - ennemyLocation.X) < 100.f && FMath::Abs(playerLocation.Z - ennemyLocation.Z) < _precision
			&& (ennemyLocation.X - playerLocation.X > 0 && !enemy->GetFacingRight() || ennemyLocation.X - playerLocation.X < 0 && enemy->GetFacingRight()))
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
	if (IsValid(enemy))
	{
		player->GetHealth()->ApplyDamage(enemy->GetHitPoints());
	}
}

void UCombatComponent::AttackEnemy(ABaseEnemy* ennemy)
{

}
