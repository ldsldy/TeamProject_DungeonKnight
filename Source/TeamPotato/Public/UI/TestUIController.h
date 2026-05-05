// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestUIController.generated.h"

class UMinimapWidget;

/**
 *
 */
UCLASS()
class TEAMPOTATO_API ATestUIController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMinimapWidget> MinimapWidgetClass;

    UPROPERTY()
    TObjectPtr<UMinimapWidget> MinimapWidget = nullptr;
};
