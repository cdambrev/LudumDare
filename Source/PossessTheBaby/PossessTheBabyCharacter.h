// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PossessTheBabyCharacter.generated.h"

class UTextRenderComponent;
class UWorldStateComponent;
class UHealthComponent;

/**
 * This class is the default character for PossessTheBaby, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class APossessTheBabyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	APossessTheBabyCharacter();

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintGetter)
	UHealthComponent* GetHealth() const;

	// Receive a hit.
	void OnHit(float damage);

	bool IsStun() const;

protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DieAnimation;

	UPROPERTY(EditAnywhere, Category=Sounds)
	class USoundBase* WorldSwitchToggleDeniedSound = nullptr;
	
	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	/** Called for side to side input */
	void MoveRight(float Value);
	void MoveUp(float Value);

	void UpdateCharacter();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	UFUNCTION()
	void OnAnimationEnded();
	void ToggleWorldState();

	void Attack();
	void AttackLeft();
	void AttackRight();

	void SetMovementEnabled(bool enabled);

	void PlayAnimAttack(bool right);

	void OnDeath();

	UPROPERTY(BlueprintGetter="GetHealth")
	UHealthComponent* Health = nullptr;

	float _stunDuration = 0.0f;
};
