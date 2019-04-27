// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnnemyController.h"
#include "BaseEnemy.h"
#include "PossessTheBabyGameState.h"
#include "PossessTheBabyCharacter.h"

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
		if (ennemy->GetCurrentHp() <= 0 && ennemy->getCurrentState() != EEnemyStateMachine::Dead)
		{
			ennemy->SetCurrentState(EEnemyStateMachine::Dead);
		}

		switch (ennemy->getCurrentState())
		{
		case EEnemyStateMachine::Wandering:
			ennemy->AddMovementInput(FVector(FMath::FRandRange(1.f, 5.f), 0.f, FMath::FRandRange(1.f, 5.f)));
			if (ennemy->canAttack())
			{
				ennemy->SetCurrentState(EEnemyStateMachine::Attacking);
			}
			break;
		case EEnemyStateMachine::Attacking:
			if (!ennemy->canAttack())
			{
				ennemy->SetCurrentState(EEnemyStateMachine::Wandering);
			}
			if (true /* out of reach */)
			{
				APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
				APossessTheBabyCharacter* player = gameState->GetPlayer();
				FVector distanceToPlayer = player->GetActorLocation() - ennemy->GetActorLocation();
				ennemy->AddMovementInput(distanceToPlayer);
			}
			break;
		case EEnemyStateMachine::Frozen:
		case EEnemyStateMachine::Dead:
		default:
			break;
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
