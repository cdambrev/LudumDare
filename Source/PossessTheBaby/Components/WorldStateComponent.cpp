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

void UWorldStateComponent::ToggleWorldState()
{
	if (State == EWorldState::Dream)
	{
		State = EWorldState::Nightmare;
	}
	else
	{
		State = EWorldState::Dream;
	}

	OnWorldStateChanged.Broadcast(State);
}

