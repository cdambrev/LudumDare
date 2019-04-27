#pragma once

// UE4
#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UObject/WeakObjectPtrTemplates.h"

// Generated
#include "HudPresenter.generated.h"

class UHealthComponent;

UCLASS()
class UHudPresenter : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Presentor")
	void Initialize(UHealthComponent* health);

	UFUNCTION(BlueprintImplementableEvent, Category="Presentor")
	void OnDreamPointsChanged(float health);

	UFUNCTION(BlueprintImplementableEvent, Category = "Presentor")
	void OnNightmarePointsChanged(float energy);

private:
	UPROPERTY(Transient)
	UHealthComponent* _health = nullptr;

	FDelegateHandle _onDreamPointsChangedHandle;
	FDelegateHandle _onNightmarePointsChangedHandle;
};
