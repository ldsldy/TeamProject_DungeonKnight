// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseLayerWidget.h"
#include "GameFramework/PlayerController.h"

UUserWidget* UBaseLayerWidget::RegisterLayerWidgetClass(const FGameplayTag LayerTag, TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
    if (!LayerTag.IsValid() || !WidgetClass)
    {
        return nullptr;
    }

    if (const FLayerWidgetEntry* ExistingEntry = LayerWidgetMap.Find(LayerTag))
    {
        return ExistingEntry->Widget;
    }

    APlayerController* OwningPlayerController = GetOwningPlayer();
    if (!OwningPlayerController)
    {
        return nullptr;
    }

    UUserWidget* NewWidget = CreateWidget<UUserWidget>(OwningPlayerController, WidgetClass);
    if (!NewWidget)
    {
        return nullptr;
    }

    RegisterLayerWidget(LayerTag, NewWidget, ZOrder);
    return NewWidget;
}

void UBaseLayerWidget::RegisterLayerWidget(const FGameplayTag LayerTag, UUserWidget* WidgetInstance, int32 ZOrder)
{
    if (!LayerTag.IsValid() || !WidgetInstance)
    {
        return;
    }

    FLayerWidgetEntry& LayerWidgetEntry = LayerWidgetMap.FindOrAdd(LayerTag);
    LayerWidgetEntry.Widget = WidgetInstance;
    LayerWidgetEntry.ZOrder = ZOrder;

    if (!WidgetInstance->IsInViewport())
    {
        WidgetInstance->AddToViewport(ZOrder);
    }

    WidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
}

bool UBaseLayerWidget::ActivateLayerWidget(const FGameplayTag LayerTag)
{
    FLayerWidgetEntry* LayerWidgetEntry = LayerWidgetMap.Find(LayerTag);
    if (!LayerWidgetEntry || !LayerWidgetEntry->Widget)
    {
        return false;
    }

    if (!LayerWidgetEntry->Widget->IsInViewport())
    {
        LayerWidgetEntry->Widget->AddToViewport(LayerWidgetEntry->ZOrder);
    }

    LayerWidgetEntry->Widget->SetVisibility(ESlateVisibility::Visible);
    return true;
}

bool UBaseLayerWidget::DeactivateLayerWidget(const FGameplayTag LayerTag)
{
    FLayerWidgetEntry* LayerWidgetEntry = LayerWidgetMap.Find(LayerTag);
    if (!LayerWidgetEntry || !LayerWidgetEntry->Widget)
    {
        return false;
    }

    LayerWidgetEntry->Widget->SetVisibility(ESlateVisibility::Collapsed);
    return true;
}

void UBaseLayerWidget::ApplyLayerActivation(const FLayerWidgetActivationContainer& ActivationContainer)
{
    TArray<FGameplayTag> TagsToDeactivate;
    ActivationContainer.DeactivateTags.GetGameplayTagArray(TagsToDeactivate);
    for (const FGameplayTag& Tag : TagsToDeactivate)
    {
        DeactivateLayerWidget(Tag);
    }

    TArray<FGameplayTag> TagsToActivate;
    ActivationContainer.ActivateTags.GetGameplayTagArray(TagsToActivate);
    for (const FGameplayTag& Tag : TagsToActivate)
    {
        ActivateLayerWidget(Tag);
    }
}

UUserWidget* UBaseLayerWidget::FindLayerWidget(const FGameplayTag LayerTag) const
{
    if (const FLayerWidgetEntry* LayerWidgetEntry = LayerWidgetMap.Find(LayerTag))
    {
        return LayerWidgetEntry->Widget;
    }

    return nullptr;
}
