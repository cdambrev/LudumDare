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


// Called when the game starts
void UFakePerspectiveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFakePerspectiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*
	AActor* target = GetOwner();
	UWorldLimitsComponent* worldLimits = GetWorldLimits();
	float rangeZ = worldLimits->GetMax

	const FVector& position = target->GetActorLocation();
	float alpha = 
	float scale = FMath::Lerp(TopScale, BottomScale, alpha);
	target->SetActorScale3D(FVector(scale, scale, scale));
	*/
}

UWorldLimitsComponent* UFakePerspectiveComponent::GetWorldLimits() const
{
	APossessTheBabyGameState* gameState = GetWorld()->GetGameState<APossessTheBabyGameState>();
	return gameState->GetWorldLimits();
}