// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Component/PlayerResource.h"

namespace PlayerResourceVMFields
{
    const FName HealthCurrent(TEXT("HealthCurrent"));
    const FName HealthMax(TEXT("HealthMax"));
    const FName HealthPercent(TEXT("HealthPercent"));

    const FName EnergyCurrent(TEXT("EnergyCurrent"));
    const FName EnergyMax(TEXT("EnergyMax"));
    const FName EnergyPercent(TEXT("EnergyPercent"));

    const FName Gold(TEXT("Gold"));
    const FName WalkSpeed(TEXT("WalkSpeed"));
    const FName AttackPower(TEXT("AttackPower"));
}

void UPlayerResourceViewModel::Initialize(UObject* InModel)
{
    Deinitialize();

    Super::Initialize(InModel);

    Model = Cast<UPlayerResource>(InModel);

    if (!Model)
    {
        return;
    }

    Model->OnScalarChanged.AddDynamic(this, &UPlayerResourceViewModel::HandleModelScalarChanged);

    // 변경된 모델의 현재 상태로 뷰모델의 필드 초기화
    HandleModelScalarChanged(PlayerResourceScalarKeys::HealthCurrent, Model->GetHealthAmount());
    HandleModelScalarChanged(PlayerResourceScalarKeys::HealthMax, Model->GetMaxHealthAmount());
    HandleModelScalarChanged(PlayerResourceScalarKeys::EnergyCurrent, Model->GetEnergyAmount());
    HandleModelScalarChanged(PlayerResourceScalarKeys::EnergyMax, Model->GetMaxEnergyAmount());
    HandleModelScalarChanged(PlayerResourceScalarKeys::Gold, static_cast<float>(Model->GetCurrentGold()));
    HandleModelScalarChanged(PlayerResourceScalarKeys::WalkSpeed, Model->GetWalkSpeed());
    HandleModelScalarChanged(PlayerResourceScalarKeys::AttackPower, Model->GetAttackPower());
}

void UPlayerResourceViewModel::Deinitialize()
{
    if (Model)
    {
        Model->OnScalarChanged.RemoveDynamic(this, &UPlayerResourceViewModel::HandleModelScalarChanged);
        Model = nullptr;
    }
    Super::Deinitialize();
}

void UPlayerResourceViewModel::HandleModelScalarChanged(FName Key, float Value)
{
    if (Key == PlayerResourceScalarKeys::HealthCurrent)
    {
        SetAndNotifyFloat(CurrentHealth, Value, PlayerResourceVMFields::HealthCurrent);
        RecalculateHealthDerived();
        return;
    }
    
    if (Key == PlayerResourceScalarKeys::HealthMax)
    {
        SetAndNotifyFloat(MaxHealth, FMath::Max(1.0f, Value), PlayerResourceVMFields::HealthMax);
        RecalculateHealthDerived();
        return;
    }
    
    if (Key == PlayerResourceScalarKeys::EnergyCurrent)
    {
        SetAndNotifyFloat(CurrentEnergy, Value, PlayerResourceVMFields::EnergyCurrent);
        RecalculateEnergyDerived();
        return;
    }
    
    if (Key == PlayerResourceScalarKeys::EnergyMax)
    {
        SetAndNotifyFloat(MaxEnergy, FMath::Max(1.0f, Value), PlayerResourceVMFields::EnergyMax);
        RecalculateEnergyDerived();
        return;
    }
    
    if (Key == PlayerResourceScalarKeys::Gold)
    {
        SetAndNotify(CurrentGold, FMath::RoundToInt(Value), PlayerResourceVMFields::Gold);
        return;
    }
    
    if (Key == PlayerResourceScalarKeys::WalkSpeed)
    {
        SetAndNotifyFloat(WalkSpeed, Value, PlayerResourceVMFields::WalkSpeed);
        return;
    }
    
    if (Key == PlayerResourceScalarKeys::AttackPower)
    {
        SetAndNotifyFloat(AttackPower, Value, PlayerResourceVMFields::AttackPower);
        return;
    }
}

void UPlayerResourceViewModel::RecalculateHealthDerived()
{
    const float SafeMax = FMath::Max(1.0f, MaxHealth);
    const float NewPercent = FMath::Clamp(CurrentHealth / SafeMax, 0.0f, 1.0f);

    SetAndNotifyFloat(HealthPercent, NewPercent, PlayerResourceVMFields::HealthPercent);
}

void UPlayerResourceViewModel::RecalculateEnergyDerived()
{
    const float SafeMax = FMath::Max(1.0f, MaxEnergy);
    const float NewPercent = FMath::Clamp(CurrentEnergy / SafeMax, 0.0f, 1.0f);

    SetAndNotifyFloat(EnergyPercent, NewPercent, PlayerResourceVMFields::EnergyPercent);
}
