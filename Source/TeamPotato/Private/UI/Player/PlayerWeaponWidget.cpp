// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerWeaponWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/WeaponViewModel.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"

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

void UPlayerWeaponWidget::UpdatePlayerResourceBar(float CurrentResource, float MaxResource)
{
    float ResourcePercent = FMath::Clamp(CurrentResource / MaxResource, 0.0f, 1.0f);

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

void UPlayerWeaponWidget::UpdateMainWeaponInfo(UWeaponDataAsset* InDataAsset)
{
    if(WeaponIconImage)
    {
        WeaponIconImage->SetBrushFromTexture(InDataAsset->WeaponIcon);
    }
    if(WeaponName)
    {
        WeaponName->SetText(InDataAsset->WeaponName);
    }
    if (UsageResourceText)
    {
        FText InWeaponCost = FText::AsNumber(FMath::FloorToInt(InDataAsset->AttackCost));
        UsageResourceText->SetText(InWeaponCost);
    }
}

void UPlayerWeaponWidget::UpdateSubWeaponInfo(UWeaponDataAsset* InDataAsset)
{
    if (SubWeaponIconImage)
    {
        SubWeaponIconImage->SetBrushFromTexture(InDataAsset->WeaponIcon);
    }
}

void UPlayerWeaponWidget::BindViewModel()
{
    // 이미 바인딩된 경우 무시
    if (WeaponViewModel && PlayerResourceViewModel && !bIsViewModelBound)
    {
        // Model -> ViewModel 바인딩
        WeaponViewModel->OnMainWeaponUpdate.AddDynamic(this, &UPlayerWeaponWidget::UpdateMainWeaponInfo);
        WeaponViewModel->OnSubWeaponUpdate.AddDynamic(this, &UPlayerWeaponWidget::UpdateSubWeaponInfo);
        PlayerResourceViewModel->OnFieldChanged.AddDynamic(this, &UPlayerWeaponWidget::HandleResourceFieldChanged);

        bIsViewModelBound = true;

        UpdatePlayerResourceBar(PlayerResourceViewModel->GetCurrentEnergy(), PlayerResourceViewModel->GetMaxEnergy());
    }
}

void UPlayerWeaponWidget::UnbindViewModel()
{
    if (bIsViewModelBound)
    {
        if (WeaponViewModel)
        {
            WeaponViewModel->OnMainWeaponUpdate.RemoveDynamic(this, &UPlayerWeaponWidget::UpdateMainWeaponInfo);
            WeaponViewModel->OnSubWeaponUpdate.RemoveDynamic(this, &UPlayerWeaponWidget::UpdateSubWeaponInfo);
        }

        if (PlayerResourceViewModel)
        {
            PlayerResourceViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerWeaponWidget::HandleResourceFieldChanged);
        }

        bIsViewModelBound = false;
    }
}

void UPlayerWeaponWidget::HandleResourceFieldChanged(FName FieldName)
{
    if (!PlayerResourceViewModel)
    {
        return;
    }

    if (FieldName == PlayerResourceVMFields::EnergyCurrent
        || FieldName == PlayerResourceVMFields::EnergyMax
        || FieldName == PlayerResourceVMFields::EnergyPercent)
    {
        UpdatePlayerResourceBar(PlayerResourceViewModel->GetCurrentEnergy(), PlayerResourceViewModel->GetMaxEnergy());
    }
}
