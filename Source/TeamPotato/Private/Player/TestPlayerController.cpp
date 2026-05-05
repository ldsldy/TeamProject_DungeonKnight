// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TestPlayerController.h"

#include "Common/MyGameSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "InputMappingContext.h"
#include "Player/TestCharacter.h"
#include "UI/InGameMenu/InGameMenuWidget.h"
#include "UI/InGameMenu/PlayerKilledWidget.h"
#include "UI/Minimap/MinimapWidget.h"

void ATestPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    SetGameOnlyInputMode();
}

void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("Player Controller Subsystem Available"));
        Subsystem->AddMappingContext(DefaultMappingContext, priority);
    }

    if (InGameMenuWidgetClass)
    {
        InGameMenuWidget = CreateWidget<UInGameMenuWidget>(this, InGameMenuWidgetClass);
        if (InGameMenuWidget)
        {
            InGameMenuWidget->AddToViewport(10);
            InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
            InGameMenuWidget->OnInGameMenuClosed.AddDynamic(this, &ATestPlayerController::OnPauseInput);
        }
    }

    if (!MinimapWidgetRef)
    {
        if (UMyGameSettings* GameSettings = UMyGameSettings::Get())
        {
            UClass* MinimapWidgetClass = GameSettings->MinimapWidget.LoadSynchronous();
            if (MinimapWidgetClass)
            {
                MinimapWidgetRef = CreateWidget<UMinimapWidget>(this, MinimapWidgetClass);
            }
        }
    }

    if (MinimapWidgetRef)
    {
        MinimapWidgetRef->AddToViewport(10);
        MinimapWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (ATestCharacter* TestCharacter = Cast<ATestCharacter>(GetPawn()))
    {
        TestCharacter->OnPlayerKilled.AddDynamic(this, &ATestPlayerController::OnAddPlayerKilledWidget);
    }
}

void ATestPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (IA_Pause)
        {
            EnhancedInput->BindAction(IA_Pause, ETriggerEvent::Started, this, &ATestPlayerController::OnPauseInput);
        }

        if (IA_Minimap)
        {
            EnhancedInput->BindAction(IA_Minimap, ETriggerEvent::Started, this, &ATestPlayerController::OnMinimapInput);
        }
    }
}

void ATestPlayerController::OnPauseInput()
{
    if (bIsMenuOpen && InGameMenuWidget)
    {
        SetPause(false);
        SetGameOnlyInputMode();

        InGameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

        bIsMenuOpen = false;
        InGameMenuWidget->InitializePauseMenu();
    }
    else if (InGameMenuWidget)
    {
        SetPause(true);
        SetGameAndUIInputMode(InGameMenuWidget);

        InGameMenuWidget->SetVisibility(ESlateVisibility::Visible);

        bIsMenuOpen = true;
    }
}

void ATestPlayerController::OnMinimapInput()
{
    if (bIsMinimapOpen && MinimapWidgetRef)
    {
        SetGameOnlyInputMode();
        bShowMouseCursor = false;
        MinimapWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
        bIsMinimapOpen = false;
    }
    else if (!bIsMinimapOpen && MinimapWidgetRef)
    {
        SetGameAndUIInputMode(MinimapWidgetRef);
        bShowMouseCursor = false;
        MinimapWidgetRef->SetVisibility(ESlateVisibility::Visible);
        bIsMinimapOpen = true;
    }
}

void ATestPlayerController::OnAddPlayerKilledWidget()
{
    if (PlayerKilledWidget)
    {
        SetGameAndUIInputMode(PlayerKilledWidget);
        PlayerKilledWidget->AddToViewport(100);
    }
}

void ATestPlayerController::SetGameOnlyInputMode()
{
    FInputModeGameOnly InputMode;
    InputMode.SetConsumeCaptureMouseDown(false);
    SetInputMode(InputMode);
    SetShowMouseCursor(false);
}

void ATestPlayerController::SetGameAndUIInputMode(UUserWidget* FocusWidget)
{
    FInputModeGameAndUI InputMode;

    UUserWidget* TargetFocusWidget = FocusWidget;
    if (!TargetFocusWidget)
    {
        TargetFocusWidget = InGameMenuWidget;
    }

    if (TargetFocusWidget)
    {
        InputMode.SetWidgetToFocus(TargetFocusWidget->TakeWidget());
    }

    SetInputMode(InputMode);
    SetShowMouseCursor(true);
}

void ATestPlayerController::SetUIOnlyInputMode()
{
}
