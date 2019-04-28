#include "WorldLimitsComponent.h"

// UE4
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UWorldLimitsComponent::UWorldLimitsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWorldLimitsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FVector topRight(GetMinX(), 0.0f, GetMaxZ());
	FVector topLeft(GetMaxX(), 0.0f, GetMaxZ());
	FVector bottomRight(GetMinX(), 0.0f, GetMinZ());
	FVector bottomLeft(GetMaxX(), 0.0f, GetMinZ());

	DrawDebugLine(GetWorld(), topLeft, topRight, FColor::Blue);
	DrawDebugLine(GetWorld(), topRight, bottomRight, FColor::Blue);
	DrawDebugLine(GetWorld(), bottomRight, bottomLeft, FColor::Blue);
	DrawDebugLine(GetWorld(), bottomLeft, topLeft, FColor::Blue);
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

bool UWorldLimitsComponent::IsInWorldLimits(FVector position) const
{
	return position.X < _maxX&& position.X > _minX&& position.Z < _maxZ&& position.Z > _minZ;
}
