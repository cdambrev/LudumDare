#include "HealthComponent.h"

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

bool UHealthComponent::IsDead() const
{
	return DreamPoints <= 0.0f && NightmarePoints <= 0.0f;
}

float UHealthComponent::GetDreamPoints() const
{
	return DreamPoints;
}

float UHealthComponent::GetNightmarePoints() const
{
	return NightmarePoints;
}
