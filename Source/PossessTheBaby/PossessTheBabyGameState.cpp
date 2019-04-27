#include "PossessTheBabyGameState.h"

#include "Components/WorldStateComponent.h"
#include "Components/WorldLimitsComponent.h"

APossessTheBabyGameState::APossessTheBabyGameState()
	: Super()
{
	WorldState = CreateDefaultSubobject<UWorldStateComponent>(TEXT("WorldState"));
	_worldLimits = CreateDefaultSubobject<UWorldLimitsComponent>(TEXT("WorldLimits"));
}

UWorldStateComponent* APossessTheBabyGameState::GetWorldState() const
{
	return WorldState;
}

UWorldLimitsComponent* APossessTheBabyGameState::GetWorldLimits() const
{
	return _worldLimits;
}