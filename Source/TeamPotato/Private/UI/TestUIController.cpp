// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestUIController.h"

#include "UI/Minimap/MinimapWidget.h"

void ATestUIController::BeginPlay()
{
    Super::BeginPlay();

    if (MinimapWidgetClass)
    {
        MinimapWidget = CreateWidget<UMinimapWidget>(this, MinimapWidgetClass);
        if (MinimapWidget)
        {
            MinimapWidget->AddToViewport(10);
        }
    }
}
