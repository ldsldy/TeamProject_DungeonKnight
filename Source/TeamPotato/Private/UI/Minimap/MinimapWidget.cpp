// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Minimap/MinimapWidget.h"
#include "Components/Image.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/MinimapViewModel.h"
#include "Materials/MaterialInstanceDynamic.h"

void UMinimapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!MinimapViewModel)
    {
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (UMVVMSubsystem* Subsystem = GameInstance->GetSubsystem<UMVVMSubsystem>())
            {
                MinimapViewModel = Subsystem->GetMinimapViewModel();
            }
        }
    }

    BindViewModel();
}

void UMinimapWidget::NativeDestruct()
{
    UnbindViewModel();

    Super::NativeDestruct();
}

void UMinimapWidget::BindViewModel()
{
    if (MinimapViewModel && !bIsViewModelBound)
    {
        MinimapViewModel->OnMinimapInitialized.AddDynamic(
            this, &UMinimapWidget::HandleMinimapInitialized);

        bIsViewModelBound = true;

        HandleMinimapInitialized();
    }
}

void UMinimapWidget::UnbindViewModel()
{
    if (MinimapViewModel && bIsViewModelBound)
    {
        MinimapViewModel->OnMinimapInitialized.RemoveDynamic(
            this, &UMinimapWidget::HandleMinimapInitialized);

        bIsViewModelBound = false;
    }
}

void UMinimapWidget::HandleMinimapInitialized()
{
    if (MinimapViewModel)
    {
        UMaterialInstanceDynamic* MinimapMaterial = MinimapViewModel->GetMinimapMaterial();
        if (MinimapMaterial && MinimapImage)
        {
            UE_LOG(LogTemp, Warning, TEXT("MinimapWidget::HandleMinimapInitialized - Setting Minimap Material"));
            MinimapImage->SetBrushFromMaterial(MinimapMaterial);
        }
    }
}
