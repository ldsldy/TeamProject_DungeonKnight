// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MinimapSubsystem.h"

#include "Common/MyGameSettings.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TeamPotato/Logic/DungeonGanarator.h"
#include "TimerManager.h"

void UMinimapSubsystem::Deinitialize()
{
    if (RegisteredDungeonGenerator)
    {
        RegisteredDungeonGenerator->OnDungeonGenerationCompleted.RemoveDynamic(this, &UMinimapSubsystem::RequestMinimapCapture);
        RegisteredDungeonGenerator = nullptr;
    }

    ResetMinimap();
    Super::Deinitialize();
}

void UMinimapSubsystem::RegisterDungeonGenerator(ADungeonGanarator* NewDungeonGenerator)
{
    if (!NewDungeonGenerator)
    {
        return;
    }

    if (RegisteredDungeonGenerator && RegisteredDungeonGenerator != NewDungeonGenerator)
    {
        UnregisterDungeonGenerator(RegisteredDungeonGenerator);
    }

    RegisteredDungeonGenerator = NewDungeonGenerator;
    RegisteredDungeonGenerator->OnDungeonGenerationCompleted.RemoveDynamic(this, &UMinimapSubsystem::RequestMinimapCapture);
    RegisteredDungeonGenerator->OnDungeonGenerationCompleted.AddDynamic(this, &UMinimapSubsystem::RequestMinimapCapture);
}

void UMinimapSubsystem::UnregisterDungeonGenerator(ADungeonGanarator* ExitingDungeonGenerator)
{
    if (RegisteredDungeonGenerator != ExitingDungeonGenerator)
    {
        return;
    }

    RegisteredDungeonGenerator->OnDungeonGenerationCompleted.RemoveDynamic(this, &UMinimapSubsystem::RequestMinimapCapture);
    RegisteredDungeonGenerator = nullptr;
}

void UMinimapSubsystem::RequestMinimapCapture(FVector2D InMinPoint, FVector2D InMaxPoint)
{
    LastCaptureMinPoint = InMinPoint;
    LastCaptureMaxPoint = InMaxPoint;
    bHasLastCaptureBounds = true;

    OnMinimapCaptureRequested.Broadcast(InMinPoint, InMaxPoint);
}

void UMinimapSubsystem::ReplayLastMinimapCaptureRequest()
{
    if (!bHasLastCaptureBounds)
    {
        return;
    }

    OnMinimapCaptureRequested.Broadcast(LastCaptureMinPoint, LastCaptureMaxPoint);
}

void UMinimapSubsystem::InitializeMinimap(UTextureRenderTarget2D* InRenderTarget, FVector2D InMinPoint, float InOrthoWidth)
{
    if (!InRenderTarget)
    {
        return;
    }

    UMyGameSettings* GameSettings = UMyGameSettings::Get();
    if (!GameSettings)
    {
        return;
    }

    UMaterialInterface* BaseMaterial = GameSettings->MinimapBaseMaterial.LoadSynchronous();
    if (!BaseMaterial)
    {
        return;
    }

    MinimapMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
    if (!MinimapMaterial)
    {
        return;
    }

    MinimapMaterial->SetTextureParameterValue(TEXT("MinimapTexture"), InRenderTarget);

    if (UTexture* PlayerIconTexture = GameSettings->MinimapPlayerIcon.LoadSynchronous())
    {
        MinimapMaterial->SetTextureParameterValue(TEXT("PlayerIconTexture"), PlayerIconTexture);
    }

    MinimapMaterial->SetScalarParameterValue(TEXT("PlayerIconSize"), 0.05f);

    WorldMinPoint = InMinPoint;
    OrthoWidth = InOrthoWidth;

    FogTexture = UTexture2D::CreateTransient(FogResolution, FogResolution, EPixelFormat::PF_B8G8R8A8);
    if (FogTexture)
    {
        FogTexture->UpdateResource();
        FogData.Init(FColor::Black, FogResolution * FogResolution);
        MinimapMaterial->SetTextureParameterValue(TEXT("FogTexture"), FogTexture);
    }

    MinimapMaterial->SetScalarParameterValue(TEXT("ViewRadius"), 0.15f);

    bIsInitialized = true;
    OnMinimapInitialized.Broadcast();

    StartPlayerTracking();
    if (APawn* TrackedPawn = GetTrackedPlayerPawn())
    {
        UpdateTrackedPlayerPosition(TrackedPawn);
    }
}

FVector2D UMinimapSubsystem::WorldToMinimapUV(const FVector2D& InWorldLocation2D) const
{
    if (OrthoWidth <= 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("MinimapSubsystem::WorldToMinimapUV - OrthoWidth is not initialized."));
        return FVector2D(0.5f, 0.5f);
    }

    const FVector2D RelativePosition = InWorldLocation2D - WorldMinPoint;

    FVector2D UV;
    UV.X = RelativePosition.Y / OrthoWidth;
    UV.Y = 1.f - (RelativePosition.X / OrthoWidth);

    UV.X = FMath::Clamp(UV.X, 0.f, 1.f);
    UV.Y = FMath::Clamp(UV.Y, 0.f, 1.f);

    return UV;
}

void UMinimapSubsystem::UpdatePlayerPosition(const FVector& InWorldLocation, float InYaw)
{
    if (!MinimapMaterial)
    {
        return;
    }

    const FVector2D PlayerUV = WorldToMinimapUV(FVector2D(InWorldLocation.X, InWorldLocation.Y));

    MinimapMaterial->SetScalarParameterValue(TEXT("PlayerPosX"), PlayerUV.X);
    MinimapMaterial->SetScalarParameterValue(TEXT("PlayerPosY"), PlayerUV.Y);
    MinimapMaterial->SetScalarParameterValue(TEXT("PlayerRotation"), InYaw / 360.f);

    RevealArea(PlayerUV, 0.1f);
}

void UMinimapSubsystem::StartPlayerTracking()
{
    UWorld* World = GetWorld();
    if (!World || World->GetTimerManager().IsTimerActive(MinimapUpdateTimer))
    {
        return;
    }

    World->GetTimerManager().SetTimer(
        MinimapUpdateTimer,
        this,
        &UMinimapSubsystem::CheckPlayerPositionUpdate,
        MinimapUpdateInterval,
        true);
}

void UMinimapSubsystem::StopPlayerTracking()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(MinimapUpdateTimer);
    }
}

void UMinimapSubsystem::CheckPlayerPositionUpdate()
{
    if (!bIsInitialized)
    {
        return;
    }

    APawn* TrackedPawn = GetTrackedPlayerPawn();
    if (!TrackedPawn)
    {
        return;
    }

    const FVector CurrentPlayerLocation = TrackedPawn->GetActorLocation();
    const float CurrentPlayerYaw = TrackedPawn->GetActorRotation().Yaw;

    if (bHasLastPlayerTransform)
    {
        const float DistanceMoved = FVector::DistSquared(CurrentPlayerLocation, LastPlayerLocation);
        const float YawDifference = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentPlayerYaw, LastPlayerYaw));

        if (DistanceMoved < MinimapUpdateThreshold * MinimapUpdateThreshold &&
            YawDifference < MinimapYawUpdateThreshold)
        {
            return;
        }
    }

    UpdateTrackedPlayerPosition(TrackedPawn);
}

void UMinimapSubsystem::UpdateTrackedPlayerPosition(APawn* TrackedPawn)
{
    if (!TrackedPawn)
    {
        return;
    }

    LastPlayerLocation = TrackedPawn->GetActorLocation();
    LastPlayerYaw = TrackedPawn->GetActorRotation().Yaw;
    bHasLastPlayerTransform = true;

    UpdatePlayerPosition(LastPlayerLocation, LastPlayerYaw);
}

APawn* UMinimapSubsystem::GetTrackedPlayerPawn() const
{
    const UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    const APlayerController* PlayerController = World->GetFirstPlayerController();
    return PlayerController ? PlayerController->GetPawn() : nullptr;
}

void UMinimapSubsystem::RevealArea(FVector2D UV, float Radius)
{
    if (FogData.IsEmpty())
    {
        return;
    }

    const int32 CenterX = FMath::RoundToInt(UV.X * FogResolution);
    const int32 CenterY = FMath::RoundToInt(UV.Y * FogResolution);
    const int32 RadiusPixels = FMath::RoundToInt(Radius * FogResolution);

    for (int32 Y = -RadiusPixels; Y <= RadiusPixels; ++Y)
    {
        for (int32 X = -RadiusPixels; X <= RadiusPixels; ++X)
        {
            if (X * X + Y * Y <= RadiusPixels * RadiusPixels)
            {
                const int32 PX = FMath::Clamp(CenterX + X, 0, FogResolution - 1);
                const int32 PY = FMath::Clamp(CenterY + Y, 0, FogResolution - 1);

                FogData[PY * FogResolution + PX] = FColor::White;
                bFogDirty = true;
            }
        }
    }

    if (bFogDirty)
    {
        UpdateFogTexture();
        bFogDirty = false;
    }
}

void UMinimapSubsystem::UpdateFogTexture()
{
    if (!FogTexture)
    {
        return;
    }

    void* TextureData = FogTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, FogData.GetData(), FogData.Num() * sizeof(FColor));
    FogTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
    FogTexture->UpdateResource();
}

void UMinimapSubsystem::ResetMinimap()
{
    StopPlayerTracking();
    MinimapMaterial = nullptr;
    FogTexture = nullptr;
    FogData.Reset();
    WorldMinPoint = FVector2D::ZeroVector;
    OrthoWidth = 0.f;
    bFogDirty = false;
    bHasLastPlayerTransform = false;
    LastPlayerLocation = FVector::ZeroVector;
    LastPlayerYaw = 0.f;
    bIsInitialized = false;
    bHasLastCaptureBounds = false;
    LastCaptureMinPoint = FVector2D::ZeroVector;
    LastCaptureMaxPoint = FVector2D::ZeroVector;
}
