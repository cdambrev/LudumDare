// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/WorldStateComponent.h"
#include "BaseEnnemyController.generated.h"

class ABaseEnemy;
/**
 * 
 */
UCLASS()
class POSSESSTHEBABY_API ABaseEnnemyController : public AAIController
{
	GENERATED_BODY()

public:
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnnemyDied, ABaseEnemy*)
	FOnEnnemyDied OnEnnemyDied;
	
	ABaseEnnemyController();
	
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override final;

protected:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float AttackMinWait = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Settings")
	float AttackMaxWait = 1.5f;
	
private:
	ABaseEnemy* GetEnemyPawn() const;

	UFUNCTION()
	void OnPlayerChangedWorld(EWorldState worldState);

	void State_Wander();
	void State_WaitForAttackingEnter();
	void State_WaitForAttacking();
	void State_DieEnter();
	void State_Die();
	void State_Dead();

	FTimerHandle _waitingForActionTimer;
};
