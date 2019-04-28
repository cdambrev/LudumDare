// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseEnemy.h"
#include "LevelDescriptor.h"
#include "Components/WorldStateComponent.h"

#include "EnemiesManager.generated.h"

class UWorldStateComponent;
enum class EWorldState : uint8;

UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class POSSESSTHEBABY_API UEnemiesManager : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UEnemiesManager();

	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

	void SetIsForDream(bool forDream);

	const TArray<ABaseEnemy*>& GetEnemiesOnScreen() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	TSubclassOf<ABaseEnemy> lightMonsterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	TSubclassOf<ABaseEnemy> strongMonsterClass;
	
	UPROPERTY(EditDefaultsOnly, Category= "Properties")
	bool _isForDream = false;

private:

	void SpawnNewEnnemy(bool strong);

	void InitializeWave();

	UWorldStateComponent* GetWorldStateComponent() const;

	UFUNCTION()
	void OnWorldStateChanged(EWorldState worldState);

	void OnEnnemyDied(ABaseEnemy* ennemy);

	void SpawnEnemy(TSubclassOf<ABaseEnemy> enemyClass);

	void InitializeNewWave();

	UPROPERTY(Transient)
	TArray<ABaseEnemy*> _ennemiesOnScreen;

	int32 _maxEnnemiesOnScreen = 0;

	int32 _maxEnnemiesAttacking = 3;

	int32 _currentEnemiesPerWave = 0;
	int32 _currentStrongEnemiesPerWave = 0;
	int32 _currentLightEnemiesPerWave = 0;

	int32 _currentWave = 0;

	int32 _enemiesSpawnedThisWave = 0;
	int32 _currentLightEnemiesSpawned = 0;
	int32 _currentStrongEnemiesSpawned = 0;

	FTimerHandle _spawnLightMonsterHandle;
	FTimerHandle _spawnStrongMonsterHandle;
};
