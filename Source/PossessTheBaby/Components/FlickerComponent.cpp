#include "FlickerComponent.h"

#include "PaperFlipbookComponent.h"

UFlickerComponent::UFlickerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UFlickerComponent::SetSprite(UPaperFlipbookComponent* sprite)
{
	_sprite = sprite;
}

void UFlickerComponent::TickComponent(float deltaSeconds, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaSeconds, tickType, thisTickFunction);

	if (_duration <= 0.0f)
	{
		_color.A = 1.0f;
	}
	else
	{
		_color.A += deltaSeconds / _duration;
	}

	if (_color.A >= 1.0f)
	{
		SetComponentTickEnabled(false);
		_color.A = 0.0f;
	}

	_sprite->SetSpriteColor(_color);
}

void UFlickerComponent::TintFlick(float duration, FLinearColor color)
{
	SetComponentTickEnabled(true);
	_duration = duration;
	_timeLeft = duration;
	_color = color;
	_color.A = 0.0f;
	_type = FlickType::Tint;
}

void UFlickerComponent::Flick(float rate, float duration)
{
	SetComponentTickEnabled(true);
	_duration = duration;
	_timeLeft = duration;
	_color.A = 0.0f;
	_type = FlickType::Flicker;
}

