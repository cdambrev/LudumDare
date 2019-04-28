// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "HealthComponent.generated.h"

/*
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Depletion,
	Punch
};
*/

class UWorldStateComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POSSESSTHEBABY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	DECLARE_MULTICAST_DELEGATE(FOnEvent)

	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsDead() const;

	FOnEvent OnDeath;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatChanged, float)

	UFUNCTION(BlueprintGetter, Category="State")
	float GetDreamPoints() const;
	float GetDreamPointsPercent() const;
	void AddDreamPoints(float points);
	FOnFloatChanged OnDreamPointsChanged;
	FOnFloatChanged OnDreamPointsPercentChanged;

	UFUNCTION(BlueprintGetter, Category = "State")
	float GetNightmarePoints() const;
	float GetNightmarePointsPercent() const;
	void AddNightmarePoints(float points);
	FOnFloatChanged OnNightmarePointsChanged;
	FOnFloatChanged OnNightmarePointsPercentChanged;

	void ApplyDamage(float value);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SetDreamPoints(float value);
	void SetNightmarePoints(float value);
	void UpdateDeath();

	UPROPERTY(BlueprintGetter="GetDreamPoints", Category="State")
	float DreamPoints = 1.0f;

	UPROPERTY(BlueprintGetter = "GetNightmarePoints", Category = "State")
	float NightmarePoints = 1.0f;

	//  Points per seconds depleted in the opposite world.
	UPROPERTY(EditAnywhere, Category = "Settings")
	float _pointsDepletionRate = 0.5f;

	//  Points per seconds depleted in the opposite world.
	UPROPERTY(EditAnywhere, Category = "Settings")
	float _maxPoints = 65.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float _initialDreamPoints = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float _initialNightmarePoints = 0.0f;

	UPROPERTY(Transient)
	UWorldStateComponent* _worldState = nullptr;
};
