// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ViewModel/PGViewModelBase.h"
#include "MinimapViewModel.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMinimapCaptureRequested, FVector2D, InMinPoint, FVector2D, InMaxPoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinimapInitialized);

class ADungeonGanarator;
class UMinimapManager;
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS(Blueprintable)
class TEAMPOTATO_API UMinimapViewModel : public UPGViewModelBase
{
    GENERATED_BODY()

public:
    virtual void Initialize(UObject* InModel) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable)
    void RequestMinimapCapture(FVector2D InMinPoint, FVector2D InMaxPoint);

    UFUNCTION(BlueprintCallable)
    void UpdatePlayerPosition(const FVector& InWorldLocation, float InYaw);

    void SetMinimapManager(UMinimapManager* InMinimapManager);

    UFUNCTION(BlueprintPure)
    UMaterialInstanceDynamic* GetMinimapMaterial() const;

    UFUNCTION(BlueprintPure)
    bool IsInitialized() const { return bIsInitialized; }

    void ReplayLastMinimapCaptureRequest();

public:
    UPROPERTY(BlueprintAssignable)
    FOnMinimapCaptureRequested OnMinimapCaptureRequested;

    UPROPERTY(BlueprintAssignable)
    FOnMinimapInitialized OnMinimapInitialized;

private:
    UPROPERTY()
    TObjectPtr<ADungeonGanarator> Model = nullptr;

    UPROPERTY()
    TObjectPtr<UMinimapManager> MinimapManager = nullptr;

    UPROPERTY()
    bool bIsInitialized = false;

    bool bHasLastCaptureBounds = false;
    FVector2D LastCaptureMinPoint = FVector2D::ZeroVector;
    FVector2D LastCaptureMaxPoint = FVector2D::ZeroVector;
};
