// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.h"
#include "LevelDescriptor.h"

#include "EnemiesManager.generated.h"

class UWorldStateComponent;
enum class EWorldState : uint8;

UCLASS()
class POSSESSTHEBABY_API AEnemiesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemiesManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIsForDream(bool forDream);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	TSubclassOf<ABaseEnemy> lightMonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	TSubclassOf<ABaseEnemy> strongMonsterClass;

private:

	void SpawnNewEnnemy(bool strong);

	void InitializeWave();

	UWorldStateComponent* GetWorldStateComponent() const;

	void OnWorldStateChanged(EWorldState worldState);

	TArray<ABaseEnemy*> _ennemiesOnScreen;

	int32 _maxEnnemiesOnScreen = 10;

	int32 _maxEnnemiesAttacking = 5;

	int32 _currentEnnemiesAttacking = 0;

	FTimerHandle _spawnLightMonsterHandle;
	FTimerHandle _spawnStrongMonsterHandle;

	bool _isForDream = false;
};
