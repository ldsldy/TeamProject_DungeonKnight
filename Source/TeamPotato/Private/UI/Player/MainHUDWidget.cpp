// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/MainHUDWidget.h"
#include "Subsystem/MVVMSubsystem.h"
#include "UI/Player/PlayerStatWidget.h"
#include "UI/Perk/InventoryPerkTileWidget.h"
#include "UI/Player/PlayerWeaponWidget.h"
#include "UI/Player/PlayerGoldWidget.h"
#include "UI/Enemy/BossWidget.h"

void UMainHUDWidget::InitializeViewModels(UMVVMSubsystem* Subsystem)
{
    // 각 자식 위젯이 NativeConstruct에서 MVVMSubsystem으로 직접 바인딩한다.
    (void)Subsystem;

    if (BossWidget)
    {
        BossWidget->SetVisibility(ESlateVisibility::Collapsed); // 초기에는 숨김
    }
}

void UMainHUDWidget::ShowBossWidget(UMVVMSubsystem* Subsystem)
{
    (void)Subsystem;

    if (BossWidget)
    {
        BossWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMainHUDWidget::HideBossWidget(UMVVMSubsystem* Subsystem)
{
    (void)Subsystem;

    if (BossWidget)
    {
        BossWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}
