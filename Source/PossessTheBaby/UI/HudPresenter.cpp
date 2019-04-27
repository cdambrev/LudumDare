#include "HudPresenter.h"

#include "Components/HealthComponent.h"

void UHudPresenter::SetHealthComponent(UHealthComponent* health)
{
	if (IsValid(_health))
	{
		_health->OnDreamPointsChanged.Remove(_onDreamPointsChangedHandle);
		_onDreamPointsChangedHandle.Reset();
		_health->OnNightmarePointsChanged.Remove(_onNightmarePointsChangedHandle);
		_onNightmarePointsChangedHandle.Reset();
	}
	_health = health;
	if (IsValid(_health))
	{
		_onDreamPointsChangedHandle = _health->OnDreamPointsPercentChanged.AddUObject(this, &UHudPresenter::OnDreamPointsChanged);
		OnDreamPointsChanged(_health->GetDreamPointsPercent());

		_onNightmarePointsChangedHandle = _health->OnNightmarePointsPercentChanged.AddUObject(this, &UHudPresenter::OnNightmarePointsChanged);
		OnNightmarePointsChanged(_health->GetNightmarePointsPercent());
	}
}

void UHudPresenter::SetWorldStateComponent(UWorldStateComponent* worldState)
{
	if (IsValid(_worldState))
	{
		_worldState->OnWorldStateChanged.Remove(_onWorldStateChangedHandle);
		_onDreamPointsChangedHandle.Reset();
	}
	_worldState = worldState;
	if (IsValid(_worldState))
	{
		_onWorldStateChangedHandle = _worldState->OnWorldStateChanged.AddUObject(this, &UHudPresenter::OnWorldStateChanged);
		OnWorldStateChanged(_worldState->GetWorldState());
	}
}

