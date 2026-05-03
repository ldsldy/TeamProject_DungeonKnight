// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Minimap/MinimapWidget.h"

#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Subsystem/MinimapSubsystem.h"

void UMinimapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!MinimapSubsystem)
    {
        if (UWorld* World = GetWorld())
        {
            MinimapSubsystem = World->GetSubsystem<UMinimapSubsystem>();
        }
    }

    BindMinimapSubsystem();
}

void UMinimapWidget::NativeDestruct()
{
    UnbindMinimapSubsystem();

    Super::NativeDestruct();
}

void UMinimapWidget::BindMinimapSubsystem()
{
    if (MinimapSubsystem && !bIsMinimapSubsystemBound)
    {
        MinimapSubsystem->OnMinimapInitialized.AddDynamic(this, &UMinimapWidget::HandleMinimapInitialized);
        bIsMinimapSubsystemBound = true;

        UpdateMinimapMaterial();
    }
}

void UMinimapWidget::UnbindMinimapSubsystem()
{
    if (MinimapSubsystem && bIsMinimapSubsystemBound)
    {
        MinimapSubsystem->OnMinimapInitialized.RemoveDynamic(this, &UMinimapWidget::HandleMinimapInitialized);
        bIsMinimapSubsystemBound = false;
    }
}

void UMinimapWidget::HandleMinimapInitialized()
{
    UpdateMinimapMaterial();
}

void UMinimapWidget::UpdateMinimapMaterial()
{
    if (!MinimapSubsystem || !MinimapImage || !MinimapSubsystem->IsInitialized())
    {
        return;
    }

    UMaterialInstanceDynamic* MinimapMaterial = MinimapSubsystem->GetMinimapMaterial();
    if (MinimapMaterial)
    {
        MinimapImage->SetBrushFromMaterial(MinimapMaterial);
    }
}
