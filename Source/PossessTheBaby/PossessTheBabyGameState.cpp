#include "PossessTheBabyGameState.h"

#include "Components/WorldStateComponent.h"
#include "PossessTheBabyCharacter.h"

APossessTheBabyGameState::APossessTheBabyGameState()
	: Super()
{
	WorldState = CreateDefaultSubobject<UWorldStateComponent>(TEXT("WorldState"));
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