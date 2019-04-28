#include "PossessTheBabyGameState.h"

#include "WaveStateComponent.h"
#include "PossessTheBabyCharacter.h"
#include "Components/WorldStateComponent.h"
#include "Components/WorldLimitsComponent.h"

APossessTheBabyGameState::APossessTheBabyGameState()
	: Super()
{
	WorldState = CreateDefaultSubobject<UWorldStateComponent>(TEXT("WorldState"));

	_dreamWaveState = CreateDefaultSubobject<UWaveStateComponent>(TEXT("DreamWaveState"));
	_nightmareWaveState = CreateDefaultSubobject<UWaveStateComponent>(TEXT("NightmareWaveState"));
	_worldLimits = CreateDefaultSubobject<UWorldLimitsComponent>(TEXT("WorldLimits"));
}

UWorldStateComponent* APossessTheBabyGameState::GetWorldState() const
{
	return WorldState;
}

APossessTheBabyCharacter* APossessTheBabyGameState::GetPlayer() const
{
	return _player;
}

void APossessTheBabyGameState::SetPlayer(APossessTheBabyCharacter* player)
{
	_player = player;
}

UWaveStateComponent* APossessTheBabyGameState::GetDreamWaveState() const
{
	return _dreamWaveState;
}
	
UWaveStateComponent* APossessTheBabyGameState::GetNightmareWaveState() const
{
	return _nightmareWaveState;
}

UWorldLimitsComponent* APossessTheBabyGameState::GetWorldLimits() const
{
	return _worldLimits;
}

FVector APossessTheBabyGameState::GetRandomSpawnPoint() const
{
	FVector spawnPoint;
	UWorldLimitsComponent* worldLimits = GetWorldLimits();
	int32 leftSide = FMath::FRandRange(0, 1.9);
	if (leftSide == 1)
	{
		spawnPoint.X = worldLimits->GetMinX() - 100.f;
	}
	else
	{
		spawnPoint.X = worldLimits->GetMaxX() + 100.f;
	}
	spawnPoint.Z = FMath::FRandRange(worldLimits->GetMinZ(), worldLimits->GetMaxZ());
	return spawnPoint;
}