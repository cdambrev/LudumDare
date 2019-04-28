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
	
	bool IsDead() const;

	UFUNCTION(BlueprintGetter)
	UHealthComponent* GetHealth() const;

	// Receive a hit.
	void OnHit(float damage);

	bool IsStun() const;

	bool GetWantToAttack() const;

	bool IsGettingHit() const;

	bool CanGetHit() const;

	void SetCanGetHit(bool canGetHit);

	DECLARE_MULTICAST_DELEGATE(FOnDeath)
	FOnDeath OnDeathDelegate;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* HitAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* GettingHitAnimation;

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

	void PlayAnimAttack(bool right);

	void OnDeath();

	void SetWantToAttack(bool attack);
	
	UFUNCTION()
	void OnAttackingEnd();

	void SetGettingHit(bool gettingHit);

	UFUNCTION()
	void OnGettingHitEnded();

	UPROPERTY(BlueprintGetter="GetHealth")
	UHealthComponent* Health = nullptr;

	float _stunDuration = 0.0f;

	bool _isBoundToDeath = false;

	bool _wantToAttack = false;
	bool _attackEnded = true;

	bool _gettingHit = false;
	bool _canGetHit = true;
};
