#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldStateComponent.generated.h"

UENUM(BlueprintType)
enum class EWorldState : uint8
{
	Dream,
	Nightmare
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POSSESSTHEBABY_API UWorldStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWorldStateComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsInDreamWorld() const;
	bool IsInNightmareWorld() const;
	EWorldState GetWorldState() const;
	void ToggleWorldState();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWorldStateChanged, EWorldState)
	FOnWorldStateChanged OnWorldStateChanged;

protected:
	virtual void BeginPlay() override;

private:
	EWorldState State = EWorldState::Dream;
};
