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
	ABaseEnemy* ennemy = Cast<ABaseEnemy>(GetPawn());
	if (IsValid(ennemy))
	{
		float XComp = FMath::FRandRange(-1.f, 1.f);
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
				ennemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
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
						APossessTheBabyCharacter* player = gameState->GetPlayer();
						FVector diffToPlayer = player->GetActorLocation() - ennemy->GetActorLocation();
						diffToPlayer.Y = 0;
						ennemy->SetFacingRight(diffToPlayer.X > 0);
						ennemy->AddMovementInput(diffToPlayer);
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

void ABaseEnnemyController::OnPlayerChangedWorld(EWorldState worldState)
{
	ABaseEnemy* ennemy = Cast<ABaseEnemy>(GetPawn());
	if (IsValid(ennemy))
	{
		if ((worldState == EWorldState::Dream && ennemy->GetIsDreamWorld()) || (worldState == EWorldState::Nightmare && !ennemy->GetIsDreamWorld()))
		{
			ennemy->SetCurrentState(EEnemyStateMachine::Wandering);
		}
		else
		{
			ennemy->SetCurrentState(EEnemyStateMachine::Frozen);
		}
	}
}

void ABaseEnnemyController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn) {}