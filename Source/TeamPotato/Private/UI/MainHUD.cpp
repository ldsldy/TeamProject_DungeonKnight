// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"
#include "Player/TestPlayerController.h"
#include "UI/BaseLayerWidget.h"
#include "UI/Perk/PerkSelectionScreenWidget.h"
#include "UI/Player/MainHUDWidget.h"
#include "UI/UIWidgetLayerTags.h"

AMainHUD::AMainHUD()
{
    OpenPerkSelectionLayerTags.ActivateTags.AddTag(TeamPotatoGameplayTags::UI_Layer_PerkSelection);
    ClosePerkSelectionLayerTags.DeactivateTags.AddTag(TeamPotatoGameplayTags::UI_Layer_PerkSelection);
}

void AMainHUD::BeginPlay()
{
    Super::BeginPlay();

    MainHUDWidget = CreateWidget<UMainHUDWidget>(GetOwningPlayerController(), MainHUDWidgetClass);
    if (MainHUDWidget)
    {
        MainHUDWidget->AddToViewport();
    }

    TSubclassOf<UBaseLayerWidget> ResolvedBaseLayerClass = BaseLayerWidgetClass;
    if (!ResolvedBaseLayerClass)
    {
        ResolvedBaseLayerClass = UBaseLayerWidget::StaticClass();
    }

    BaseLayerWidget = CreateWidget<UBaseLayerWidget>(GetOwningPlayerController(), ResolvedBaseLayerClass);
    if (BaseLayerWidget)
    {
        BaseLayerWidget->AddToViewport(0);
        BaseLayerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }

    if (BaseLayerWidget && PerkSelectionScreenClass)
    {
        UPerkSelectionScreenWidget* PerkSelectionScreen = Cast<UPerkSelectionScreenWidget>(
            BaseLayerWidget->RegisterLayerWidgetClass(
                TeamPotatoGameplayTags::UI_Layer_PerkSelection,
                PerkSelectionScreenClass,
                5));

        if (PerkSelectionScreen)
        {
            PerkSelectionScreen->OnPerkSelected.AddDynamic(this, &AMainHUD::RemovePerkSelectionScreenFromViewport);
        }
    }
}

void AMainHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (BaseLayerWidget)
    {
        UPerkSelectionScreenWidget* PerkSelectionScreen = Cast<UPerkSelectionScreenWidget>(
            BaseLayerWidget->FindLayerWidget(TeamPotatoGameplayTags::UI_Layer_PerkSelection));

        if (PerkSelectionScreen)
        {
            PerkSelectionScreen->OnPerkSelected.RemoveDynamic(this, &AMainHUD::RemovePerkSelectionScreenFromViewport);
        }
    }

    Super::EndPlay(EndPlayReason);
}

void AMainHUD::TryPerkSelectionScreen(int32 InStage, int32 InChapter)
{
    if (InChapter == 2 || InChapter == 4)
    {
        AddPerkSelectionScreenToViewport();
    }
}

void AMainHUD::AddPerkSelectionScreenToViewport()
{
    if (!BaseLayerWidget)
    {
        return;
    }

    BaseLayerWidget->ApplyLayerActivation(OpenPerkSelectionLayerTags);

    if (ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwningPlayerController()))
    {
        PC->SetGameAndUIInputMode(BaseLayerWidget->FindLayerWidget(TeamPotatoGameplayTags::UI_Layer_PerkSelection));
    }
}

void AMainHUD::RemovePerkSelectionScreenFromViewport()
{
    if (BaseLayerWidget)
    {
        BaseLayerWidget->ApplyLayerActivation(ClosePerkSelectionLayerTags);
    }

    if (ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwningPlayerController()))
    {
        PC->SetGameOnlyInputMode();
    }
}
