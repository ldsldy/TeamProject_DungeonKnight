// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Component/PlayerResource.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"

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
    HandleModelScalarChanged(PlayerResourceVMFields::HealthCurrent, Model->GetHealthAmount());
    HandleModelScalarChanged(PlayerResourceVMFields::HealthMax, Model->GetMaxHealthAmount());
    HandleModelScalarChanged(PlayerResourceVMFields::EnergyCurrent, Model->GetEnergyAmount());
    HandleModelScalarChanged(PlayerResourceVMFields::EnergyMax, Model->GetMaxEnergyAmount());
    HandleModelScalarChanged(PlayerResourceVMFields::Gold, static_cast<float>(Model->GetCurrentGold()));
    HandleModelScalarChanged(PlayerResourceVMFields::WalkSpeed, Model->GetWalkSpeed());
    HandleModelScalarChanged(PlayerResourceVMFields::AttackPower, Model->GetAttackPower());
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
    if (Key == PlayerResourceVMFields::HealthCurrent)
    {
        SetAndNotifyFloat(CurrentHealth, Value, PlayerResourceVMFields::HealthCurrent);
        RecalculateHealthDerived();
        return;
    }
    
    if (Key == PlayerResourceVMFields::HealthMax)
    {
        SetAndNotifyFloat(MaxHealth, FMath::Max(1.0f, Value), PlayerResourceVMFields::HealthMax);
        RecalculateHealthDerived();
        return;
    }
    
    if (Key == PlayerResourceVMFields::EnergyCurrent)
    {
        SetAndNotifyFloat(CurrentEnergy, Value, PlayerResourceVMFields::EnergyCurrent);
        RecalculateEnergyDerived();
        return;
    }
    
    if (Key == PlayerResourceVMFields::EnergyMax)
    {
        SetAndNotifyFloat(MaxEnergy, FMath::Max(1.0f, Value), PlayerResourceVMFields::EnergyMax);
        RecalculateEnergyDerived();
        return;
    }
    
    if (Key == PlayerResourceVMFields::Gold)
    {
        SetAndNotify(CurrentGold, FMath::RoundToInt(Value), PlayerResourceVMFields::Gold);
        return;
    }
    
    if (Key == PlayerResourceVMFields::WalkSpeed)
    {
        SetAndNotifyFloat(WalkSpeed, Value, PlayerResourceVMFields::WalkSpeed);
        return;
    }
    
    if (Key == PlayerResourceVMFields::AttackPower)
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
