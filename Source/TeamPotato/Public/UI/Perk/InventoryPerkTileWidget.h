// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPerkTileWidget.generated.h"

class UPerkDataAsset;
class UPerkViewModel;
class UTileView;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UInventoryPerkTileWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION()
    void OnPerkitemHoveredChanged(UObject* Item, bool bIsHovered);

private:
    void BindViewModel();
    void UnbindViewModel();
    void RefreshPerkTileList();
    void LoadPerkDataFromDataAsset(UPerkDataAsset* InData);

    UFUNCTION()
    void HandlePerkFieldChanged(FName FieldName);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTileView> PerkTileView = nullptr;

private:
    UPROPERTY()
    TObjectPtr<UPerkViewModel> PerkViewModel;
};
