// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ViewModel/PerkViewModel.h"
#include "Component/PerkComponent.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"

void UPerkViewModel::Initialize(UObject* InModel)
{
    Deinitialize();

    Super::Initialize(InModel);

    Model = Cast<UPerkComponent>(InModel);
    if (!Model)
    {
        return;
    }

    Model->OnEquipmentUpdated.AddDynamic(this, &UPerkViewModel::SetPerkDataAsset);
    Model->OnPerkEquipmentCleared.AddDynamic(this, &UPerkViewModel::HandleModelPerkCleared);

    RefreshCachedPerksFromModel();
}

void UPerkViewModel::Deinitialize()
{
    if (Model)
    {
        Model->OnEquipmentUpdated.RemoveDynamic(this, &UPerkViewModel::SetPerkDataAsset);
        Model->OnPerkEquipmentCleared.RemoveDynamic(this, &UPerkViewModel::HandleModelPerkCleared);
        Model = nullptr;
    }

    Super::Deinitialize();
}

void UPerkViewModel::SetPerkDataAsset(UPerkDataAsset* NewData)
{
    if (!NewData)
    {
        return;
    }

    if (!CachedEquippedPerks.Contains(NewData))
    {
        CachedEquippedPerks.Add(NewData);
    }

    OnPerkEquipped.Broadcast(NewData);
    OnFieldChanged.Broadcast(PerkVMFields::EquippedPerks);
}

bool UPerkViewModel::RequestEquipPerk(UPerkDataAsset* NewData, int32 SlotIndex)
{
    if (!NewData || !Model)
    {
        return false;
    }

    return Model->EquipPerk(NewData, SlotIndex);
}

void UPerkViewModel::RequestClearAllPerks()
{
    if (Model)
    {
        Model->ClearAllEquippedPerks();
        return;
    }

    HandleModelPerkCleared();
}

UPerkDataAsset* UPerkViewModel::GetEquippedPerkAt(int32 Index) const
{
    if (CachedEquippedPerks.IsValidIndex(Index))
    {
        return CachedEquippedPerks[Index];
    }

    return nullptr;
}

void UPerkViewModel::HandleModelPerkCleared()
{
    CachedEquippedPerks.Empty();

    OnTryAllPerkClear.Broadcast();
    OnFieldChanged.Broadcast(PerkVMFields::EquippedPerks);
}

void UPerkViewModel::RefreshCachedPerksFromModel()
{
    CachedEquippedPerks.Empty();

    if (!Model)
    {
        return;
    }

    const TArray<UPerkDataAsset*>& ModelPerks = Model->GetEquippedPerks();
    for (UPerkDataAsset* PerkData : ModelPerks)
    {
        if (PerkData && !CachedEquippedPerks.Contains(PerkData))
        {
            CachedEquippedPerks.Add(PerkData);
        }
    }

    OnFieldChanged.Broadcast(PerkVMFields::EquippedPerks);
}
