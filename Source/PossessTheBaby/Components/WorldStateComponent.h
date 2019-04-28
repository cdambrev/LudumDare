#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldStateComponent.generated.h"

class UAudioComponent;

UENUM(BlueprintType)
enum class EWorldState : uint8
{
	Dream,
	Nightmare
};

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* AmbienceDream = nullptr;

	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* AmbienceNightmare = nullptr;

private:
	EWorldState State = EWorldState::Dream;

	UPROPERTY(Transient)
	UAudioComponent* _dreamAmbienceInstance = nullptr;

	UPROPERTY(Transient)
	UAudioComponent* _nightmareAmbienceInstance = nullptr;
};
