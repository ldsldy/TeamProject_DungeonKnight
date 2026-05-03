// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWeaponWidget.generated.h"

class UImage;
class UTextBlock;
class UWeaponViewModel;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UPlayerStatWeaponWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindViewModel();
    void UnbindViewModel();
    void UpdateMainWeaponIcon();
    void UpdateSubWeaponIcon();

    UFUNCTION()
    void HandleWeaponFieldChanged(FName FieldName);

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> MainWeaponIcon;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SubWeaponIcon;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MainWeaponName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SubWeaponName;

    UPROPERTY()
    TObjectPtr<UWeaponViewModel> WeaponViewModel;

    bool bIsBound = false;
};
