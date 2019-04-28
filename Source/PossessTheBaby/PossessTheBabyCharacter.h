// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PossessTheBabyCharacter.generated.h"

class UTextRenderComponent;
class UWorldStateComponent;
class UHealthComponent;
class UFakePerspectiveComponent;
class UCombatComponent;
class UFlickerComponent;

/**
 * This class is the default character for PossessTheBaby, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class APossessTheBabyCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintGetter)
	UHealthComponent* GetHealth() const;

	// Receive a hit.
	void OnHit(float damage);

	bool IsStun() const;

	bool GetFacingRight() const;

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

	UWorldStateComponent* GetWorldState() const;

	void AttackLeft();

	void AttackRight();

	void SetMovementEnabled(bool enabled);

	void PlayAnimAttack(bool right);

	UPROPERTY(BlueprintGetter="GetHealth")
	UHealthComponent* Health = nullptr;

	UPROPERTY()
	UFakePerspectiveComponent* _fakePerspective = nullptr;

	UPROPERTY()
	UCombatComponent* _combat = nullptr;

	bool _facingRight = true;

	UPROPERTY()
	UFlickerComponent* _flicker = nullptr;

	float _stunDuration = 0.0f;

public:
	APossessTheBabyCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UCombatComponent* GetCombatComponent() const;
};
