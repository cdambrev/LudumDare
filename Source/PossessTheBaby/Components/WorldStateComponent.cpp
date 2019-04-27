#include "WorldStateComponent.h"

UWorldStateComponent::UWorldStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWorldStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWorldStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UWorldStateComponent::IsInDreamWorld() const
{
	return State == EWorldState::Dream;
}

bool UWorldStateComponent::IsInNightmareWorld() const
{
	return State == EWorldState::Nightmare;
}

void UWorldStateComponent::ToggleWorldState()
{
	if (IsInDreamWorld())
	{
		State = EWorldState::Nightmare;
	}
	else
	{
		State = EWorldState::Dream;
	}

	OnWorldStateChanged.Broadcast(State);
}

