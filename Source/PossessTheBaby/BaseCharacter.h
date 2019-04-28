// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Components/WorldStateComponent.h"

#include "BaseCharacter.generated.h"

class UTextRenderComponent;
class UFakePerspectiveComponent;
class UCombatComponent;
class UFlickerComponent;
class UAnimationComponent;
class UWorldStateComponent;

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
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintGetter, Category="Combat")
	UCombatComponent* GetCombatComponent() const;
	UFakePerspectiveComponent* GetFakePerspective() const;

	void OnHit(float damage);
	bool GetFacingRight() const;

	void SetFacingRight(bool facingRight);

	void PlayFoleySound();
	void PlayHitSound();
	void PlayFootStep();
	void PlayDieSound();

	void SetMovementEnabled(bool enabled);
	void SetAttackEnabled(bool enabled);
	bool IsAttackEnabled() const;
	void StopMoving();

protected:

	UFlickerComponent* GetFlicker() const;

	UPROPERTY(EditAnywhere, Category=Sounds)
	class USoundBase* FoleySound = nullptr;

	UPROPERTY(EditAnywhere, Category=Sounds)
	class USoundBase* DreamHitSound = nullptr;

	UPROPERTY(EditAnywhere, Category=Sounds)
	class USoundBase* NightmareHitSound = nullptr;

	UPROPERTY(EditAnywhere, Category=Sounds)
	class USoundBase* FootStep = nullptr;

	UPROPERTY(EditAnywhere, Category=Sounds)
	class USoundBase* DieSound = nullptr;

	UPROPERTY(EditAnywhere, Category=Sounds)
	float FootStepPerMeter = 1.0f;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float _attackDuration = 0.1f;

	UWorldStateComponent* GetWorldStateComponent() const;
	EWorldState GetWorldState() const;

private:

	UPROPERTY()
	UFakePerspectiveComponent* _fakePerspective = nullptr;

	UPROPERTY(BlueprintGetter = "GetCombatComponent")
	UCombatComponent* _combat = nullptr;

	UPROPERTY()
	UFlickerComponent* _flicker = nullptr;

	UPROPERTY()
	UAnimationComponent* _animationComponent = nullptr;

	bool _facingRight = true;

	float _footStepTimer = 0.0f;
	bool _attackEnabled = true;
};
