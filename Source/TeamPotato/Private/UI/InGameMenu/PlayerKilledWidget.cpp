// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameMenu/PlayerKilledWidget.h"
#include "Subsystem/GameStateSubsystem.h"
#include "Components/Button.h"

void UPlayerKilledWidget::NativeConstruct()
{
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UPlayerKilledWidget::OnRestartButtonClicked);
    }
}

void UPlayerKilledWidget::OnRestartButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Restart Button Clicked - Traveling to Lobby"));
    if (UGameStateSubsystem* GameStateSubsystem = GetGameInstance()->GetSubsystem<UGameStateSubsystem>())
    {
        GameStateSubsystem->TravelToState(EGameState::Lobby);
    }
}
