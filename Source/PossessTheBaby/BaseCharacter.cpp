// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BaseCharacter.h"

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

ABaseCharacter::ABaseCharacter()
	: Super()
{
	_fakePerspective = CreateDefaultSubobject<UFakePerspectiveComponent>(TEXT("Fake Perspective"));
	_combat = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	_flicker = CreateDefaultSubobject<UFlickerComponent>(TEXT("Flicker Component"));
	_flicker->SetSprite(GetSprite());
	_animationComponent = CreateDefaultSubobject<UAnimationComponent>(TEXT("Animation Component"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetSprite()->SetSpriteColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	float speed = PlayerVelocity.Size();
	_footStepTimer += DeltaTime * (speed / 100.0f);
	if (_footStepTimer > FootStepPerMeter)
	{
		PlayFootStep();
		_footStepTimer = 0.0f;
	}
}

bool ABaseCharacter::GetFacingRight() const
{
	return _facingRight;
}

void ABaseCharacter::SetFacingRight(bool facingRight)
{
	_facingRight = facingRight;
}

UCombatComponent* ABaseCharacter::GetCombatComponent() const
{
	return _combat;
}

UFlickerComponent* ABaseCharacter::GetFlicker() const
{
	return _flicker;
}

UFakePerspectiveComponent* ABaseCharacter::GetFakePerspective() const
{
	return _fakePerspective;
}

UWorldStateComponent* ABaseCharacter::GetWorldStateComponent() const
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return gameState->GetWorldState();
}

EWorldState ABaseCharacter::GetWorldState() const
{
	return GetWorldStateComponent()->GetWorldState();
}

void ABaseCharacter::PlayFoleySound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), FoleySound);
}

void ABaseCharacter::PlayHitSound()
{
	if (GetWorldState() == EWorldState::Dream)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DreamHitSound);
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), NightmareHitSound);
	}
}

void ABaseCharacter::PlayFootStep()
{
	UGameplayStatics::PlaySound2D(GetWorld(), FootStep);
}

void ABaseCharacter::PlayDieSound()
{
	UGameplayStatics::PlaySound2D(GetWorld(), DieSound);
}

bool ABaseCharacter::IsAttackEnabled() const
{
	return _attackEnabled;
}

void ABaseCharacter::SetAttackEnabled(bool enabled)
{
	_attackEnabled = enabled;
	SetMovementEnabled(enabled);
}

void ABaseCharacter::SetMovementEnabled(bool enabled)
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

void ABaseCharacter::StopMoving()
{
	GetCharacterMovement()->StopActiveMovement();
	ConsumeMovementInputVector();
}