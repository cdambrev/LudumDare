// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelDescriptor.h"
#include "Components/WorldStateComponent.h"

#include "PossessTheBabyGameMode.generated.h"

class UEnemiesManager;

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

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "LevelProperties")
	FLevelData levelData;

	UPROPERTY(VisibleAnywhere, Category = "LevelProperties")
	UEnemiesManager* _dreamWorldEnnemiesManager = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "LevelProperties")
	UEnemiesManager* _nightmareEnnemiesManager = nullptr;
};
