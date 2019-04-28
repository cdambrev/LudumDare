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
	_fakePerspective = CreateDefaultSubobject<UFakePerspectiveComponent>(TEXT("Fake Perspective"));
	_combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	_flicker = CreateDefaultSubobject<UFlickerComponent>(TEXT("Flicker Component"));
	_flicker->SetSprite(GetSprite());

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void APossessTheBabyCharacter::UpdateAnimation()
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

void APossessTheBabyCharacter::BeginPlay()
{
	Super::BeginPlay();

	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	if (IsValid(gameState))
	{
		gameState->SetPlayer(this);
	}

	GetSprite()->SetSpriteColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}

void APossessTheBabyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();
}

//////////////////////////////////////////////////////////////////////////
// Input

void APossessTheBabyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("ToggleWorldState", IE_Pressed, this, &APossessTheBabyCharacter::ToggleWorldState);

	PlayerInputComponent->BindAxis("MoveRight", this, &APossessTheBabyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &APossessTheBabyCharacter::MoveUp);
	PlayerInputComponent->BindAxis("AttackLeft", this, &APossessTheBabyGameState::AttackLeft);
	PlayerInputComponent->BindAxis("AttackRight", this, &APossessTheBabyGameState::AttackRight);
}

void APossessTheBabyCharacter::MoveRight(float Value)
{
	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	_facingRight = Value > 0;
}

void APossessTheBabyCharacter::MoveUp(float Value)
{
	// Apply the input to the character motion
	AddMovementInput(FVector(0.0f, 0.0f, 1.0f), Value);
}

void APossessTheBabyCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling.
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

UWorldStateComponent* APossessTheBabyCharacter::GetWorldState() const
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return gameState->GetWorldState();
}

UHealthComponent* APossessTheBabyCharacter::GetHealth() const
{
	return Health;
}

void APossessTheBabyCharacter::ToggleWorldState()
{
	GetWorldState()->ToggleWorldState();

	_flicker->Flick(0.2f, FLinearColor(1.0f, 0.0f, 0.0f));
}

void APossessTheBabyCharacter::AttackLeft()
{
	// jouer animation et stopper le player pour le temps de l'anim
	GetCharacterMovement()->StopActiveMovement();
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	PlayAnimAttack(false);
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &APossessTheBabyCharacter::SetMovementEnabled, true);
	GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 1.f, false);
	
	// tenter de toucher qqchose
	ABaseEnemy* ennemy = _combat->TestAttackEnemy();
	_combat->AttackEnemy(ennemy);
}

void APossessTheBabyCharacter::AttackRight()
{
	// jouer animation et stopper le player pour le temps de l'anim
	GetCharacterMovement()->StopActiveMovement();
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	PlayAnimAttack(true);
	FTimerHandle timerHandle;
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &APossessTheBabyCharacter::SetMovementEnabled, true);
	GetWorldTimerManager().SetTimer(timerHandle, timerDelegate, 1.f, false);
	// jouer animation toujours
	// tenter de toucher qqchose
}

void APossessTheBabyCharacter::SetMovementEnabled(bool enabled)
{
	if (enabled)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}

void APossessTheBabyCharacter::PlayAnimAttack(bool right)
{
	if (right)
	{

	}
	else
	{

	}
}

UCombatComponent* APossessTheBabyCharacter::GetCombatComponent() const
{
	return _combat;
}
