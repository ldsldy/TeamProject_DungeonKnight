// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ViewModel/MinimapViewModel.h"
#include "UI/Minimap/MinimapManager.h"
#include "Materials/MaterialInstanceDynamic.h"

// --- 맵 생성시에 미니맵 캡처 요청 함수 ---
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

// --- 미니맵 매니저에서 머티리얼의 플레이어 위치 업데이트 함수 ---
void UMinimapViewModel::UpdatePlayerPosition(const FVector& InWorldLocation, float InYaw)
{
    if (MinimapManager)
    {
        MinimapManager->UpdatePlayerPosition(InWorldLocation, InYaw);
    }
}

// --- 위젯에 미니맵 머티리얼 제공을 위한 함수 ---
void UMinimapViewModel::SetMinimapManager(UMinimapManager* InMinimapManager)
{
    MinimapManager = InMinimapManager;

    // 초기화 완료 알림
    if (OnMinimapInitialized.IsBound())
    {
        OnMinimapInitialized.Broadcast();
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
