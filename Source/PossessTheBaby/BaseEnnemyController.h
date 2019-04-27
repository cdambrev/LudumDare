// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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
	
private:

	void OnPlayerChangedWorld();

};
