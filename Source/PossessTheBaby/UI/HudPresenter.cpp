#include "HudPresenter.h"

#include "Components/HealthComponent.h"

void UHudPresenter::Initialize(UHealthComponent* health)
{
	if (IsValid(_health))
	{
		_health->OnDreamPointsChanged.Remove(_onDreamPointsChangedHandle);
		_onDreamPointsChangedHandle.Reset();
		_health->OnNightmarePointsChanged.Remove(_onNightmarePointsChangedHandle);
		_onNightmarePointsChangedHandle.Reset();
	}
	_character = character;
	if (IsValid(_character))
	{
		_onDreamPointsChangedHandle = _health->OnDreamPointsChanged.AddUObject(this, &UHudPresenter::OnDreamPointsChanged);
		OnDreamPointsChanged(_health->GetDreamPoints());

		_onNightmarePointsChangedHandle = _character->OnNightmarePointsChanged.AddUObject(this, &UHudPresenter::OnNightmarePointsChanged);
		OnNightmarePointsChanged(_character->GetNightmarePoints());
	}
}

