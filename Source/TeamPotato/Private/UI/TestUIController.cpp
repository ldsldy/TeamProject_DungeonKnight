// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestUIController.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "Subsystem/ViewModel/MinimapViewModel.h"
#include "UI/Minimap/MinimapWidget.h"

void ATestUIController::BeginPlay()
{
    Super::BeginPlay();

    // 위젯 생성 및 뷰포트 추가
    if (MinimapWidgetClass)
    {
        MinimapWidget = CreateWidget<UMinimapWidget>(this, MinimapWidgetClass);

        if (MinimapWidget)
        {
            MinimapWidget->AddToViewport(10);

            // 뷰모델 설정
            if (UMVVMSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UMVVMSubsystem>())
            {
                if (!MinimapViewModel)
                {
                    MinimapViewModel = Subsystem->GetMinimapViewModel();
                    MinimapViewModel->OnFieldChanged.AddDynamic(this, &ATestUIController::HandleMinimapViewModelFieldChanged);
                }
            }
        }
    }

    // 플레이어 위치 업데이트 타이머
    GetWorldTimerManager().SetTimer(
        MinimapUpdateTimer,
        this,
        &ATestUIController::IsMinimapUpdateThresholdReached,
        0.1f,
        true
    );
}

void ATestUIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(MinimapUpdateTimer);

    if (MinimapViewModel)
    {
        MinimapViewModel->OnFieldChanged.RemoveDynamic(this, &ATestUIController::HandleMinimapViewModelFieldChanged);
    }

    Super::EndPlay(EndPlayReason);
}

void ATestUIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    // 초기 위치 설정
    if (InPawn)
    {
        LastPawnLocation = FVector::ZeroVector;
        LastPawnYaw = 0;
    }
}

void ATestUIController::IsMinimapUpdateThresholdReached()
{
    //UE_LOG(LogTemp, Log, TEXT("LastPawnLocation: %s, LastPawnYaw: %f"), *LastPawnLocation.ToString(), LastPawnYaw);

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    CurrentPawnLocation = ControlledPawn->GetActorLocation();
    CurrentPawnYaw = ControlledPawn->GetActorRotation().Yaw;

    const float DistanceMoved = FVector::DistSquared(CurrentPawnLocation, LastPawnLocation);
    const float YawDifference = FMath::Abs(CurrentPawnYaw - LastPawnYaw);

    //UE_LOG(LogTemp, Warning, TEXT("DistanceMoved: %f, YawDifference: %f"), DistanceMoved, YawDifference);s

    if(DistanceMoved < MinimapUpdateThreshold * MinimapUpdateThreshold &&
       YawDifference < MinimapYawUpdateThreshold)
    {
        return; // 임계값 이하로 이동/회전했으면 업데이트하지 않음
    }

    UpdateMinimapPlayerPosition();
}

void ATestUIController::UpdateMinimapPlayerPosition()
{
    if (!MinimapViewModel)
    {
        if (UMVVMSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UMVVMSubsystem>())
        {
            MinimapViewModel = Subsystem->GetMinimapViewModel();
        }
    }

    if (MinimapViewModel)
    {
        MinimapViewModel->UpdatePlayerPosition(CurrentPawnLocation, CurrentPawnYaw);
        LastPawnLocation = CurrentPawnLocation;
        LastPawnYaw = CurrentPawnYaw;
    }
}

void ATestUIController::HandleMinimapViewModelFieldChanged(FName FieldName)
{
    if (FieldName == MinimapVMFields::IsInitialized)
    {
        UpdateMinimapPlayerPosition();
    }
}
