// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ViewModel/PGViewModelBase.h"
#include "PlayerResourceViewModel.generated.h"

class UPlayerResource;

// 플레이어 리소스 뷰모델에서 사용할 필드 이름 상수들
namespace PlayerResourceVMFields
{
    TEAMPOTATO_API extern const FName HealthCurrent;
    TEAMPOTATO_API extern const FName HealthMax;
    TEAMPOTATO_API extern const FName HealthPercent;

    TEAMPOTATO_API extern const FName EnergyCurrent;
    TEAMPOTATO_API extern const FName EnergyMax;
    TEAMPOTATO_API extern const FName EnergyPercent;

    TEAMPOTATO_API extern const FName Gold;
    TEAMPOTATO_API extern const FName WalkSpeed;
    TEAMPOTATO_API extern const FName AttackPower;
}

/**
 * 
 */
UCLASS(Blueprintable)
class TEAMPOTATO_API UPlayerResourceViewModel : public UPGViewModelBase
{
	GENERATED_BODY()
	
public:
    virtual void Initialize(UObject* InModel) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintPure) float GetCurrentHealth() const { return CurrentHealth; }
    UFUNCTION(BlueprintPure) float GetMaxHealth() const { return MaxHealth; }
    UFUNCTION(BlueprintPure) float GetHealthPercent() const { return HealthPercent; }
    UFUNCTION(BlueprintPure) float GetCurrentEnergy() const { return CurrentEnergy; }
    UFUNCTION(BlueprintPure) float GetMaxEnergy() const { return MaxEnergy; }
    UFUNCTION(BlueprintPure) float GetEnergyPercent() const { return EnergyPercent; }
    UFUNCTION(BlueprintPure) int32 GetCurrentGold() const { return CurrentGold; }
    UFUNCTION(BlueprintPure) float GetWalkSpeed() const { return WalkSpeed; }
    UFUNCTION(BlueprintPure) float GetAttackPower() const { return AttackPower; }

private:
    UFUNCTION()
    void HandleModelScalarChanged(FName Key, float Value);

    void RecalculateHealthDerived();
    void RecalculateEnergyDerived();

    // 모델에서 가져온 값들
    UPROPERTY()
    TObjectPtr<UPlayerResource> Model = nullptr;

    UPROPERTY()
    float CurrentHealth = 0.0f;

    UPROPERTY()
    float MaxHealth = 1.0f;

    UPROPERTY()
    float HealthPercent = 0.0f;

    UPROPERTY()
    float CurrentEnergy = 0.0f;

    UPROPERTY()
    float MaxEnergy = 1.0f;

    UPROPERTY()
    float EnergyPercent = 0.0f;

    UPROPERTY()
    int32 CurrentGold = 0;

    UPROPERTY()
    float WalkSpeed = 0.0f;

    UPROPERTY()
    float AttackPower = 0.0f;
};
