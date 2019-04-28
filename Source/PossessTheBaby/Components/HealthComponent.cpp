#include "HealthComponent.h"

#include "Components/WorldStateComponent.h"
#include "PossessTheBabyGameState.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	DreamPoints = _initialDreamPoints;
	NightmarePoints = _initialNightmarePoints;

	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	if (ensure(IsValid(gameState)))
	{
		_worldState = gameState->GetWorldState();
		SetComponentTickEnabled(true);
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ApplyDamage(DeltaTime * _pointsDepletionRate);
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

float UHealthComponent::GetDreamPointsPercent() const
{
	return GetDreamPoints() /_maxPoints;
}

void UHealthComponent::SetDreamPoints(float value)
{
	DreamPoints = FMath::Max(0.0f, value);
	OnDreamPointsChanged.Broadcast(GetDreamPoints());
	OnDreamPointsPercentChanged.Broadcast(GetDreamPointsPercent());

	UpdateDeath();
}

float UHealthComponent::GetNightmarePoints() const
{
	return NightmarePoints;
}

float UHealthComponent::GetNightmarePointsPercent() const
{
	return GetNightmarePoints() / _maxPoints;
}

void UHealthComponent::SetNightmarePoints(float value)
{
	NightmarePoints = FMath::Max(0.0f, value);
	OnNightmarePointsChanged.Broadcast(GetNightmarePoints());
	OnNightmarePointsPercentChanged.Broadcast(GetNightmarePointsPercent());

	UpdateDeath();
}

void UHealthComponent::UpdateDeath()
{
	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}
