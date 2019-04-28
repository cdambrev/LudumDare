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