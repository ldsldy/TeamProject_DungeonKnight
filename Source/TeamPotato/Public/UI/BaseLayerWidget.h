// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "BaseLayerWidget.generated.h"

USTRUCT(BlueprintType)
struct FLayerWidgetActivationContainer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Layer")
    FGameplayTagContainer ActivateTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Layer")
    FGameplayTagContainer DeactivateTags;
};

USTRUCT()
struct FLayerWidgetEntry
{
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UUserWidget> Widget = nullptr;

    UPROPERTY()
    int32 ZOrder = 0;
};

UCLASS()
class TEAMPOTATO_API UBaseLayerWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI|Layer")
    UUserWidget* RegisterLayerWidgetClass(FGameplayTag LayerTag, TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);

    UFUNCTION(BlueprintCallable, Category = "UI|Layer")
    void RegisterLayerWidget(FGameplayTag LayerTag, UUserWidget* WidgetInstance, int32 ZOrder = 0);

    UFUNCTION(BlueprintCallable, Category = "UI|Layer")
    bool ActivateLayerWidget(FGameplayTag LayerTag);

    UFUNCTION(BlueprintCallable, Category = "UI|Layer")
    bool DeactivateLayerWidget(FGameplayTag LayerTag);

    UFUNCTION(BlueprintCallable, Category = "UI|Layer")
    void ApplyLayerActivation(const FLayerWidgetActivationContainer& ActivationContainer);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI|Layer")
    UUserWidget* FindLayerWidget(FGameplayTag LayerTag) const;

private:
    UPROPERTY(Transient)
    TMap<FGameplayTag, FLayerWidgetEntry> LayerWidgetMap;
};
