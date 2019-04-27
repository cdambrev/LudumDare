#include "HealthComponent.h"

#include "Components/WorldStateComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::ApplyDamage(float value)
{
	if (_worldState->IsInDreamWorld())
	{
		SetDreamPoints(GetDreamPoints() - value);
	}
	else
	{
		SetNightmarePoints(GetNightmarePoints() - value);
	}
}

bool UHealthComponent::IsDead() const
{
	return DreamPoints <= 0.0f && NightmarePoints <= 0.0f;
}

float UHealthComponent::GetDreamPoints() const
{
	return DreamPoints;
}

void UHealthComponent::SetDreamPoints(float value)
{
	DreamPoints = FMath::Max(0.0f, value);
	OnDreamPointsChanged.Broadcast(DreamPoints);
}

float UHealthComponent::GetNightmarePoints() const
{
	return NightmarePoints;
}

void UHealthComponent::SetNightmarePoints(float value)
{
	NightmarePoints = FMath::Max(0.0f, value);
	OnNightmarePointsChanged.Broadcast(NightmarePoints);
}
