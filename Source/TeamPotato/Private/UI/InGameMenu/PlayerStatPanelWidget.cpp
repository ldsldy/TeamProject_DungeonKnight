// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameMenu/PlayerStatPanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Subsystem/ViewModel/WeaponViewModel.h"
#include "Data/WeaponDataAsset.h"

void UPlayerStatPanelWidget::NativeConstruct()
{
    Super::NativeConstruct();

}

void UPlayerStatPanelWidget::NativeDestruct()
{
    UnbindViewModel();

    Super::NativeDestruct();
}

void UPlayerStatPanelWidget::SetViewModel(UPlayerResourceViewModel* InPlayerViewModel, UWeaponViewModel* InWeaponViewModel)
{
    UnbindViewModel();

    PlayerStatusViewModel = InPlayerViewModel;
    PlayerWeaponViewModel = InWeaponViewModel;

    BindViewModel();
}

void UPlayerStatPanelWidget::BindViewModel()
{
    if (PlayerStatusViewModel && !bIsBound
        && PlayerWeaponViewModel && !bIsBound)
    {
        UE_LOG(LogTemp, Warning, TEXT("UPlayerStatPanelWidget::BindViewModel - Binding to PlayerStatusViewModel"));

        PlayerStatusViewModel->OnFieldChanged.AddDynamic(this, &UPlayerStatPanelWidget::HandlePlayerResourceFieldChanged);

        UE_LOG(LogTemp, Warning, TEXT("UPlayerStatPanelWidget::BindViewModel - Binding to WeaponViewModel"));

        PlayerWeaponViewModel->OnMainWeaponUpdate.AddDynamic(this, &UPlayerStatPanelWidget::UpdateWeaponDataUI);

        bIsBound = true;

        UpdateHealthUI();
        UpdateEnergyUI();
        UpdateWalkSpeedUI();
    }
}

void UPlayerStatPanelWidget::UnbindViewModel()
{
    if(PlayerStatusViewModel && bIsBound)
    {
        PlayerStatusViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerStatPanelWidget::HandlePlayerResourceFieldChanged);
    }
    if(PlayerWeaponViewModel && bIsBound)
    {
        PlayerWeaponViewModel->OnMainWeaponUpdate.RemoveDynamic(this, &UPlayerStatPanelWidget::UpdateWeaponDataUI);
    }

    bIsBound = false;
}

//void UPlayerStatPanelWidget::OnPlayerStatHealthChanged(float InCurrentHealth, float InMaxHealth)
//{
//    UpdateHealthUI(InCurrentHealth, InMaxHealth);
//}
//
//void UPlayerStatPanelWidget::OnPlayerStatEnergyChanged(float CurrentResource, float MaxResource)
//{
//    UpdateEnergyUI(CurrentResource, MaxResource);
//}

void UPlayerStatPanelWidget::HandlePlayerResourceFieldChanged(FName FieldName)
{
    if (FieldName == PlayerResourceVMFields::HealthCurrent
        || FieldName == PlayerResourceVMFields::HealthMax
        || FieldName == PlayerResourceVMFields::HealthPercent)
    {
        UpdateHealthUI();
        return;
    }

    if (FieldName == PlayerResourceVMFields::EnergyCurrent
        || FieldName == PlayerResourceVMFields::EnergyMax
        || FieldName == PlayerResourceVMFields::EnergyPercent)
    {
        UpdateEnergyUI();
        return;
    }

    if (FieldName == PlayerResourceVMFields::WalkSpeed)
    {
        UpdateWalkSpeedUI();
    }
}

void UPlayerStatPanelWidget::UpdateHealthUI()
{
    if (!PlayerStatusViewModel) return;

    const float CurrentHealth = PlayerStatusViewModel->GetCurrentHealth();
    const float MaxHealth = FMath::Max(PlayerStatusViewModel->GetMaxHealth(), 1.0f);

    if(HealthProgressBar)
    {
        float HealthPercent = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
        HealthProgressBar->SetPercent(HealthPercent);
    }
    if(CurrentHealthText)
    {
        FText HealthText = FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth));
        CurrentHealthText->SetText(HealthText);
    }
    if(HealthStatText)
    {
        FText HealthStat = FText::FromString(FString::Printf(TEXT("%.0f"), MaxHealth));
        HealthStatText->SetText(HealthStat);
    }
}

void UPlayerStatPanelWidget::UpdateEnergyUI()
{
    if (!PlayerStatusViewModel) return;

    const float CurrentResource = PlayerStatusViewModel->GetCurrentEnergy();
    const float MaxResource = FMath::Max(PlayerStatusViewModel->GetMaxEnergy(), 1.0f);

    if(EnergyProgressBar)
    {
        float EnergyPercent = FMath::Clamp(CurrentResource / MaxResource, 0.0f, 1.0f);
        EnergyProgressBar->SetPercent(EnergyPercent);
    }
    if(CurrentEnergyText)
    {
        FText EnergyText = FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentResource, MaxResource));
        CurrentEnergyText->SetText(EnergyText);
    }
    if(EnergyStatText)
    {
        FText EnergyStat = FText::FromString(FString::Printf(TEXT("%.0f"), MaxResource));
        EnergyStatText->SetText(EnergyStat);
    }
}

void UPlayerStatPanelWidget::UpdateWeaponDataUI(UWeaponDataAsset* InWeaponData)
{
    if (AttackDamageText)
    {
        if(InWeaponData)
        {
            FText AttackDamageStat = FText::FromString(FString::Printf(TEXT("%.0f"), InWeaponData->AttackDamage));
            AttackDamageText->SetText(AttackDamageStat);
        }
    }
}

void UPlayerStatPanelWidget::UpdateWalkSpeedUI()
{
    if (!PlayerStatusViewModel) return;

    if (WalkSpeedText)
    {
        const float NewWalkSpeed = PlayerStatusViewModel->GetWalkSpeed();
        FText WalkSpeedStat = FText::FromString(FString::Printf(TEXT("%.0f"), NewWalkSpeed));
        WalkSpeedText->SetText(WalkSpeedStat);
    }
}
