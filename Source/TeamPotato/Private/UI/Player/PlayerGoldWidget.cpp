// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerGoldWidget.h"
#include "Components/TextBlock.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"

void UPlayerGoldWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BindViewModel();
}

void UPlayerGoldWidget::NativeDestruct()
{
    UnbindViewModel();

    Super::NativeDestruct();
}

void UPlayerGoldWidget::SetViewModel(UPlayerResourceViewModel* InViewModel)
{
    UnbindViewModel();
    PlayerResourceViewModel = InViewModel;
    BindViewModel();
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
