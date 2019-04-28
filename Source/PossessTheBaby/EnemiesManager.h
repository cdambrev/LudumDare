// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseEnemy.h"
#include "LevelDescriptor.h"

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

	void OnWorldStateChanged(EWorldState worldState);

	void OnEnnemyDied(ABaseEnemy* ennemy);

	UPROPERTY(Transient)
	TArray<ABaseEnemy*> _ennemiesOnScreen;

	int32 _maxEnnemiesOnScreen = 1;

	int32 _maxEnnemiesAttacking = 5;

	FTimerHandle _spawnLightMonsterHandle;
	FTimerHandle _spawnStrongMonsterHandle;
};
