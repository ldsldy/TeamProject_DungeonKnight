// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"
#include "Player/TestPlayerController.h"
#include "UI/Perk/PerkSelectionScreenWidget.h"
#include "UI/Player/MainHUDWidget.h"

void AMainHUD::BeginPlay()
{
    Super::BeginPlay();

    MainHUDWidget = CreateWidget<UMainHUDWidget>(GetOwningPlayerController(), MainHUDWidgetClass);
    if (MainHUDWidget)
    {
        MainHUDWidget->AddToViewport();
    }

    if (PerkSelectionScreenClass)
    {
        PerkSelectionScreenWidget = CreateWidget<UPerkSelectionScreenWidget>(
            GetOwningPlayerController(),
            PerkSelectionScreenClass);

        if (PerkSelectionScreenWidget)
        {
            PerkSelectionScreenWidget->AddToViewport(5);
            PerkSelectionScreenWidget->SetVisibility(ESlateVisibility::Collapsed);
            PerkSelectionScreenWidget->OnPerkSelected.AddDynamic(this, &AMainHUD::RemovePerkSelectionScreenFromViewport);
        }
    }
}

void AMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (PerkSelectionScreenWidget)
    {
        PerkSelectionScreenWidget->OnPerkSelected.RemoveDynamic(this, &AMainHUD::RemovePerkSelectionScreenFromViewport);
    }

    Super::EndPlay(EndPlayReason);
}

void AMainHUD::TryPerkSelectionScreen(int32 InStage, int32 InChapter)
{
    if (InChapter == 1 || InChapter == 3)
    {
        AddPerkSelectionScreenToViewport();
    }
}

void AMainHUD::AddPerkSelectionScreenToViewport()
{
    if (!PerkSelectionScreenWidget)
    {
        return;
    }

    PerkSelectionScreenWidget->SetVisibility(ESlateVisibility::Visible);

    if (ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwningPlayerController()))
    {
        PC->SetGameAndUIInputMode(PerkSelectionScreenWidget);
    }
}

void AMainHUD::RemovePerkSelectionScreenFromViewport()
{
    if (PerkSelectionScreenWidget)
    {
        PerkSelectionScreenWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwningPlayerController()))
    {
        PC->SetGameOnlyInputMode();
    }
}
