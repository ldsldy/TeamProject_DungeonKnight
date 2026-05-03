// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWeaponWidget.generated.h"

class UImage;
class UPlayerResourceViewModel;
class UProgressBar;
class UTextBlock;
class UWeaponViewModel;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UPlayerWeaponWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindViewModel();
    void UnbindViewModel();

    void UpdatePlayerResourceBar();
    void UpdateMainWeaponInfo();
    void UpdateSubWeaponInfo();

    UFUNCTION()
    void HandleResourceFieldChanged(FName FieldName);

    UFUNCTION()
    void HandleWeaponFieldChanged(FName FieldName);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> WeaponIconImage = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WeaponName = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SubWeaponIconImage = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> PlayerResourceBar = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> UsageResourceText = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrentEnergyText = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MaxEnergyText = nullptr;

private:
    UPROPERTY()
    TObjectPtr<UWeaponViewModel> WeaponViewModel;

    UPROPERTY()
    TObjectPtr<UPlayerResourceViewModel> PlayerResourceViewModel;

    bool bIsViewModelBound = false;
};
