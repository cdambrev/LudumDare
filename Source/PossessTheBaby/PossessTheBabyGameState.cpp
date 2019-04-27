#include "PossessTheBabyGameState.h"

#include "Components/WorldStateComponent.h"

APossessTheBabyGameState::APossessTheBabyGameState()
	: Super()
{
	WorldState = CreateDefaultSubobject<UWorldStateComponent>(TEXT("WorldState"));
}

UWorldStateComponent* APossessTheBabyGameState::GetWorldState() const
{
	return WorldState;
}