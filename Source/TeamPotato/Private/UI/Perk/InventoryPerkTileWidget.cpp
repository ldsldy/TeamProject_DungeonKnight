// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Perk/InventoryPerkTileWidget.h"
#include "Components/TileView.h"
#include "Data/Object/PerkDataObject.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "Subsystem/ViewModel/PerkViewModel.h"

void UInventoryPerkTileWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!PerkViewModel)
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
            {
                PerkViewModel = Subsystem->GetPerkViewModel();
            }
        }
    }

    if (PerkTileView)
    {
        PerkTileView->ClearListItems();
        PerkTileView->OnItemIsHoveredChanged().AddUObject(this, &UInventoryPerkTileWidget::OnPerkitemHoveredChanged);
    }

    BindViewModel();
}

void UInventoryPerkTileWidget::NativeDestruct()
{
    if (PerkTileView)
    {
        PerkTileView->OnItemIsHoveredChanged().RemoveAll(this);
    }

    UnbindViewModel();

    Super::NativeDestruct();
}

void UInventoryPerkTileWidget::BindViewModel()
{
    if (PerkViewModel && PerkTileView)
    {
        PerkViewModel->OnFieldChanged.AddDynamic(this, &UInventoryPerkTileWidget::HandlePerkFieldChanged);
        RefreshPerkTileList();
    }
}

void UInventoryPerkTileWidget::UnbindViewModel()
{
    if (PerkViewModel)
    {
        PerkViewModel->OnFieldChanged.RemoveDynamic(this, &UInventoryPerkTileWidget::HandlePerkFieldChanged);
    }
}

void UInventoryPerkTileWidget::HandlePerkFieldChanged(FName FieldName)
{
    if (FieldName == PerkVMFields::EquippedPerks)
    {
        RefreshPerkTileList();
    }
}

void UInventoryPerkTileWidget::RefreshPerkTileList()
{
    if (!PerkTileView || !PerkViewModel)
    {
        return;
    }

    PerkTileView->ClearListItems();

    const TArray<TObjectPtr<UPerkDataAsset>>& EquippedPerks = PerkViewModel->GetEquippedPerks();
    for (UPerkDataAsset* PerkData : EquippedPerks)
    {
        LoadPerkDataFromDataAsset(PerkData);
    }
}

void UInventoryPerkTileWidget::LoadPerkDataFromDataAsset(UPerkDataAsset* InData)
{
    if (!InData || !PerkTileView)
    {
        return;
    }

    UPerkDataObject* PerDataObject = NewObject<UPerkDataObject>(this);
    PerDataObject->InitFromDataTableAsset(InData);
    PerkTileView->AddItem(PerDataObject);
}

void UInventoryPerkTileWidget::OnPerkitemHoveredChanged(UObject* Item, bool bIsHovered)
{
    if (Item)
    {
        // Hover state can be extended later.
    }

    if (bIsHovered)
    {
        // Hover state can be extended later.
    }
}
