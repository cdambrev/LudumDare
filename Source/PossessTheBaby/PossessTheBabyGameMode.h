// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelDescriptor.h"
#include "Components/WorldStateComponent.h"

#include "PossessTheBabyGameMode.generated.h"

class AEnemiesManager;

/**
 * The GameMode defines the game being played. It governs the game rules, scoring, what actors
 * are allowed to exist in this game type, and who may enter the game.
 *
 * This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of PossessTheBabyCharacter
 */
UCLASS(minimalapi)
class APossessTheBabyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	APossessTheBabyGameMode();

	virtual void BeginPlay() override;

	FLevelData GetLevelData() const;

	AEnemiesManager* GetEnemyManager(EWorldState worldState) const;
	AEnemiesManager* GetEnemyManager() const;

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "LevelProperties")
	FLevelData levelData;
	
	UPROPERTY(EditDefaultsOnly, Category = "LevelProperties")
	TSubclassOf<AEnemiesManager> enemiesManagerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "LevelProperties")
	AEnemiesManager* _dreamWorldEnnemiesManager = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "LevelProperties")
	AEnemiesManager* _nightmareEnnemiesManager = nullptr;

private:
};
