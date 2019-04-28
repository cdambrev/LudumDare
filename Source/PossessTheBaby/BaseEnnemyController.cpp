// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnnemyController.h"
#include "BaseEnemy.h"
#include "PossessTheBabyGameState.h"
#include "PossessTheBabyCharacter.h"
#include "Components/CombatComponent.h"

ABaseEnnemyController::ABaseEnnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ABaseEnemy* ennemy = Cast<ABaseEnemy>(GetPawn());
	if (IsValid(ennemy))
	{
		float XComp = FMath::FRandRange(-1.f, 1.f);
		switch (ennemy->getCurrentState())
		{
		case EEnemyStateMachine::Wandering:
			ennemy->AddMovementInput(FVector(XComp, 0.f, FMath::FRandRange(-1.f, 1.f)));
			ennemy->SetFacingRight(XComp > 0);
			if (ennemy->canAttack())
			{
				ennemy->SetCurrentState(EEnemyStateMachine::MovingToPlayer);
			}
			break;
		case EEnemyStateMachine::MovingToPlayer:
			if (!ennemy->canAttack())
			{
				ennemy->SetCurrentState(EEnemyStateMachine::Wandering);
			}
			else
			{
				if (!ennemy->GetCombatComponent()->TestAttackHero()) // out of reach
				{
					APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
					APossessTheBabyCharacter* player = gameState->GetPlayer();
					FVector distanceToPlayer = player->GetActorLocation() - ennemy->GetActorLocation();
					distanceToPlayer.Y = 0;
					ennemy->SetFacingRight(distanceToPlayer.X > 0);
					ennemy->AddMovementInput(distanceToPlayer);
				}
				else
				{
					ennemy->SetCurrentState(EEnemyStateMachine::Attacking);
				}
			}
			break;
		case EEnemyStateMachine::Attacking:
			if (!ennemy->canAttack())
			{
				ennemy->SetCurrentState(EEnemyStateMachine::Wandering);
			}
			else if (!ennemy->GetCombatComponent()->TestAttackHero())
			{
				ennemy->SetCurrentState(EEnemyStateMachine::MovingToPlayer);
			}
			else
			{
				// animation
				ennemy->GetCombatComponent()->AttackHero();
			}
			break;
		case EEnemyStateMachine::Dead:
			ennemy->Destroy();
			break;
		case EEnemyStateMachine::Frozen:
		default:
			break;
		}
		if (ennemy->GetCurrentHp() <= 0 && ennemy->getCurrentState() != EEnemyStateMachine::Dead)
		{
			ennemy->SetCurrentState(EEnemyStateMachine::Dead);
			OnEnnemyDied.Broadcast(ennemy);
		}
	}
}

void ABaseEnnemyController::OnPlayerChangedWorld()
{
	ABaseEnemy* ennemy = Cast<ABaseEnemy>(GetPawn());
	if (true /*player in this world*/)
	{
		if (true /* player in reach*/)
		{
			ennemy->SetCurrentState(EEnemyStateMachine::Attacking);
		}
		else
		{
			ennemy->SetCurrentState(EEnemyStateMachine::Attacking);
		}
	}
	else
	{
			ennemy->SetCurrentState(EEnemyStateMachine::Attacking);
	}
}
