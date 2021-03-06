// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PossessTheBabyCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WorldStateComponent.h"
#include "PossessTheBabyGameState.h"
#include "Components/FakePerspectiveComponent.h"
#include "Components/CombatComponent.h"
#include "Components/FlickerComponent.h"
#include "Components/AnimationComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// APossessTheBabyCharacter

APossessTheBabyCharacter::APossessTheBabyCharacter()
	: Super()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	/*
	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	*/

	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	//GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void APossessTheBabyCharacter::BeginPlay()
{
	Super::BeginPlay();

	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	if (IsValid(gameState))
	{
		gameState->SetPlayer(this);
	}

	GetSprite()->SetSpriteColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));

	Health->OnDeath.AddUObject(this, &APossessTheBabyCharacter::OnDeath);
}

void APossessTheBabyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	_stunDuration -= DeltaSeconds;

	UpdateCharacter();
}

void APossessTheBabyCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	if (IsDead())
	{
		if (!_isBoundToDeath)
		{
			GetSprite()->SetLooping(false);
			GetSprite()->SetFlipbook(DieAnimation);
			GetSprite()->PlayFromStart();
			GetSprite()->OnFinishedPlaying.AddDynamic(this, &APossessTheBabyCharacter::OnAnimationEnded);
			_isBoundToDeath = true;
		}
	}
	else if (IsGettingHit())
	{
		if (GetSprite()->GetFlipbook() != GettingHitAnimation)
		{
			GetSprite()->SetLooping(false);
			GetSprite()->SetFlipbook(GettingHitAnimation);
			GetSprite()->PlayFromStart();
			GetSprite()->OnFinishedPlaying.AddDynamic(this, &APossessTheBabyCharacter::OnGettingHitEnded);
		}
	}
	else if (GetWantToAttack() || !_attackEnded)
	{
		if (_attackEnded)
		{
			GetSprite()->SetLooping(false);
			GetSprite()->SetFlipbook(HitAnimation);
			GetSprite()->PlayFromStart();
			SetWantToAttack(false);
			_attackEnded = false;
			GetSprite()->OnFinishedPlaying.AddDynamic(this, &APossessTheBabyCharacter::OnAttackingEnd);
		}
	}
	else
	{
		UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
		if (GetSprite()->GetFlipbook() != DesiredAnimation)
		{
			GetSprite()->SetLooping(true);
			GetSprite()->SetFlipbook(DesiredAnimation);
			GetSprite()->PlayFromStart();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APossessTheBabyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("ToggleWorldState", IE_Pressed, this, &APossessTheBabyCharacter::ToggleWorldState);
	PlayerInputComponent->BindAction("AttackLeft", IE_Pressed, this, &APossessTheBabyCharacter::AttackLeft);
	PlayerInputComponent->BindAction("AttackRight", IE_Pressed, this, &APossessTheBabyCharacter::AttackRight);

	PlayerInputComponent->BindAxis("MoveRight", this, &APossessTheBabyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &APossessTheBabyCharacter::MoveUp);
}

void APossessTheBabyCharacter::MoveRight(float Value)
{
	if (!IsDead())
	{
		// Apply the input to the character motion
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
		if (FMath::Abs(Value) > 0.1f)
		{
			SetFacingRight(Value > 0);
		}
	}
}

void APossessTheBabyCharacter::MoveUp(float Value)
{
	if (!IsDead())
	{
		// Apply the input to the character motion
		AddMovementInput(FVector(0.0f, 0.0f, 1.0f), Value);
	}
}

void APossessTheBabyCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();
}

UHealthComponent* APossessTheBabyCharacter::GetHealth() const
{
	return Health;
}

void APossessTheBabyCharacter::ToggleWorldState()
{
	bool canToogle = false;
	if (!IsDead())
	{
		if (GetWorldState() == EWorldState::Dream)
		{
			canToogle = GetHealth()->GetNightmarePoints() > 0;
		}
		else
		{
			canToogle = GetHealth()->GetDreamPoints() > 0;
		}
	}

	if (canToogle)
	{
		GetWorldStateComponent()->ToggleWorldState();
		UGameplayStatics::PlaySound2D(GetWorld(), WorldSwitchToggleDeniedSound);
	}
}

bool APossessTheBabyCharacter::IsStun() const
{
	return _stunDuration >= 0.0f;
}

void APossessTheBabyCharacter::OnHit(float damage)
{
	if (CanGetHit() && !IsGettingHit())
	{
		GetHealth()->ApplyDamage(damage);
		GetFlicker()->TintFlick(0.2f, FColor::Red);
		SetGettingHit(true);
		SetCanGetHit(false);
		SetMovementEnabled(false);
		_stunDuration = 0.5f;
	}
}

void APossessTheBabyCharacter::SetGettingHit(bool gettingHit)
{
	_gettingHit = gettingHit;
}

bool APossessTheBabyCharacter::IsGettingHit() const
{
	return _gettingHit;
}

void APossessTheBabyCharacter::AttackLeft()
{
	if (IsAttackEnabled())
	{
		Attack();
	}
}

void APossessTheBabyCharacter::AttackRight()
{
	if (IsAttackEnabled())
	{
		Attack();
	}
}

void APossessTheBabyCharacter::Attack()
{
	// jouer animation et stopper le player pour le temps de l'anim
	GetCharacterMovement()->StopActiveMovement();
	SetWantToAttack(true);
	SetAttackEnabled(false);

	// tenter de toucher qqchose
	ABaseEnemy* ennemy = GetCombatComponent()->TestAttackEnemy();
	if (ennemy != nullptr)
	{
		GetCombatComponent()->AttackEnemy(ennemy);
		PlayHitSound();
	}

	PlayFoleySound();
}

void APossessTheBabyCharacter::OnAnimationEnded()
{
	GetSprite()->Stop();
}

void APossessTheBabyCharacter::OnAttackingEnd()
{
	OnAnimationEnded();
	GetSprite()->OnFinishedPlaying.RemoveDynamic(this, &APossessTheBabyCharacter::OnAttackingEnd);
	_wantToAttack = false;
	_attackEnded = true;
	SetAttackEnabled(true);
}

void APossessTheBabyCharacter::OnDeath()
{
	PlayDieSound();
	StopMoving();
	SetMovementEnabled(false);
	OnDeathDelegate.Broadcast();
}

bool APossessTheBabyCharacter::IsDead() const
{
	return Health->IsDead();
}

void APossessTheBabyCharacter::SetWantToAttack(bool attack)
{
	_wantToAttack = attack;
}

bool APossessTheBabyCharacter::GetWantToAttack() const
{
	return _wantToAttack;
}

void APossessTheBabyCharacter::OnGettingHitEnded()
{
	SetCanGetHit(true);
	SetGettingHit(false);
	GetSprite()->OnFinishedPlaying.RemoveDynamic(this,&APossessTheBabyCharacter::OnGettingHitEnded);
	SetMovementEnabled(true);
}

bool APossessTheBabyCharacter::CanGetHit() const
{
	return _canGetHit;
}

void APossessTheBabyCharacter::SetCanGetHit(bool canGetHit)
{
	_canGetHit = canGetHit;
}
