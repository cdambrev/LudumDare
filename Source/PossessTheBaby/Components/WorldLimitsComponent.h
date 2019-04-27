// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldLimitsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POSSESSTHEBABY_API UWorldLimitsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldLimitsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintGetter, Category="Settings")
	float GetMinX() const;

	UFUNCTION(BlueprintGetter, Category = "Settings")
	float GetMaxX() const;

	UFUNCTION(BlueprintGetter, Category = "Settings")
	float GetMinZ() const;

	UFUNCTION(BlueprintGetter, Category = "Settings")
	float GetMaxZ() const;

private:
	UPROPERTY(EditAnywhere, BlueprintGetter=GetMinX, Category = "Settings")
	float _minX = -925.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxX, Category = "Settings")
	float _maxX = 925.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMinZ, Category = "Settings")
	float _minZ = -325.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxZ, Category = "Settings")
	float _maxZ = 200.0f;
};
