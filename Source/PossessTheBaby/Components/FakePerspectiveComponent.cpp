#include "FakePerspectiveComponent.h"

#include "Components/WorldLimitsComponent.h"
#include "PossessTheBabyGameState.h"

// Sets default values for this component's properties
UFakePerspectiveComponent::UFakePerspectiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called every frame
void UFakePerspectiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* target = GetOwner();
	UWorldLimitsComponent* worldLimits = GetWorldLimits();
	float rangeZ = worldLimits->GetMaxZ() - worldLimits->GetMinZ();

	FVector position = target->GetActorLocation();
	if (_restrictInX)
	{
		position.X = FMath::Clamp(position.X, worldLimits->GetMinX(), worldLimits->GetMaxX());
	}
	if (_restrictInZ)
	{
		position.Z = FMath::Clamp(position.Z, worldLimits->GetMinZ(), worldLimits->GetMaxZ());
	}
	position.Y = -position.Z / 10.0f;

	float clampedPosZ = FMath::Clamp(position.Z, worldLimits->GetMinZ(), worldLimits->GetMaxZ());
	float alpha = 1.0f - (clampedPosZ / rangeZ);
	float scale = FMath::Lerp(TopScale, BottomScale, alpha);
	target->SetActorScale3D(FVector(scale, scale, scale));
	target->SetActorLocation(position);
}

UWorldLimitsComponent* UFakePerspectiveComponent::GetWorldLimits() const
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return gameState->GetWorldLimits();
}

void UFakePerspectiveComponent::SetRestrictInX(bool value)
{
	_restrictInX = value;
}

void UFakePerspectiveComponent::SetRestrictInZ(bool value)
{
	_restrictInZ = value;
}

