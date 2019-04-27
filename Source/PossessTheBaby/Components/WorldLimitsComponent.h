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

	UFUNCTION(BlueprintGetter, Category="Settings")
	float GetMinX() const;

	UFUNCTION(BlueprintGetter, Category = "Settings")
	float GetMaxX() const;

	UFUNCTION(BlueprintGetter, Category = "Settings")
	float GetMinZ() const;

	UFUNCTION(BlueprintGetter, Category = "Settings")
	float GetMaxZ() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintGetter=GetMinX, Category = "Settings")
	float _minX = -1000.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxX, Category = "Settings")
	float _maxX = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMinZ, Category = "Settings")
	float _minZ = -600.0f;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxZ, Category = "Settings")
	float _maxZ = 300.0f;
};
