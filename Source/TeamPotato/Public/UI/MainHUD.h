// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/BaseLayerWidget.h"
#include "MainHUD.generated.h"

class UPerkSelectionScreenWidget;
class UMainHUDWidget;
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
    AMainHUD();

    UFUNCTION(BlueprintCallable, Category = "Perk|Selection")
    void TryPerkSelectionScreen(int32 InStage, int32 InChapter);

    UFUNCTION(BlueprintCallable, Category = "Perk|Selection")
    void AddPerkSelectionScreenToViewport();

    UFUNCTION()
    void RemovePerkSelectionScreenFromViewport();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMainHUDWidget> MainHUDWidgetClass;

    UPROPERTY()
    TObjectPtr<UMainHUDWidget> MainHUDWidget;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UPerkSelectionScreenWidget> PerkSelectionScreenClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBaseLayerWidget> BaseLayerWidgetClass;

    UPROPERTY(Transient)
    TObjectPtr<UBaseLayerWidget> BaseLayerWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Layer")
    FLayerWidgetActivationContainer OpenPerkSelectionLayerTags;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Layer")
    FLayerWidgetActivationContainer ClosePerkSelectionLayerTags;
};
