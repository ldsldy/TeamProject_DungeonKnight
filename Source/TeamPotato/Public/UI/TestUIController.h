// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestUIController.generated.h"

class UMVVMSubsystem;
class UInGameMenuWidget;
class UMinimapWidget;
class UMinimapViewModel;
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API ATestUIController : public APlayerController
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION()
    void IsMinimapUpdateThresholdReached();

    UFUNCTION()
    void UpdateMinimapPlayerPosition();

    UFUNCTION()
    void HandleMinimapViewModelFieldChanged(FName FieldName);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMinimapWidget> MinimapWidgetClass;

    UPROPERTY()
    TObjectPtr<UMinimapWidget> MinimapWidget = nullptr;

private:
    TObjectPtr<UMinimapViewModel> MinimapViewModel = nullptr;

    FVector CurrentPawnLocation = FVector::ZeroVector;
    float CurrentPawnYaw = 0.f;

    FVector LastPawnLocation = FVector::ZeroVector;
    float LastPawnYaw = 0.f;

    // --- 거리 업데이트 임계값 ---
    UPROPERTY(EditDefaultsOnly, Category = "Minimap")
    float MinimapUpdateThreshold = 10.f;

    UPROPERTY(EditDefaultsOnly, Category = "Minimap")
    float MinimapYawUpdateThreshold = 5.f;

    FTimerHandle MinimapUpdateTimer;
};
