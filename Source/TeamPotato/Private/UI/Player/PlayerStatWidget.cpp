// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/PlayerStatWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/PlayerResourceViewModel.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "TimerManager.h"

void UPlayerStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (!PlayerStatusViewModel)
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
            {
                PlayerStatusViewModel = Subsystem->GetPlayerStatusViewModel();
            }
        }
    }

    BindViewModel();
}

void UPlayerStatWidget::NativeDestruct()
{
    UnbindViewModel();

	Super::NativeDestruct();
}

void  UPlayerStatWidget::BindViewModel()
{
    // 뷰모델의 델리게이트에 바인딩
    if (PlayerStatusViewModel && !bIsViewModelBound)
    {
        PlayerStatusViewModel->OnFieldChanged.AddDynamic(this, &UPlayerStatWidget::HandleViewModelFieldChanged);

        bIsViewModelBound = true;
        RefreshHealthUI();
    }
}

void  UPlayerStatWidget::UnbindViewModel()
{
    if (PlayerStatusViewModel && bIsViewModelBound)
    {
        PlayerStatusViewModel->OnFieldChanged.RemoveDynamic(this, &UPlayerStatWidget::HandleViewModelFieldChanged);

        bIsViewModelBound = false;
    }
}

void UPlayerStatWidget::HandleViewModelFieldChanged(FName FieldName)
{
    if (FieldName == PlayerResourceVMFields::HealthCurrent
        || FieldName == PlayerResourceVMFields::HealthMax
        || FieldName == PlayerResourceVMFields::HealthPercent)
    {
        RefreshHealthUI();
    }
}

void UPlayerStatWidget::RefreshHealthUI()
{
    if (!PlayerStatusViewModel)
    {
        return;
    }

    CurrentHealthPercent = PlayerStatusViewModel->GetHealthPercent();

    if (PlayerHealthBar)
    {
        PlayerHealthBar->SetPercent(CurrentHealthPercent);
    }

    if (HealthText)
    {
        const int32 CurrentHealth = FMath::RoundToInt(PlayerStatusViewModel->GetCurrentHealth());
        const int32 MaxHealth = FMath::RoundToInt(PlayerStatusViewModel->GetMaxHealth());
        HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentHealth, MaxHealth)));
    }

    if (CurrentHealthPercent < DelayBarPercent)
    {
        GetWorld()->GetTimerManager().ClearTimer(DelayBarTimerHandle);
        GetWorld()->GetTimerManager().ClearTimer(DelayBarWaitTimerHandle);

        // 딜레이 웨이트 타임이 지난 후에 딜레이 바 업데이트 타이머 시작
        GetWorld()->GetTimerManager().SetTimer(DelayBarWaitTimerHandle, this, &UPlayerStatWidget::StartDelayBarTimer, DelayBarWaitTime, false);
    }
    else if (CurrentHealthPercent >= DelayBarPercent)
    {
        // 현재 체력이 딜레이 바보다 크거나 같으면 즉시 동기화
        DelayBarPercent = CurrentHealthPercent;
        if (DelayProgressBar)
        {
            DelayProgressBar->SetPercent(DelayBarPercent);
        }
    }
}

void UPlayerStatWidget::StartDelayBarTimer()
{
    InterpAlpha = 0.0f;
    // 딜레이 바 업데이트 반복 실행
    GetWorld()->GetTimerManager().SetTimer(DelayBarTimerHandle, this, &UPlayerStatWidget::UpdatePlayerDelayBar, DelayBarInterval, true);
}

void UPlayerStatWidget::UpdatePlayerDelayBar()
{
    //UE_LOG(LogTemp, Warning, TEXT("UPlayerStatWidget::UpdatePlayerDelayBar called,"));
    //UE_LOG(LogTemp, Warning, TEXT("InterpAlpha = %f, CurrentHealthPercent = %f, DelayBarPercent = %f"), InterpAlpha, CurrentHealthPercent, DelayBarPercent);
    if (!DelayBarHealthCurve)
    {
        UE_LOG(LogTemp, Warning, TEXT("DelayBarHealthCurve is null!"));
    }
    if (InterpAlpha < 1.0f && DelayBarHealthCurve)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Updating Delay Bar: InterpAlpha = %f"), InterpAlpha);
        // 커브를 사용하여 보간
        InterpAlpha += DelayBarInterval / InterpDuration;
        InterpAlpha = FMath::Clamp(InterpAlpha, 0.0f, 1.0f);

        float CurveValue = DelayBarHealthCurve->GetFloatValue(InterpAlpha);
        DelayBarPercent = FMath::Lerp(DelayBarPercent, CurrentHealthPercent, CurveValue);

        DelayProgressBar->SetPercent(DelayBarPercent);
    }
    else
    {
        // 딜레이 바가 현재 체력에 도달했으므로 타이머 정지
        GetWorld()->GetTimerManager().ClearTimer(DelayBarTimerHandle);
    }
}

