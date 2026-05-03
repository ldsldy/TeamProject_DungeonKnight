// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerWeaponWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Data/WeaponDataAsset.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Subsystem/ViewModel/WeaponViewModel.h"

void UPlayerWeaponWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
        {
            if (!WeaponViewModel)
            {
                WeaponViewModel = Subsystem->GetWeaponViewModel();
            }

            if (!PlayerResourceViewModel)
            {
                PlayerResourceViewModel = Subsystem->GetPlayerStatusViewModel();
            }
        }
    }

    BindViewModel();
}

void UPlayerWeaponWidget::NativeDestruct()
{
    UnbindViewModel();

    Super::NativeDestruct();
}

void UPlayerWeaponWidget::UpdatePlayerResourceBar()
{
    if (!PlayerResourceViewModel)
    {
        return;
    }

    const float CurrentResource = PlayerResourceViewModel->GetCurrentEnergy();
    const float MaxResource = FMath::Max(1.0f, PlayerResourceViewModel->GetMaxEnergy());
    const float ResourcePercent = FMath::Clamp(CurrentResource / MaxResource, 0.0f, 1.0f);

    if (PlayerResourceBar)
    {
        PlayerResourceBar->SetPercent(ResourcePercent);
    }

    if (CurrentEnergyText && MaxEnergyText)
    {
        CurrentEnergyText->SetText(FText::AsNumber(FMath::FloorToInt(CurrentResource)));
        MaxEnergyText->SetText(FText::AsNumber(FMath::FloorToInt(MaxResource)));
    }
}

void UPlayerWeaponWidget::UpdateMainWeaponInfo()
{
    if (!WeaponViewModel)
    {
        return;
    }

    UWeaponDataAsset* InDataAsset = WeaponViewModel->GetMainWeaponData();
    if (!InDataAsset)
    {
        return;
    }

    if (WeaponIconImage)
    {
        WeaponIconImage->SetBrushFromTexture(InDataAsset->WeaponIcon);
    }

    if (WeaponName)
    {
        WeaponName->SetText(InDataAsset->WeaponName);
    }

    if (UsageResourceText)
    {
        const FText WeaponCost = FText::AsNumber(FMath::FloorToInt(InDataAsset->AttackCost));
        UsageResourceText->SetText(WeaponCost);
    }
}

void UPlayerWeaponWidget::UpdateSubWeaponInfo()
{
    if (!WeaponViewModel)
    {
        return;
    }

    UWeaponDataAsset* InDataAsset = WeaponViewModel->GetSubWeaponData();
    if (!InDataAsset)
    {
        return;
    }

    if (SubWeaponIconImage)
    {
        SubWeaponIconImage->SetBrushFromTexture(InDataAsset->WeaponIcon);
    }
}

void UPlayerWeaponWidget::BindViewModel()
{
    if (WeaponViewModel && PlayerResourceViewModel && !bIsViewModelBound)
    {
        WeaponViewModel->OnFieldChanged.AddDynamic(this, &UPlayerWeaponWidget::HandleWeaponFieldChanged);
        PlayerResourceViewModel->OnFieldChanged.AddDynamic(this, &UPlayerWeaponWidget::HandleResourceFieldChanged);

        bIsViewModelBound = true;

        UpdateMainWeaponInfo();
        UpdateSubWeaponInfo();
        UpdatePlayerResourceBar();
    }
}

void UPlayerWeaponWidget::UnbindViewModel()
{
    if (!bIsViewModelBound)
    {
        return;
    }

    if (WeaponViewModel)
    {
        WeaponViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerWeaponWidget::HandleWeaponFieldChanged);
    }

    if (PlayerResourceViewModel)
    {
        PlayerResourceViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerWeaponWidget::HandleResourceFieldChanged);
    }

    bIsViewModelBound = false;
}

void UPlayerWeaponWidget::HandleResourceFieldChanged(FName FieldName)
{
    if (FieldName == PlayerResourceVMFields::EnergyCurrent
        || FieldName == PlayerResourceVMFields::EnergyMax
        || FieldName == PlayerResourceVMFields::EnergyPercent)
    {
        UpdatePlayerResourceBar();
    }
}

void UPlayerWeaponWidget::HandleWeaponFieldChanged(FName FieldName)
{
    if (FieldName == WeaponVMFields::MainWeaponData)
    {
        UpdateMainWeaponInfo();
        return;
    }

    if (FieldName == WeaponVMFields::SubWeaponData)
    {
        UpdateSubWeaponInfo();
    }
}
