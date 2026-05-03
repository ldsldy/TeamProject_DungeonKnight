// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Minimap/MinimapSceneCapture2D.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Subsystem/MinimapSubsystem.h"

AMinimapSceneCapture2D::AMinimapSceneCapture2D()
{
    InitializeCaptureComponent();
}

void AMinimapSceneCapture2D::BeginPlay()
{
    Super::BeginPlay();

    InitializeRenderTarget();

    MinimapSubsystem = GetWorld()->GetSubsystem<UMinimapSubsystem>();
    if (MinimapSubsystem)
    {
        MinimapSubsystem->OnMinimapCaptureRequested.RemoveDynamic(this, &AMinimapSceneCapture2D::OnMinimapCapture);
        MinimapSubsystem->OnMinimapCaptureRequested.AddDynamic(this, &AMinimapSceneCapture2D::OnMinimapCapture);
        MinimapSubsystem->ReplayLastMinimapCaptureRequest();
    }
}

void AMinimapSceneCapture2D::OnMinimapCapture(FVector2D InMinPoint, FVector2D InMaxPoint)
{
    if (!CaptureComp)
    {
        return;
    }

    WorldMinPoint = InMinPoint;
    WorldMaxPoint = InMaxPoint;

    const FVector2D DungeonCenter = FVector2D(
        (InMinPoint.X + InMaxPoint.X) / 2.0f,
        (InMinPoint.Y + InMaxPoint.Y) / 2.0f);
    SetActorLocation(FVector(DungeonCenter, GetActorLocation().Z));

    const FVector2D DungeonSize = InMaxPoint - InMinPoint;
    CaptureOrthoWidth = FMath::Max(DungeonSize.X, DungeonSize.Y) * BorderRatio;
    CaptureComp->OrthoWidth = CaptureOrthoWidth;

    CaptureComp->CaptureScene();

    const FVector2D AdjustedMinPoint = DungeonCenter - FVector2D(CaptureOrthoWidth / 2.0f, CaptureOrthoWidth / 2.0f);

    if (MinimapSubsystem)
    {
        MinimapSubsystem->InitializeMinimap(
            CaptureComp->TextureTarget,
            AdjustedMinPoint,
            CaptureOrthoWidth);
    }
}

void AMinimapSceneCapture2D::InitializeCaptureComponent()
{
    CaptureComp = GetCaptureComponent2D();
    CaptureComp->bCaptureEveryFrame = false;
    CaptureComp->bCaptureOnMovement = false;
    CaptureComp->ProjectionType = ECameraProjectionMode::Orthographic;
    CaptureComp->CaptureSource = ESceneCaptureSource::SCS_BaseColor;
}

void AMinimapSceneCapture2D::InitializeRenderTarget()
{
    if (CaptureComp->TextureTarget)
    {
        CaptureComp->TextureTarget->SizeX = RenderTargetSize.X;
        CaptureComp->TextureTarget->SizeY = RenderTargetSize.Y;
    }
}
