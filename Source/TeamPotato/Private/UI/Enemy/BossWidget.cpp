// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/BossWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/EnemyViewModel.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "TimerManager.h"

void UBossWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!EnemyViewModel)
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
            {
                EnemyViewModel = Subsystem->GetEnemyViewModel();
            }
        }
    }

    BindViewModel();
}

void UBossWidget::NativeDestruct()
{
    UnbindViewModel();

    Super::NativeDestruct();
}

void UBossWidget::BindViewModel()
{
    if (EnemyViewModel && !bIsViewModelBound)
    {
        EnemyViewModel->OnFieldChanged.AddDynamic(this, &UBossWidget::HandleEnemyFieldChanged);

        bIsViewModelBound = true;
        UpdateBossVisibility();
        UpdateBossHealthUI();
    }
}

void UBossWidget::UnbindViewModel()
{
    if (EnemyViewModel && bIsViewModelBound)
    {
        EnemyViewModel->OnFieldChanged.RemoveDynamic(this, &UBossWidget::HandleEnemyFieldChanged);
        bIsViewModelBound = false;
    }
}

void UBossWidget::HandleEnemyFieldChanged(FName FieldName)
{
    if (FieldName == EnemyVMFields::CurrentHealth
        || FieldName == EnemyVMFields::MaxHealth
        || FieldName == EnemyVMFields::HealthPercent)
    {
        UpdateBossHealthUI();
        return;
    }

    if (FieldName == EnemyVMFields::IsVisible)
    {
        UpdateBossVisibility();
    }
}

void UBossWidget::UpdateBossVisibility()
{
    if (!EnemyViewModel)
    {
        return;
    }

    SetVisibility(EnemyViewModel->IsBossVisible() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UBossWidget::UpdateBossHealthUI()
{
    if (!EnemyViewModel)
    {
        return;
    }

    const float CurrentHealth = EnemyViewModel->GetCurrentHealth();
    const float MaxHealth = FMath::Max(1.0f, EnemyViewModel->GetMaxHealth());

    if (BossCurrentHealthText)
    {
        BossCurrentHealthText->SetText(FText::AsNumber(FMath::RoundToInt(CurrentHealth)));
    }

    if (BossMaxHealthText)
    {
        BossMaxHealthText->SetText(FText::AsNumber(FMath::RoundToInt(MaxHealth)));
    }

    if (BossHealthBar)
    {
        CurrentHealthPercent = EnemyViewModel->GetHealthPercent();
        BossHealthBar->SetPercent(CurrentHealthPercent);
    }

    if (CurrentHealthPercent < DelayBarPercent)
    {
        GetWorld()->GetTimerManager().ClearTimer(DelayBarTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(DelayBarWaitTimerHandle);

        GetWorld()->GetTimerManager().SetTimer(
            DelayBarWaitTimerHandle,
            this,
            &UBossWidget::StartDelayBarTimer,
            DelayBarWaitTime,
            false);
    }
    else if (CurrentHealthPercent >= DelayBarPercent)
    {
        DelayBarPercent = CurrentHealthPercent;
        if (BossHealthDelayBar)
        {
            BossHealthDelayBar->SetPercent(DelayBarPercent);
        }
    }
}

void UBossWidget::StartDelayBarTimer()
{
    InterpAlpha = 0.0f;
    GetWorld()->GetTimerManager().SetTimer(DelayBarTimerHandle, this, &UBossWidget::UpdateDelayBar, DelayBarInterval, true);
}

void UBossWidget::UpdateDelayBar()
{
    if (InterpAlpha < 1.0f && DelayBarCurve)
    {
        InterpAlpha += DelayBarInterval / InterpDuration;
        InterpAlpha = FMath::Clamp(InterpAlpha, 0.0f, 1.0f);

        const float CurveValue = DelayBarCurve->GetFloatValue(InterpAlpha);
        DelayBarPercent = FMath::Lerp(DelayBarPercent, CurrentHealthPercent, CurveValue);

        if (BossHealthDelayBar)
        {
            BossHealthDelayBar->SetPercent(DelayBarPercent);
        }
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(DelayBarTimerHandle);
    }
}
