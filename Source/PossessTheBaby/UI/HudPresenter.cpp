#include "HudPresenter.h"

#include "Components/HealthComponent.h"
#include "Components/CombatComponent.h"

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
		_worldState->OnWorldStateChanged.RemoveDynamic(this, &UHudPresenter::OnWorldStateChanged);
		_onDreamPointsChangedHandle.Reset();
	}
	_worldState = worldState;
	if (IsValid(_worldState))
	{
		_worldState->OnWorldStateChanged.AddDynamic(this, &UHudPresenter::OnWorldStateChanged);
		OnWorldStateChanged(_worldState->GetWorldState());
	}
}

void UHudPresenter::SetCombatComponent(UCombatComponent* combat)
{
	if (IsValid(_combat))
	{
		_combat->OnSuccessfulHit.Remove(_onCombatChangedHandle);
		_onCombatChangedHandle.Reset();
	}
	_combat = _combat;
	if (IsValid(_combat))
	{
		_combat->OnSuccessfulHit.AddUObject(this, &UHudPresenter::OnSuccessfulHitChanged);
		OnSuccessfulHitChanged(_combat->GetSuccessfulHit());
	}
}

