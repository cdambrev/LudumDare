// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseEnemy.generated.h"

class UPaperFlipbook;
class UFakePerspectiveComponent;
class UCombatComponent;

enum class EEnemyStateMachine : uint8
{
	Frozen,
	Wandering,
	MovingToPlayer,
	Attacking,
	GettingHit,
	Dead,
	Spawning
};

UCLASS()
class POSSESSTHEBABY_API ABaseEnemy : public ABaseCharacter
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

	float GetCurrentHp() const;

	bool IsDead() const;

	float GetHitPoints() const;

	float GetSoulsRewarded() const;

	void ApplyDamage();

	void SetIsDreamWorld(bool isDreamWorld);

	bool GetIsDreamWorld() const;

	UFakePerspectiveComponent* getFakePerspective() const;

	void SetWantToAttack(bool wantToAttack);

	bool GetWantToAttack() const;

	bool playAppear = false;

	bool alreadyPlayedAnim = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UPaperFlipbook* IdleAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DieAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* HitAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* FrozenAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float speed = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float hit = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float maxHp = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float soulsReward = 0.f;
	
private:

	EEnemyStateMachine _currentState = EEnemyStateMachine::Spawning;

	bool _allowedToAttack = false;
	
	float _currentHp = 0;
	
	bool _isDreamWorld = false;

	bool _wantToAttack = false;
};
