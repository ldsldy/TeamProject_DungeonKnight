// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPSCounterWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

UFPSCounterWidget::UFPSCounterWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , TextFormat(NSLOCTEXT("FPSCounterWidget", "FPSTextFormat", "FPS: {0}"))
{
}

TSharedRef<SWidget> UFPSCounterWidget::RebuildWidget()
{
    if (WidgetTree && !WidgetTree->RootWidget)
    {
        FPSText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("FPSText"));
        FPSText->SetJustification(ETextJustify::Left);
        FPSText->SetColorAndOpacity(FSlateColor(FLinearColor(0.0f, 1.0f, 0.15f, 1.0f)));
        FPSText->SetShadowColorAndOpacity(FLinearColor::Black);
        FPSText->SetShadowOffset(FVector2D(1.0f, 1.0f));

        WidgetTree->RootWidget = FPSText;
    }

    return Super::RebuildWidget();
}

void UFPSCounterWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetVisibility(ESlateVisibility::HitTestInvisible);
    UpdateFPSText();
}

void UFPSCounterWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (InDeltaTime <= 0.0f)
    {
        return;
    }

    AccumulatedTime += InDeltaTime;
    ++AccumulatedFrames;

    const float ClampedUpdateInterval = FMath::Max(UpdateInterval, 0.05f);
    if (AccumulatedTime >= ClampedUpdateInterval)
    {
        CurrentFPS = FMath::RoundToInt(static_cast<float>(AccumulatedFrames) / AccumulatedTime);
        AccumulatedTime = 0.0f;
        AccumulatedFrames = 0;

        UpdateFPSText();
    }
}

UTextBlock* UFPSCounterWidget::ResolveFPSText()
{
    if (FPSText)
    {
        return FPSText;
    }

    if (!WidgetTree)
    {
        return nullptr;
    }

    TArray<UWidget*> Widgets;
    WidgetTree->GetAllWidgets(Widgets);

    for (UWidget* Widget : Widgets)
    {
        if (Widget && Widget->GetFName() == TEXT("FPSText"))
        {
            FPSText = Cast<UTextBlock>(Widget);
            if (FPSText)
            {
                return FPSText;
            }
        }
    }

    for (UWidget* Widget : Widgets)
    {
        if (UTextBlock* TextBlock = Cast<UTextBlock>(Widget))
        {
            FPSText = TextBlock;
            return FPSText;
        }
    }

    return nullptr;
}

void UFPSCounterWidget::UpdateFPSText()
{
    if (UTextBlock* TextBlock = ResolveFPSText())
    {
        TextBlock->SetText(FText::Format(TextFormat, FText::AsNumber(CurrentFPS)));
    }
}
