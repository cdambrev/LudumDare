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
	UEnemiesManager* enemiesManager = GetEnemyManager();
	TArray<ABaseEnemy*> enemies = enemiesManager->GetEnemiesOnScreen();
	ABaseEnemy** result = enemies.FindByPredicate([&](ABaseEnemy* enemy)
	{
		APossessTheBabyCharacter* player = Cast<APossessTheBabyCharacter>(GetOwner());
		return CanHit(player->GetActorLocation(), player->GetFacingRight(), enemy->GetActorLocation());
	});

	if (result != nullptr)
	{
		return *result;
	}
	return nullptr;
}

bool UCombatComponent::TestAttackHero() const
{
	bool canHit = false;
	ABaseEnemy* enemy = Cast<ABaseEnemy>(GetOwner());
	if (IsValid(enemy))
	{
		APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
		APossessTheBabyCharacter* player = gameState->GetPlayer();

		canHit = CanHit(enemy->GetActorLocation(), enemy->GetFacingRight(), player->GetActorLocation());

	}
	return canHit;
}

bool UCombatComponent::CanHit(const FVector& attackerLocation, bool isAttackerFacingRight, const FVector& attackeeLocation) const
{
	bool result = false;

	float diffX = attackerLocation.X - attackeeLocation.X;
	bool onRight = diffX > 0.0f;
	float distanceX = FMath::Abs(diffX);
	float distanceZ = FMath::Abs(attackerLocation.Z - attackeeLocation.Z);
	result = distanceX < 100.f
		&& distanceZ < _precision
		&& (onRight != isAttackerFacingRight);
	return result;
}

UEnemiesManager* UCombatComponent::GetEnemyManager() const
{
	APossessTheBabyGameMode* gameMode = GetWorld()->GetAuthGameMode<APossessTheBabyGameMode>();
	return gameMode->GetEnemyManager();
}

void UCombatComponent::AttackHero()
{
	return;

	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	APossessTheBabyCharacter* player = gameState->GetPlayer();
	ABaseEnemy* enemy = Cast<ABaseEnemy>(GetOwner());
	player->OnHit(enemy->GetHitPoints());
}

void UCombatComponent::AttackEnemy(ABaseEnemy* enemy)
{
	enemy->ApplyDamage();
}
