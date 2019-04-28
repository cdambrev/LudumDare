// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlickerComponent.generated.h"

class UPaperFlipbookComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POSSESSTHEBABY_API UFlickerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	enum FlickType
	{
		Flicker,
		Tint
	};

	// Sets default values for this component's properties
	UFlickerComponent();

	void SetSprite(UPaperFlipbookComponent* sprite);
	void TintFlick(float duration, FLinearColor color);
	void Flick(float rate, float duration);

public:	
	// Called every frame
	virtual void TickComponent(float deltaSeconds, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	UPROPERTY()
	UPaperFlipbookComponent* _sprite = nullptr;

	float _duration = 0.1f;
	float _rate = 0.1f;
	FLinearColor _color;

	FlickType _type = FlickType::Tint;

	float _timeLeft = 0.0f;
};

