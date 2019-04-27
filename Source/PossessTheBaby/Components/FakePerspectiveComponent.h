#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FakePerspectiveComponent.generated.h"

class UWorldLimitsComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POSSESSTHEBABY_API UFakePerspectiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFakePerspectiveComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = Perspective)
	float TopScale = 0.8f;

	UPROPERTY(EditAnywhere, Category = Perspective)
	float BottomScale = 1.2f;

	UWorldLimitsComponent* GetWorldLimits() const;	
};
