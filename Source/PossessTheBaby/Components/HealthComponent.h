// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POSSESSTHEBABY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsDead() const;

	UFUNCTION(BlueprintGetter, Category="State")
	float GetDreamPoints() const;

	UFUNCTION(BlueprintGetter, Category = "State")
	float GetNightmarePoints() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintGetter="GetDreamPoints", Category="State")
	float DreamPoints = 0;

	UPROPERTY(BlueprintGetter = "GetNightmarePoints", Category = "State")
	float NightmarePoints = 0;
};
