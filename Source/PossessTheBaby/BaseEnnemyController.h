// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnnemyController.generated.h"

class ABaseEnemy;
enum class EWorldState : uint8;
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
	
private:

	void OnPlayerChangedWorld(EWorldState worldState);

};
