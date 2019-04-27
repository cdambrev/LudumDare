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