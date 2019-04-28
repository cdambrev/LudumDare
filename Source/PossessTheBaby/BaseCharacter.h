// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"

#include "BaseCharacter.generated.h"

class UTextRenderComponent;
class UFakePerspectiveComponent;
class UCombatComponent;
class UFlickerComponent;
class UAnimationComponent;

UENUM(BlueprintType)
enum class EAnimState : uint8
{
	Idle,
	Running, 
	Punching,
	Hit,
	Death
};

/**
 * This class is the default character for PossessTheBaby, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class ABaseCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter();

	virtual void BeginPlay() override;

	UCombatComponent* GetCombatComponent() const;
	UFakePerspectiveComponent* GetFakePerspective() const;

	void OnHit(float damage);
	bool GetFacingRight() const;

	void SetFacingRight(bool facingRight);

protected:

	UFlickerComponent* GetFlicker() const;

private:
	UPROPERTY()
	UFakePerspectiveComponent* _fakePerspective = nullptr;

	UPROPERTY()
	UCombatComponent* _combat = nullptr;

	UPROPERTY()
	UFlickerComponent* _flicker = nullptr;

	UPROPERTY()
	UAnimationComponent* _animationComponent = nullptr;

	bool _facingRight = true;
};