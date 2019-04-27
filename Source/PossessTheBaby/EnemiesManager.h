// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.h"
#include "LevelDescriptor.h"

#include "EnemiesManager.generated.h"

UCLASS()
class POSSESSTHEBABY_API AEnemiesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemiesManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLevelData levelDescriptor;

private:

	void SpawnNewEnnemy();

	TArray<ABaseEnemy> _ennemiesOnScreen;

	int32 currentWave = 0;

	int32 maxEnnemiesOnScreen = 0;

	int32 maxEnnemiesAttacking = 3;

	int32 currentEnnemiesAttacking = 0;
};
