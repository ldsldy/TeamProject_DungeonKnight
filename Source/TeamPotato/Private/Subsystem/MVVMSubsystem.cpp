// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MVVMSubsystem.h"
#include "Component/PerkComponent.h"
#include "Component/PlayerResource.h"
#include "Component/WeaponComponent.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Subsystem/ViewModel/PerkViewModel.h"
#include "Subsystem/ViewModel/WeaponViewModel.h"
#include "Subsystem/ViewModel/MinimapViewModel.h"
#include "Subsystem/ViewModel/EnemyViewModel.h"
#include "Enemy/BossBase.h"

void UMVVMSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UMVVMSubsystem::Deinitialize()
{
    if (PlayerStatusViewModel)
    {
        PlayerStatusViewModel->Deinitialize();
    }

    if (PerkViewModel)
    {
        PerkViewModel->Deinitialize();
    }

    if (WeaponViewModel)
    {
        WeaponViewModel->Deinitialize();
    }

    if (MinimapViewModel)
    {
        MinimapViewModel->Deinitialize();
    }

    if (EnemyViewModel)
    {
        EnemyViewModel->Deinitialize();
    }

    RegisteredPlayerResourceComp = nullptr;
    RegisteredPerkComp = nullptr;
    RegisteredWeaponComp = nullptr;
    RegisteredBossActor = nullptr;

    Super::Deinitialize();
}


UPlayerResourceViewModel* UMVVMSubsystem::GetPlayerStatusViewModel()
{
    if (!PlayerStatusViewModel)
    {
        PlayerStatusViewModel = NewObject<UPlayerResourceViewModel>(this);
    }
    return PlayerStatusViewModel;
}

UPerkViewModel* UMVVMSubsystem::GetPerkViewModel()
{
    if (!PerkViewModel)
    {
        PerkViewModel = NewObject<UPerkViewModel>(this);
    }
    return PerkViewModel;
}

UWeaponViewModel* UMVVMSubsystem::GetWeaponViewModel()
{
    if (!WeaponViewModel)
    {
        WeaponViewModel = NewObject<UWeaponViewModel>(this);
    }
    return WeaponViewModel;
}

UMinimapViewModel* UMVVMSubsystem::GetMinimapViewModel()
{
    if (!MinimapViewModel)
    {
        MinimapViewModel = NewObject<UMinimapViewModel>(this);
    }
    return MinimapViewModel;
}

UEnemyViewModel* UMVVMSubsystem::GetEnemyViewModel()
{
    if (!EnemyViewModel)
    {
        EnemyViewModel = NewObject<UEnemyViewModel>(this);
    }
    return EnemyViewModel;
}


void UMVVMSubsystem::RegisterPlayerResourceComp(UPlayerResource* NewComp)
{
    if (!NewComp)
    {
        return;
    }

    if (RegisteredPlayerResourceComp && RegisteredPlayerResourceComp != NewComp)
    {
        UnregisterPlayerResourceComp(RegisteredPlayerResourceComp);
    }

    RegisteredPlayerResourceComp = NewComp;
    GetPlayerStatusViewModel()->Initialize(NewComp);
}

void UMVVMSubsystem::UnregisterPlayerResourceComp(UPlayerResource* ExitingComp)
{
    if (RegisteredPlayerResourceComp != ExitingComp)
    {
        return;
    }

    if (PlayerStatusViewModel)
    {
        PlayerStatusViewModel->Deinitialize();
    }

    RegisteredPlayerResourceComp = nullptr;
}

void UMVVMSubsystem::RegisterPerkComp(UPerkComponent* NewComp)
{
    if (!NewComp)
    {
        return;
    }

    if (RegisteredPerkComp && RegisteredPerkComp != NewComp)
    {
        UnregisterPerkComp(RegisteredPerkComp);
    }

    RegisteredPerkComp = NewComp;
    GetPerkViewModel()->Initialize(NewComp);
}

void UMVVMSubsystem::UnregisterPerkComp(UPerkComponent* ExitingComp)
{
    if (RegisteredPerkComp != ExitingComp)
    {
        return;
    }

    if (PerkViewModel)
    {
        PerkViewModel->Deinitialize();
    }

    RegisteredPerkComp = nullptr;
}

void UMVVMSubsystem::RegisterWeaponComp(UWeaponComponent* NewComp)
{
    if (!NewComp)
    {
        return;
    }

    if (RegisteredWeaponComp && RegisteredWeaponComp != NewComp)
    {
        UnregisterWeaponComp(RegisteredWeaponComp);
    }

    RegisteredWeaponComp = NewComp;
    GetWeaponViewModel()->Initialize(NewComp);
}

void UMVVMSubsystem::UnregisterWeaponComp(UWeaponComponent* ExitingComp)
{
    if (RegisteredWeaponComp != ExitingComp)
    {
        return;
    }

    if (WeaponViewModel)
    {
        WeaponViewModel->Deinitialize();
    }

    RegisteredWeaponComp = nullptr;
}

void UMVVMSubsystem::RegisterBossActor(ABossBase* NewActor)
{
    if (!NewActor)
    {
        return;
    }

    if (RegisteredBossActor && RegisteredBossActor != NewActor)
    {
        UnregisterBossActor(RegisteredBossActor);
    }

    RegisteredBossActor = NewActor;
    GetEnemyViewModel()->Initialize(NewActor);
}

void UMVVMSubsystem::UnregisterBossActor(ABossBase* ExitingActor)
{
    if (RegisteredBossActor != ExitingActor)
    {
        return;
    }

    if (EnemyViewModel)
    {
        EnemyViewModel->Deinitialize();
    }

    RegisteredBossActor = nullptr;
}
