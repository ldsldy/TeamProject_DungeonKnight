// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameMenu/PlayerStatPanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Data/WeaponDataAsset.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Subsystem/ViewModel/WeaponViewModel.h"

void UPlayerStatPanelWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
        {
            if (!PlayerStatusViewModel)
            {
                PlayerStatusViewModel = Subsystem->GetPlayerStatusViewModel();
            }

            if (!PlayerWeaponViewModel)
            {
                PlayerWeaponViewModel = Subsystem->GetWeaponViewModel();
            }
        }
    }

    BindViewModel();
}

void UPlayerStatPanelWidget::NativeDestruct()
{
    UnbindViewModel();

    Super::NativeDestruct();
}

void UPlayerStatPanelWidget::BindViewModel()
{
    if (!bIsBound && PlayerStatusViewModel && PlayerWeaponViewModel)
    {
        PlayerStatusViewModel->OnFieldChanged.AddDynamic(this, &UPlayerStatPanelWidget::HandlePlayerResourceFieldChanged);
        PlayerWeaponViewModel->OnFieldChanged.AddDynamic(this, &UPlayerStatPanelWidget::HandleWeaponFieldChanged);

        bIsBound = true;

        UpdateHealthUI();
        UpdateEnergyUI();
        UpdateWalkSpeedUI();
        UpdateWeaponDataUI();
    }
}

void UPlayerStatPanelWidget::UnbindViewModel()
{
    if (!bIsBound)
    {
        return;
    }

    if (PlayerStatusViewModel)
    {
        PlayerStatusViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerStatPanelWidget::HandlePlayerResourceFieldChanged);
    }

    if (PlayerWeaponViewModel)
    {
        PlayerWeaponViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerStatPanelWidget::HandleWeaponFieldChanged);
    }

    bIsBound = false;
}

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

void UPlayerStatPanelWidget::HandleWeaponFieldChanged(FName FieldName)
{
    if (FieldName == WeaponVMFields::MainWeaponData)
    {
        UpdateWeaponDataUI();
    }
}

void UPlayerStatPanelWidget::UpdateHealthUI()
{
    if (!PlayerStatusViewModel)
    {
        return;
    }

    const float CurrentHealth = PlayerStatusViewModel->GetCurrentHealth();
    const float MaxHealth = FMath::Max(PlayerStatusViewModel->GetMaxHealth(), 1.0f);

    if (HealthProgressBar)
    {
        const float HealthPercent = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
        HealthProgressBar->SetPercent(HealthPercent);
    }

    if (CurrentHealthText)
    {
        const FText HealthText = FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHealth, MaxHealth));
        CurrentHealthText->SetText(HealthText);
    }

    if (HealthStatText)
    {
        const FText HealthStat = FText::FromString(FString::Printf(TEXT("%.0f"), MaxHealth));
        HealthStatText->SetText(HealthStat);
    }
}

void UPlayerStatPanelWidget::UpdateEnergyUI()
{
    if (!PlayerStatusViewModel)
    {
        return;
    }

    const float CurrentResource = PlayerStatusViewModel->GetCurrentEnergy();
    const float MaxResource = FMath::Max(PlayerStatusViewModel->GetMaxEnergy(), 1.0f);

    if (EnergyProgressBar)
    {
        const float EnergyPercent = FMath::Clamp(CurrentResource / MaxResource, 0.0f, 1.0f);
        EnergyProgressBar->SetPercent(EnergyPercent);
    }

    if (CurrentEnergyText)
    {
        const FText EnergyText = FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentResource, MaxResource));
        CurrentEnergyText->SetText(EnergyText);
    }

    if (EnergyStatText)
    {
        const FText EnergyStat = FText::FromString(FString::Printf(TEXT("%.0f"), MaxResource));
        EnergyStatText->SetText(EnergyStat);
    }
}

void UPlayerStatPanelWidget::UpdateWeaponDataUI()
{
    if (!PlayerWeaponViewModel || !AttackDamageText)
    {
        return;
    }

    if (UWeaponDataAsset* InWeaponData = PlayerWeaponViewModel->GetMainWeaponData())
    {
        const FText AttackDamageStat = FText::FromString(FString::Printf(TEXT("%.0f"), InWeaponData->AttackDamage));
        AttackDamageText->SetText(AttackDamageStat);
    }
}

void UPlayerStatPanelWidget::UpdateWalkSpeedUI()
{
    if (!PlayerStatusViewModel || !WalkSpeedText)
    {
        return;
    }

    const float NewWalkSpeed = PlayerStatusViewModel->GetWalkSpeed();
    const FText WalkSpeedStat = FText::FromString(FString::Printf(TEXT("%.0f"), NewWalkSpeed));
    WalkSpeedText->SetText(WalkSpeedStat);
}
