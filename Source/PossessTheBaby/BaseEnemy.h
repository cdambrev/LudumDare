// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "BaseEnemy.generated.h"

class UPaperFlipbook;

enum class EEnemyStateMachine : uint8
{
	Frozen,
	Wandering,
	Attacking,
	GettingHit,
	Dead
};

UCLASS()
class POSSESSTHEBABY_API ABaseEnemy : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EEnemyStateMachine getCurrentState() const;

	void SetCurrentState(EEnemyStateMachine nextState);

	void SetAllowedToAttack(bool allowed);

	bool canAttack() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float speed = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float hit = 0.f;

private:

	EEnemyStateMachine _currentState = EEnemyStateMachine::Frozen;

	bool _allowedToAttack = false;
};
