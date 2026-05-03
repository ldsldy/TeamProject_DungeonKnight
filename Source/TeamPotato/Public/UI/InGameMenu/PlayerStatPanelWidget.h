// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatPanelWidget.generated.h"

class UPlayerResourceViewModel;
class UProgressBar;
class UTextBlock;
class UWeaponViewModel;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UPlayerStatPanelWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindViewModel();
    void UnbindViewModel();

    UFUNCTION()
    void HandlePlayerResourceFieldChanged(FName FieldName);

    UFUNCTION()
    void HandleWeaponFieldChanged(FName FieldName);

    void UpdateWalkSpeedUI();
    void UpdateHealthUI();
    void UpdateEnergyUI();
    void UpdateWeaponDataUI();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentHealthText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentEnergyText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> EnergyProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> HealthStatText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> EnergyStatText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> AttackDamageText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WalkSpeedText;

private:
    UPROPERTY()
    TObjectPtr<UPlayerResourceViewModel> PlayerStatusViewModel;

    UPROPERTY()
    TObjectPtr<UWeaponViewModel> PlayerWeaponViewModel;

    bool bIsBound = false;
};
