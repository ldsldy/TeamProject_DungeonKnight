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
    if (!Subsystem) return;

    UPlayerResourceViewModel* ResourceViewModel = Subsystem->GetPlayerStatusViewModel();

    // 위젯들에 뷰모델 세팅(이제 위젯은 뷰모델을 알게 된다)
    if (PlayerStatPanel)
    {
        UE_LOG(LogTemp, Warning, TEXT("UMainHUDWidget::InitializeViewModels - Setting PlayerStatPanel ViewModel"));
        PlayerStatPanel->SetViewModel(ResourceViewModel);
    }
    if (InventoryPerkPanel)
    {
        InventoryPerkPanel->SetViewModel(Subsystem->GetPerkViewModel());
    }
    if (PlayerWeaponPanel)
    {
        PlayerWeaponPanel->SetViewModel(Subsystem->GetWeaponViewModel(), ResourceViewModel);
    }
    if (PlayerGoldPanel)
    {
        PlayerGoldPanel->SetViewModel(ResourceViewModel);
    }
    if (BossWidget)
    {
        BossWidget->SetViewModel(Subsystem->GetEnemyViewModel());
        BossWidget->SetVisibility(ESlateVisibility::Collapsed); // 초기에는 숨김
    }
}

void UMainHUDWidget::ShowBossWidget(UMVVMSubsystem* Subsystem)
{
    if (BossWidget)
    {
        BossWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void UMainHUDWidget::HideBossWidget(UMVVMSubsystem* Subsystem)
{
    if (BossWidget)
    {
        BossWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}
