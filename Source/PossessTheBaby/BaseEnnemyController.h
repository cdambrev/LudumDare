// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnnemyController.generated.h"

/**
 * 
 */
UCLASS()
class POSSESSTHEBABY_API ABaseEnnemyController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseEnnemyController();
	
	virtual void Tick(float DeltaTime) override;
	
private:

	void OnPlayerChangedWorld();

};
