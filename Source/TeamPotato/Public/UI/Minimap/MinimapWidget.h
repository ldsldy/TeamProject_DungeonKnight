// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

class UImage;
class UMinimapSubsystem;

/**
 * Displays the minimap material owned by the minimap subsystem.
 */
UCLASS()
class TEAMPOTATO_API UMinimapWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindMinimapSubsystem();
    void UnbindMinimapSubsystem();
    void UpdateMinimapMaterial();

    UFUNCTION()
    void HandleMinimapInitialized();

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> MinimapImage;

private:
    UPROPERTY()
    TObjectPtr<UMinimapSubsystem> MinimapSubsystem;

    bool bIsMinimapSubsystemBound = false;
};
