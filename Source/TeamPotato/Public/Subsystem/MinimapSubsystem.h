// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MinimapSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMinimapCaptureRequested, FVector2D, InMinPoint, FVector2D, InMaxPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinimapInitialized);

class ADungeonGanarator;
class UMaterialInstanceDynamic;
class UTexture2D;
class UTextureRenderTarget2D;

/**
 * Owns the shared minimap state and routes minimap-specific gameplay events.
 */
UCLASS()
class TEAMPOTATO_API UMinimapSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Deinitialize() override;

    void RegisterDungeonGenerator(ADungeonGanarator* NewDungeonGenerator);
    void UnregisterDungeonGenerator(ADungeonGanarator* ExitingDungeonGenerator);

    UFUNCTION(BlueprintCallable)
    void RequestMinimapCapture(FVector2D InMinPoint, FVector2D InMaxPoint);

    void ReplayLastMinimapCaptureRequest();

    void InitializeMinimap(UTextureRenderTarget2D* InRenderTarget, FVector2D InMinPoint, float InOrthoWidth);

    UFUNCTION(BlueprintCallable)
    void UpdatePlayerPosition(const FVector& InWorldLocation, float InYaw);

    UFUNCTION(BlueprintPure)
    UMaterialInstanceDynamic* GetMinimapMaterial() const { return MinimapMaterial; }

    UFUNCTION(BlueprintPure)
    bool IsInitialized() const { return bIsInitialized; }

public:
    UPROPERTY(BlueprintAssignable)
    FOnMinimapCaptureRequested OnMinimapCaptureRequested;

    UPROPERTY(BlueprintAssignable)
    FOnMinimapInitialized OnMinimapInitialized;

private:
    FVector2D WorldToMinimapUV(const FVector2D& InWorldLocation2D) const;
    void RevealArea(FVector2D UV, float Radius);
    void UpdateFogTexture();
    void ResetMinimap();

private:
    UPROPERTY()
    TObjectPtr<ADungeonGanarator> RegisteredDungeonGenerator = nullptr;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> MinimapMaterial = nullptr;

    UPROPERTY()
    TObjectPtr<UTexture2D> FogTexture = nullptr;

    UPROPERTY()
    FVector2D WorldMinPoint = FVector2D::ZeroVector;

    UPROPERTY()
    float OrthoWidth = 0.f;

    UPROPERTY()
    TArray<FColor> FogData;

    UPROPERTY()
    int32 FogResolution = 128;

    UPROPERTY()
    bool bIsInitialized = false;

    bool bFogDirty = false;
    bool bHasLastCaptureBounds = false;
    FVector2D LastCaptureMinPoint = FVector2D::ZeroVector;
    FVector2D LastCaptureMaxPoint = FVector2D::ZeroVector;
};
