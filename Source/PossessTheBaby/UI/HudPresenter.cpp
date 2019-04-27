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
		_onDreamPointsChangedHandle = _health->OnDreamPointsChanged.AddUObject(this, &UHudPresenter::OnDreamPointsChanged);
		OnDreamPointsChanged(_health->GetDreamPoints());

		_onNightmarePointsChangedHandle = _health->OnNightmarePointsChanged.AddUObject(this, &UHudPresenter::OnNightmarePointsChanged);
		OnNightmarePointsChanged(_health->GetNightmarePoints());
	}
}

