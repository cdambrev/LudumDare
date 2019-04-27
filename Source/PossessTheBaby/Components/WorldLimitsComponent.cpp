#include "WorldLimitsComponent.h"

// Sets default values for this component's properties
UWorldLimitsComponent::UWorldLimitsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldLimitsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

float UWorldLimitsComponent::GetMinX() const
{
	return _minX;
}

float UWorldLimitsComponent::GetMaxX() const
{
	return _maxX;
}

float UWorldLimitsComponent::GetMinZ() const
{
	return _minZ;
}

float UWorldLimitsComponent::GetMaxZ() const
{
	return _maxZ;
}
