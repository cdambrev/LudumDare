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

	UPaperFlipbookComponent* sprite = GetSprite();
	UPaperFlipbook* currentFlipbook = sprite->GetFlipbook();

	if (IsDead())
	{
		if (currentFlipbook != DieAnimation)
		{
			GetSprite()->SetLooping(false);
			GetSprite()->SetFlipbook(DieAnimation);
			GetSprite()->PlayFromStart();
			GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ABaseEnemy::OnDeadAnimOver);
			GetSprite()->OnFinishedPlaying.AddDynamic(this, &ABaseEnemy::OnDeadAnimOver);
		}
	}
	else if (_currentState == EEnemyStateMachine::Frozen)
	{
		if (currentFlipbook != FrozenAnimation)
		{
			GetSprite()->SetLooping(false);
			GetSprite()->SetFlipbook(FrozenAnimation);
			GetSprite()->PlayFromStart();
		}
	}
	else if (playAppear)
	{
		if (currentFlipbook != AppearAnimation)
		{
			GetSprite()->SetLooping(false);
			GetSprite()->SetFlipbook(AppearAnimation);
			GetSprite()->PlayFromStart();
			GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ABaseEnemy::OnAppearEnd);
			GetSprite()->OnFinishedPlaying.AddDynamic(this, &ABaseEnemy::OnAppearEnd);
		}
	}
	else if (GetWantToAttack() || !_attackEnd)
	{
		if (_attackEnd)
		{
			GetSprite()->SetLooping(false);
			GetSprite()->SetFlipbook(HitAnimation);
			GetSprite()->PlayFromStart();
			SetWantToAttack(false);
			_wantToAttack = false;
			_attackEnd = false;
			GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ABaseEnemy::OnAttackEnd);
			GetSprite()->OnFinishedPlaying.AddDynamic(this, &ABaseEnemy::OnAttackEnd);
		}
	}
	else
	{
		// Are we moving or standing still?
		GetSprite()->SetLooping(true);
		UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
		if (GetSprite()->GetFlipbook() != DesiredAnimation)
		{
			GetSprite()->SetFlipbook(DesiredAnimation);
			GetSprite()->PlayFromStart();
		}
	}
}

bool ABaseEnemy::canAttack() const
{
	return _allowedToAttack;
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

void ABaseEnemy::OnAppearEnd()
{
	GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ABaseEnemy::OnAppearEnd);
	playAppear = false;
}
	
void ABaseEnemy::OnDeadAnimOver()
{
	GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ABaseEnemy::OnDeadAnimOver);
}

void ABaseEnemy::OnAttackEnd()
{
	GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &ABaseEnemy::OnAttackEnd);
	_attackEnd = true;
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
