// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerGoldWidget.h"
#include "Components/TextBlock.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"

void UPlayerGoldWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!PlayerResourceViewModel)
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
            {
                PlayerResourceViewModel = Subsystem->GetPlayerStatusViewModel();
            }
        }
    }

    BindViewModel();
}

void UPlayerGoldWidget::NativeDestruct()
{
    UnbindViewModel();

    Super::NativeDestruct();
}

void UPlayerGoldWidget::BindViewModel()
{
    if (PlayerResourceViewModel && !bIsViewModelBound)
    {
        PlayerResourceViewModel->OnFieldChanged.AddDynamic(this, &UPlayerGoldWidget::HandleViewModelFieldChanged);

        bIsViewModelBound = true;
        UpdatePlayerGold();
    }
}


void UPlayerGoldWidget::UnbindViewModel()
{
    if (PlayerResourceViewModel && bIsViewModelBound)
    {
        PlayerResourceViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerGoldWidget::HandleViewModelFieldChanged);

        bIsViewModelBound = false;
    }
}

void UPlayerGoldWidget::HandleViewModelFieldChanged(FName FieldName)
{
    if (FieldName == PlayerResourceVMFields::Gold)
    {
        UpdatePlayerGold();
    }
}

void UPlayerGoldWidget::UpdatePlayerGold()
{
    if (PlayerGoldText && PlayerResourceViewModel)
    {
        PlayerGoldText->SetText(FText::AsNumber(PlayerResourceViewModel->GetCurrentGold()));
    }
}
