// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ViewModel/WeaponViewModel.h"
#include "Component/WeaponComponent.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"

void UWeaponViewModel::Initialize(UObject* InModel)
{
    Deinitialize();

    Super::Initialize(InModel);

    Model = Cast<UWeaponComponent>(InModel);
    if (!Model)
    {
        return;
    }

    Model->OnMainWeaponChanged.AddDynamic(this, &UWeaponViewModel::HandleMainWeaponChanged);
    Model->OnSubWeaponChanged.AddDynamic(this, &UWeaponViewModel::HandleSubWeaponChanged);

    HandleMainWeaponChanged(Model->GetMainWeaponData());
    HandleSubWeaponChanged(Model->GetSubWeaponData());
}

void UWeaponViewModel::Deinitialize()
{
    if (Model)
    {
        Model->OnMainWeaponChanged.RemoveDynamic(this, &UWeaponViewModel::HandleMainWeaponChanged);
        Model->OnSubWeaponChanged.RemoveDynamic(this, &UWeaponViewModel::HandleSubWeaponChanged);
        Model = nullptr;
    }

    Super::Deinitialize();
}

void UWeaponViewModel::HandleMainWeaponChanged(UWeaponDataAsset* InWeaponData)
{
    SetAndNotify(MainWeaponData, InWeaponData, WeaponVMFields::MainWeaponData);
}

void UWeaponViewModel::HandleSubWeaponChanged(UWeaponDataAsset* InWeaponData)
{
    SetAndNotify(SubWeaponData, InWeaponData, WeaponVMFields::SubWeaponData);
}
