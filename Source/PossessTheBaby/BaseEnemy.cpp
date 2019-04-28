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

	_attackDuration = 1.0f;
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
		GetSprite()->SetLooping(false);
	}
	else if (_currentState == EEnemyStateMachine::Frozen)
	{
		GetSprite()->SetFlipbook(FrozenAnimation);
		GetSprite()->SetLooping(false);
	}
	else if (playAppear)
	{
		GetSprite()->ReverseFromEnd();
		GetSprite()->SetLooping(false);
		playAppear = false;
	}
	else if (GetWantToAttack())
	{
		GetSprite()->SetFlipbook(HitAnimation);
		GetSprite()->SetLooping(false);
		SetWantToAttack(false);
	}
	else
	{
		// Are we moving or standing still?
		UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
		if (GetSprite()->GetFlipbook() != DesiredAnimation)
		{
			GetSprite()->SetFlipbook(DesiredAnimation);
			GetSprite()->SetLooping(true);
		}
	}
}

bool ABaseEnemy::canAttack() const
{
	return _allowedToAttack || GetCombatComponent()->TestAttackHero();
}

bool ABaseEnemy::CanMoveCloseToHero() const
{
	return _canMoveCloseToHero;
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

void ABaseEnemy::SetWantToAttack(bool wantToAttack)
{
	_wantToAttack = wantToAttack;
}

bool ABaseEnemy::GetWantToAttack() const
{
	return _wantToAttack;
}

void ABaseEnemy::Attack()
{
	SetWantToAttack(true);

	GetCharacterMovement()->StopActiveMovement();
	SetAttackEnabled(false);

	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &ABaseEnemy::SetAttackEnabled, true);
	GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, _attackDuration, false);

	GetCombatComponent()->AttackHero();
	PlayHitSound();

	PlayFoleySound();
}
