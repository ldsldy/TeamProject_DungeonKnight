// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ViewModel/PGViewModelBase.h"
#include "EnemyViewModel.generated.h"

class ABossBase;

/**
 * 
 */
UCLASS(Blueprintable)
class TEAMPOTATO_API UEnemyViewModel : public UPGViewModelBase
{
	GENERATED_BODY()
	
public:
    virtual void Initialize(UObject* InModel) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintPure)
    float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure)
    float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintPure)
    float GetHealthPercent() const { return HealthPercent; }

    UFUNCTION(BlueprintPure)
    bool IsBossVisible() const { return bIsVisible; }

private:
    UFUNCTION()
    void HandleBossHealthChanged(float InCurrentHealth, float InMaxHealth);

    UFUNCTION()
    void HandleBossSpawned();

    UFUNCTION()
    void HandleBossDied();

    void UpdateHealthPercent();

private:
    UPROPERTY()
    TObjectPtr<ABossBase> Model = nullptr;

    UPROPERTY()
    float CurrentHealth = 0.0f;

    UPROPERTY()
    float MaxHealth = 1.0f;

    UPROPERTY()
    float HealthPercent = 0.0f;

    UPROPERTY()
    bool bIsVisible = false;
};
