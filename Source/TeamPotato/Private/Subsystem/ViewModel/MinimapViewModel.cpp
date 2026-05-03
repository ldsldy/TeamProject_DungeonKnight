// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ViewModel/MinimapViewModel.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "TeamPotato/Logic/DungeonGanarator.h"
#include "UI/Minimap/MinimapManager.h"
#include "Materials/MaterialInstanceDynamic.h"

void UMinimapViewModel::Initialize(UObject* InModel)
{
    Deinitialize();

    Super::Initialize(InModel);

    Model = Cast<ADungeonGanarator>(InModel);
    if (!Model)
    {
        return;
    }

    Model->OnDungeonGenerationCompleted.AddDynamic(this, &UMinimapViewModel::RequestMinimapCapture);
}

void UMinimapViewModel::Deinitialize()
{
    if (Model)
    {
        Model->OnDungeonGenerationCompleted.RemoveDynamic(this, &UMinimapViewModel::RequestMinimapCapture);
        Model = nullptr;
    }

    MinimapManager = nullptr;
    SetAndNotify(bIsInitialized, false, MinimapVMFields::IsInitialized);

    Super::Deinitialize();
}

void UMinimapViewModel::RequestMinimapCapture(FVector2D InMinPoint, FVector2D InMaxPoint)
{
    LastCaptureMinPoint = InMinPoint;
    LastCaptureMaxPoint = InMaxPoint;
    bHasLastCaptureBounds = true;

    if (OnMinimapCaptureRequested.IsBound())
    {
        OnMinimapCaptureRequested.Broadcast(InMinPoint, InMaxPoint);
    }
}

void UMinimapViewModel::ReplayLastMinimapCaptureRequest()
{
    if (!bHasLastCaptureBounds || !OnMinimapCaptureRequested.IsBound())
    {
        return;
    }

    OnMinimapCaptureRequested.Broadcast(LastCaptureMinPoint, LastCaptureMaxPoint);
}

void UMinimapViewModel::UpdatePlayerPosition(const FVector& InWorldLocation, float InYaw)
{
    if (MinimapManager)
    {
        MinimapManager->UpdatePlayerPosition(InWorldLocation, InYaw);
    }
}

void UMinimapViewModel::SetMinimapManager(UMinimapManager* InMinimapManager)
{
    MinimapManager = InMinimapManager;
    if (SetAndNotify(bIsInitialized, MinimapManager != nullptr, MinimapVMFields::IsInitialized))
    {
        if (bIsInitialized && OnMinimapInitialized.IsBound())
        {
            OnMinimapInitialized.Broadcast();
        }
    }
}

UMaterialInstanceDynamic* UMinimapViewModel::GetMinimapMaterial() const
{
    if (MinimapManager)
    {
        return MinimapManager->GetMinimapMaterial();
    }

    return nullptr;
}
