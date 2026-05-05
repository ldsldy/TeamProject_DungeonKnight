// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "TestPlayerController.generated.h"

class UInGameMenuWidget;
class UInputMappingContext;
class UMinimapWidget;
class UPlayerKilledWidget;
class UUserWidget;

/**
 *
 */
UCLASS()
class TEAMPOTATO_API ATestPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetGameOnlyInputMode();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetUIOnlyInputMode();

    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetGameAndUIInputMode(UUserWidget* FocusWidget = nullptr);

protected:
    virtual void OnPossess(APawn* InPawn) override;
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION()
    void OnPauseInput();

    UFUNCTION()
    void OnMinimapInput();

private:
    UFUNCTION()
    void OnAddPlayerKilledWidget();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
    TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
    TObjectPtr<UInputAction> IA_Pause = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
    TObjectPtr<UInputAction> IA_Minimap = nullptr;

private:
    int32 priority = 1;

    bool bIsMenuOpen = false;

    UPROPERTY()
    TObjectPtr<UInGameMenuWidget> InGameMenuWidget = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UInGameMenuWidget> InGameMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPlayerKilledWidget> PlayerKilledWidget;

    bool bIsMinimapOpen = false;

    UPROPERTY(EditAnywhere, Category = "UI")
    TObjectPtr<UMinimapWidget> MinimapWidgetRef = nullptr;
};
