// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/FakePerspectiveComponent.h"
#include "Components/CombatComponent.h"
#include "Components/FlickerComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
	: Super()
{
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	_currentHp = maxHp;
	GetCharacterMovement()->MaxFlySpeed = speed;
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAnimation();
}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	if (IsDead())
	{
		GetSprite()->SetFlipbook(DieAnimation);
	}
	else
	{
		// Are we moving or standing still?
		UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
		if (GetSprite()->GetFlipbook() != DesiredAnimation)
		{
			GetSprite()->SetFlipbook(DesiredAnimation);
		}
	}
}

bool ABaseEnemy::canAttack() const
{
	return _allowedToAttack || GetCombatComponent()->TestAttackHero();
}

EEnemyStateMachine ABaseEnemy::getCurrentState() const
{
	return _currentState;
}

void ABaseEnemy::SetCurrentState(EEnemyStateMachine nextState)
{
	_currentState = nextState;
}

void ABaseEnemy::SetAllowedToAttack(bool allowed)
{
	_allowedToAttack = allowed;
}

bool ABaseEnemy::IsDead() const
{
	return GetCurrentHp() <= 0.0f;
}

float ABaseEnemy::GetCurrentHp() const
{
	return _currentHp;
}

float ABaseEnemy::GetHitPoints() const
{
	return hit;
}

float ABaseEnemy::GetSoulsRewarded() const
{
	return soulsReward;
}

void ABaseEnemy::ApplyDamage()
{
	GetFlicker()->TintFlick(0.2f, FColor::Red);
	_currentHp--;
}

void ABaseEnemy::SetIsDreamWorld(bool isDreamWorld)
{
	_isDreamWorld = isDreamWorld;
}

bool ABaseEnemy::GetIsDreamWorld() const
{
	return _isDreamWorld;
}
