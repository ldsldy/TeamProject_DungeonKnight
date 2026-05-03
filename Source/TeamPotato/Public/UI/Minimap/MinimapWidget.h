// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

class UImage;
class UMinimapViewModel;

/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UMinimapWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

private:
    void BindViewModel();
    void UnbindViewModel();
    void UpdateMinimapMaterial();

    UFUNCTION()
    void HandleViewModelFieldChanged(FName FieldName);

protected:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<UImage> MinimapImage;

private:
    UPROPERTY()
    TObjectPtr<UMinimapViewModel> MinimapViewModel;

    bool bIsViewModelBound = false;
};
