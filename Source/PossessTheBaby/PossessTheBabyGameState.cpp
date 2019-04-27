#include "PossessTheBabyGameState.h"

#include "WorldStateComponent.h"
#include "WaveStateComponent.h"
#include "PossessTheBabyCharacter.h"

APossessTheBabyGameState::APossessTheBabyGameState()
	: Super()
{
	WorldState = CreateDefaultSubobject<UWorldStateComponent>(TEXT("WorldState"));

	_dreamWaveState = CreateDefaultSubobject<UWaveStateComponent>(TEXT("DreamWaveState"));
	_nightmareWaveState = CreateDefaultSubobject<UWaveStateComponent>(TEXT("NightmareWaveState"));
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