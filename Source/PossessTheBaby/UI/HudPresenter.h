#pragma once

#include "Components/WorldStateComponent.h"

// UE4
#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UObject/WeakObjectPtrTemplates.h"

// Generated
#include "HudPresenter.generated.h"

class UHealthComponent;
class UCombatComponent;

UCLASS()
class UHudPresenter : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Presentor")
	void SetHealthComponent(UHealthComponent* health);

	UFUNCTION(BlueprintCallable, Category = "Presentor")
	void SetWorldStateComponent(UWorldStateComponent* worldState);

	UFUNCTION(BlueprintCallable, Category = "Presentor")
	void SetCombatComponent(UCombatComponent* combat);

	UFUNCTION(BlueprintImplementableEvent, Category="Presentor")
	void OnDreamPointsChanged(float dreamPoints);

	UFUNCTION(BlueprintImplementableEvent, Category = "Presentor")
	void OnNightmarePointsChanged(float nightmarePoints);

	UFUNCTION(BlueprintImplementableEvent, Category = "Presentor")
	void OnWorldStateChanged(EWorldState worldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "Presentor")
	void OnSuccessfulHitChanged(int32 count);

private:
	UPROPERTY(Transient)
	UHealthComponent* _health = nullptr;

	UPROPERTY(Transient)
	UWorldStateComponent* _worldState = nullptr;

	UPROPERTY(Transient)
	UCombatComponent* _combat = nullptr;

	FDelegateHandle _onDreamPointsChangedHandle;
	FDelegateHandle _onNightmarePointsChangedHandle;
	FDelegateHandle _onWorldStateChangedHandle;
	FDelegateHandle _onCombatChangedHandle;
};
