// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/FakePerspectiveComponent.h"
#include "Components/CombatComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	
	_fakePerspective = CreateDefaultSubobject<UFakePerspectiveComponent>(TEXT("Fake Perspective"));
	_combatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetSprite()->SetSpriteColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));

	_currentHp = maxHp;
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
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

float ABaseEnemy::GetCurrentHp() const
{
	return _currentHp;
}

UCombatComponent* ABaseEnemy::GetCombatComponent() const
{
	return _combatComponent;
}

void ABaseEnemy::SetFacingRight(bool facingRight)
{
	_facingRight = facingRight;
}

bool ABaseEnemy::GetFacingRight() const
{
	return _facingRight;
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
	_currentHp--;
}
