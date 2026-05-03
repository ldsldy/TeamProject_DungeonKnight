// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameMenu/PlayerStatWeaponWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/WeaponDataAsset.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "Subsystem/ViewModel/WeaponViewModel.h"

void UPlayerStatWeaponWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!WeaponViewModel)
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
            {
                WeaponViewModel = Subsystem->GetWeaponViewModel();
            }
        }
    }

    BindViewModel();
}

void UPlayerStatWeaponWidget::NativeDestruct()
{
    UnbindViewModel();

    Super::NativeDestruct();
}

void UPlayerStatWeaponWidget::UpdateMainWeaponIcon()
{
    if (!WeaponViewModel)
    {
        return;
    }

    if (UWeaponDataAsset* InDataAsset = WeaponViewModel->GetMainWeaponData())
    {
        if (MainWeaponIcon)
        {
            MainWeaponIcon->SetBrushFromTexture(InDataAsset->WeaponIcon);
        }
        if (MainWeaponName)
        {
            MainWeaponName->SetText(InDataAsset->WeaponName);
        }
    }
}

void UPlayerStatWeaponWidget::UpdateSubWeaponIcon()
{
    if (!WeaponViewModel)
    {
        return;
    }

    if (UWeaponDataAsset* InDataAsset = WeaponViewModel->GetSubWeaponData())
    {
        if (SubWeaponIcon)
        {
            SubWeaponIcon->SetBrushFromTexture(InDataAsset->WeaponIcon);
        }
        if (SubWeaponName)
        {
            SubWeaponName->SetText(InDataAsset->WeaponName);
        }
    }
}

void UPlayerStatWeaponWidget::BindViewModel()
{
    if (WeaponViewModel && !bIsBound)
    {
        WeaponViewModel->OnFieldChanged.AddDynamic(this, &UPlayerStatWeaponWidget::HandleWeaponFieldChanged);

        bIsBound = true;
        UpdateMainWeaponIcon();
        UpdateSubWeaponIcon();
    }
}

void UPlayerStatWeaponWidget::UnbindViewModel()
{
    if (WeaponViewModel && bIsBound)
    {
        WeaponViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerStatWeaponWidget::HandleWeaponFieldChanged);
        bIsBound = false;
    }
}

void UPlayerStatWeaponWidget::HandleWeaponFieldChanged(FName FieldName)
{
    if (FieldName == WeaponVMFields::MainWeaponData)
    {
        UpdateMainWeaponIcon();
        return;
    }

    if (FieldName == WeaponVMFields::SubWeaponData)
    {
        UpdateSubWeaponIcon();
    }
}
