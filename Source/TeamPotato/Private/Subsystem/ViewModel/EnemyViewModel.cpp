// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ViewModel/EnemyViewModel.h"
#include "Enemy/BossBase.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"

void UEnemyViewModel::Initialize(UObject* InModel)
{
    Deinitialize();

    Super::Initialize(InModel);

    Model = Cast<ABossBase>(InModel);
    if (!Model)
    {
        return;
    }

    Model->OnBossHealthChanged.AddDynamic(this, &UEnemyViewModel::HandleBossHealthChanged);
    Model->OnBossSpawn.AddDynamic(this, &UEnemyViewModel::HandleBossSpawned);
    Model->OnBossDie.AddDynamic(this, &UEnemyViewModel::HandleBossDied);

    HandleBossSpawned();
    HandleBossHealthChanged(Model->GetCurrentHealthValue(), Model->GetMaxHealthValue());
}

void UEnemyViewModel::Deinitialize()
{
    if (Model)
    {
        Model->OnBossHealthChanged.RemoveDynamic(this, &UEnemyViewModel::HandleBossHealthChanged);
        Model->OnBossSpawn.RemoveDynamic(this, &UEnemyViewModel::HandleBossSpawned);
        Model->OnBossDie.RemoveDynamic(this, &UEnemyViewModel::HandleBossDied);
        Model = nullptr;
    }

    Super::Deinitialize();
}

void UEnemyViewModel::HandleBossHealthChanged(float InCurrentHealth, float InMaxHealth)
{
    SetAndNotifyFloat(CurrentHealth, InCurrentHealth, EnemyVMFields::CurrentHealth);
    SetAndNotifyFloat(MaxHealth, FMath::Max(1.0f, InMaxHealth), EnemyVMFields::MaxHealth);
    UpdateHealthPercent();
}

void UEnemyViewModel::HandleBossSpawned()
{
    SetAndNotify(bIsVisible, true, EnemyVMFields::IsVisible);
}

void UEnemyViewModel::HandleBossDied()
{
    SetAndNotify(bIsVisible, false, EnemyVMFields::IsVisible);
}

void UEnemyViewModel::UpdateHealthPercent()
{
    const float SafeMax = FMath::Max(1.0f, MaxHealth);
    const float NewPercent = FMath::Clamp(CurrentHealth / SafeMax, 0.0f, 1.0f);

    SetAndNotifyFloat(HealthPercent, NewPercent, EnemyVMFields::HealthPercent);
}
