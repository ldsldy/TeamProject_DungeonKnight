// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Minimap/MinimapWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "Subsystem/ViewModel/MinimapViewModel.h"

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
        MinimapViewModel->OnFieldChanged.AddDynamic(this, &UMinimapWidget::HandleViewModelFieldChanged);
        bIsViewModelBound = true;

        UpdateMinimapMaterial();
    }
}

void UMinimapWidget::UnbindViewModel()
{
    if (MinimapViewModel && bIsViewModelBound)
    {
        MinimapViewModel->OnFieldChanged.RemoveDynamic(this, &UMinimapWidget::HandleViewModelFieldChanged);
        bIsViewModelBound = false;
    }
}

void UMinimapWidget::HandleViewModelFieldChanged(FName FieldName)
{
    if (FieldName == MinimapVMFields::IsInitialized)
    {
        UpdateMinimapMaterial();
    }
}

void UMinimapWidget::UpdateMinimapMaterial()
{
    if (!MinimapViewModel || !MinimapImage || !MinimapViewModel->IsInitialized())
    {
        return;
    }

    UMaterialInstanceDynamic* MinimapMaterial = MinimapViewModel->GetMinimapMaterial();
    if (MinimapMaterial)
    {
        MinimapImage->SetBrushFromMaterial(MinimapMaterial);
    }
}
