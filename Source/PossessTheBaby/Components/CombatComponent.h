// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UEnemiesManager;
class ABaseEnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POSSESSTHEBABY_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	ABaseEnemy* TestAttackEnemy() const;
	bool TestAttackHero() const;

	void AttackHero();

	void AttackEnemy(ABaseEnemy* ennemy);

private:
	UEnemiesManager* GetEnemyManager() const;
	bool CanHit(const FVector& attackerLocation, bool isAttackerFacingRight, const FVector& attackeeLocation) const;

	float _precision = 10.0f;
};
