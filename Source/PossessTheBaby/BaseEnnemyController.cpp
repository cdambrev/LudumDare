// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnnemyController.h"
#include "BaseEnemy.h"
#include "PossessTheBabyGameState.h"
#include "PossessTheBabyCharacter.h"
#include "Components/CombatComponent.h"
#include "Components/WorldStateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WorldLimitsComponent.h"
#include "Components/FakePerspectiveComponent.h"

ABaseEnnemyController::ABaseEnnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseEnnemyController::BeginPlay()
{
	Super::BeginPlay();
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	if (IsValid(gameState))
	{
		gameState->GetWorldState()->OnWorldStateChanged.AddDynamic(this, &ABaseEnnemyController::OnPlayerChangedWorld);
	}
}

void ABaseEnnemyController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	if (IsValid(gameState))
	{
		gameState->GetWorldState()->OnWorldStateChanged.RemoveDynamic(this, &ABaseEnnemyController::OnPlayerChangedWorld);
	}
	Super::EndPlay(EndPlayReason);
}

void ABaseEnnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ABaseEnemy* ennemy = GetEnemyPawn();

	if (IsValid(ennemy))
	{
		APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
		if (IsValid(gameState))
		{
			switch (ennemy->getCurrentState())
			{
			case EEnemyStateMachine::Spawning:
				if (gameState->GetWorldLimits()->IsInWorldLimits(ennemy->GetActorLocation()))
				{
					ennemy->GetFakePerspective()->SetRestrictInX(true);
					ennemy->SetCurrentState(EEnemyStateMachine::Wandering);
				}
				else
				{
					float input = 1.0f;
					if (ennemy->GetActorLocation().X > 0)
					{
						input = -1.0f;
					}

					ennemy->AddMovementInput(FVector(input, 0.f, 0.f));
					ennemy->SetFacingRight(input > 0);
				}
				break;
			case EEnemyStateMachine::Wandering:
				State_Wander();
				break;
			case EEnemyStateMachine::MovingToPlayer:
				ennemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
				if (!ennemy->canAttack())
				{
					ennemy->SetCurrentState(EEnemyStateMachine::Wandering);
				}
				else
				{
					if (!ennemy->GetCombatComponent()->TestAttackHero()) // out of reach
					{
						APossessTheBabyCharacter* player = gameState->GetPlayer();
						FVector diffToPlayer = player->GetActorLocation() - ennemy->GetActorLocation();
						diffToPlayer.Y = 0;
						ennemy->SetFacingRight(diffToPlayer.X > 0);
						ennemy->AddMovementInput(diffToPlayer);
					}
					else
					{
						ennemy->SetCurrentState(EEnemyStateMachine::Attack);
					}
				}
				break;
			case EEnemyStateMachine::Attack:
				ennemy->PlayFoleySound();
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
					ennemy->Attack();
					ennemy->SetCurrentState(EEnemyStateMachine::Attacking);
				}
				break;
			case EEnemyStateMachine::Attacking:
				if (ennemy->IsAttackEnabled())
				{
					ennemy->SetCurrentState(EEnemyStateMachine::Attack);
				}
				break;
			case EEnemyStateMachine::Dead:
				ennemy->PlayDieSound();
				ennemy->GetCharacterMovement()->StopActiveMovement();
				break;
			case EEnemyStateMachine::Frozen:
				ennemy->GetCharacterMovement()->StopActiveMovement();
				ennemy->ConsumeMovementInputVector();
				ennemy->GetCharacterMovement()->SetMovementMode(MOVE_None);
				break;
			default:
				break;
			}
		}
		if (ennemy->IsDead() && ennemy->getCurrentState() != EEnemyStateMachine::Dead)
		{
			ennemy->SetCurrentState(EEnemyStateMachine::Dead);
			OnEnnemyDied.Broadcast(ennemy);
		}
	}


	ennemy->FaceRotation(GetControlRotation(), DeltaTime);
}

void ABaseEnnemyController::State_Wander()
{
	ABaseEnemy* enemy = GetEnemyPawn();
	float inputX = FMath::FRandRange(-1.f, 1.f);
	float inputZ = FMath::FRandRange(-1.f, 1.f);

	enemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	enemy->AddMovementInput(FVector(inputX, 0.f, inputZ));
	enemy->SetFacingRight(inputX > 0);
	if (enemy->canAttack())
	{
		enemy->SetCurrentState(EEnemyStateMachine::MovingToPlayer);
	}
}


void ABaseEnnemyController::OnPlayerChangedWorld(EWorldState worldState)
{
	ABaseEnemy* enemy = GetEnemyPawn();
	if (IsValid(enemy))
	{
		if ((worldState == EWorldState::Dream && enemy->GetIsDreamWorld()) || (worldState == EWorldState::Nightmare && !enemy->GetIsDreamWorld()))
		{
			enemy->playAppear = true;
			enemy->SetCurrentState(EEnemyStateMachine::Wandering);
		}
		else
		{
			enemy->SetCurrentState(EEnemyStateMachine::Frozen);
		}
	}
}

void ABaseEnnemyController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn) {}

ABaseEnemy* ABaseEnnemyController::GetEnemyPawn() const
{
	return Cast<ABaseEnemy>(GetPawn());
}
