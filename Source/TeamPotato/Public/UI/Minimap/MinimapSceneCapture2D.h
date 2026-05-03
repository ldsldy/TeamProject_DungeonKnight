// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "MinimapSceneCapture2D.generated.h"

class UMinimapSubsystem;

/**
 * Captures the generated dungeon map and passes the render target to the minimap subsystem.
 */
UCLASS()
class TEAMPOTATO_API AMinimapSceneCapture2D : public ASceneCapture2D
{
    GENERATED_BODY()

public:
    AMinimapSceneCapture2D();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnMinimapCapture(FVector2D InMinPoint, FVector2D InMaxPoint);

private:
    void InitializeCaptureComponent();
    void InitializeRenderTarget();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneCaptureComponent2D> CaptureComp;

    UPROPERTY()
    TObjectPtr<UMinimapSubsystem> MinimapSubsystem = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector2D RenderTargetSize = FVector2D(800.0f, 800.0f);

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float BorderRatio = 1.1f;

    float CaptureOrthoWidth = 0.f;
    FVector2D WorldMinPoint = FVector2D::ZeroVector;
    FVector2D WorldMaxPoint = FVector2D::ZeroVector;
};
