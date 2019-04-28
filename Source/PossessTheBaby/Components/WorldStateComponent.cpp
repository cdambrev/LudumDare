#include "WorldStateComponent.h"

#include "Engine/Engine.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

UWorldStateComponent::UWorldStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWorldStateComponent::BeginPlay()
{
	Super::BeginPlay();

	_dreamAmbienceInstance = UGameplayStatics::SpawnSound2D(GetWorld(), AmbienceDream);
	_dreamAmbienceInstance->Play();

	_nightmareAmbienceInstance = UGameplayStatics::SpawnSound2D(GetWorld(), AmbienceNightmare);
	_nightmareAmbienceInstance->Stop();
}

void UWorldStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsInDreamWorld())
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, TEXT("Dream World"));

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, TEXT("Nightmare World"));
	}
}

EWorldState UWorldStateComponent::GetWorldState() const
{
	return State;
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
		_dreamAmbienceInstance->FadeOut(1.0f, 0.0f);
		_nightmareAmbienceInstance->FadeIn(1.0f);
	}
	else
	{
		State = EWorldState::Dream;
		_nightmareAmbienceInstance->FadeOut(1.0f, 0.0f);
		_dreamAmbienceInstance->FadeIn(1.0f);
	}

	UGameplayStatics::PlaySound2D(GetWorld(), WorldSwitch);
	OnWorldStateChanged.Broadcast(State);
}

