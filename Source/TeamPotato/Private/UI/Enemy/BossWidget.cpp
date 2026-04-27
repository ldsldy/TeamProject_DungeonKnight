// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/BossWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/EnemyViewModel.h"
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


void  UBossWidget::BindViewModel()
{
    // 뷰모델의 델리게이트에 바인딩
    if (EnemyViewModel && !bIsViewModelBound)
    {
        // 모델 -> 뷰
        EnemyViewModel->UpdateBossHealth.AddDynamic(this, &UBossWidget::SetBossHealthUI);

        bIsViewModelBound = true;
    }
}

void  UBossWidget::UnbindViewModel()
{
    if (EnemyViewModel && bIsViewModelBound)
    {
        EnemyViewModel->UpdateBossHealth.RemoveDynamic(this, &UBossWidget::SetBossHealthUI);

        bIsViewModelBound = false;
    }
}

void UBossWidget::SetBossHealthUI(float CurrentHealth, float MaxHealth)
{
    if (BossCurrentHealthText)
    {
        BossCurrentHealthText->SetText(FText::AsNumber(FMath::RoundToInt(CurrentHealth)));
    }

    if (BossMaxHealthText)
    {
        BossMaxHealthText->SetText(FText::AsNumber(FMath::RoundToInt(MaxHealth)));
    }

    // 체력바는 즉시 업데이트
    if (BossHealthBar)
    {
        CurrentHealthPercent = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
        BossHealthBar->SetPercent(CurrentHealthPercent);
    }

    if (CurrentHealthPercent < DelayBarPercent)
    {
        GetWorld()->GetTimerManager().ClearTimer(DelayBarTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(DelayBarWaitTimerHandle);

        // 딜레이 웨이트 타임이 지난 후에 딜레이 바 업데이트 타이머 시작
        GetWorld()->GetTimerManager().SetTimer(DelayBarWaitTimerHandle, this, &UBossWidget::StartDelayBarTimer, DelayBarWaitTime, false);
    }
    else if (CurrentHealthPercent >= DelayBarPercent)
    {
        // 현재 체력이 딜레이 바보다 크거나 같으면 즉시 동기화
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
    // 딜레이 바 업데이트 반복 실행
    GetWorld()->GetTimerManager().SetTimer(DelayBarTimerHandle, this, &UBossWidget::UpdateDelayBar, DelayBarInterval, true);
}

void UBossWidget::UpdateDelayBar()
{
    if (InterpAlpha < 1.0f && DelayBarCurve)
    {
        // 커브를 사용하여 보간
        InterpAlpha += DelayBarInterval / InterpDuration;
        InterpAlpha = FMath::Clamp(InterpAlpha, 0.0f, 1.0f);
        
        float CurveValue = DelayBarCurve->GetFloatValue(InterpAlpha);
        DelayBarPercent = FMath::Lerp(DelayBarPercent, CurrentHealthPercent, CurveValue);

        BossHealthDelayBar->SetPercent(DelayBarPercent);
    }
    else
    {
        // 딜레이 바가 현재 체력에 도달했으므로 타이머 정지
        GetWorld()->GetTimerManager().ClearTimer(DelayBarTimerHandle);
    }
}
