// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/MainMenu/MainMenuButtonWidget.h"
#include "UI/MainMenu/MainMenuSoundOptionWidget.h"
#include "Subsystem/GameStateSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (NewGameButton)
	{
		NewGameButton->MainMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
	}
	if (SettingsButton)
	{
		SettingsButton->MainMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingClicked);
	}
	if (QuitButton)
	{
		QuitButton->MainMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
    if (MainSoundOptionPanel)
    {
        MainSoundOptionPanel->OnCloseButtonClickedDelegate.AddDynamic(this, &UMainMenuWidget::OnSoundOptionCloseClicked);
        if (MainSoundOptionPanel->OnCloseButtonClickedDelegate.IsBound())
        {
            UE_LOG(LogTemp, Warning, TEXT("Close Button is bound successfully in MainMenuWidget"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Close Button binding failed in MainMenuWidget"));
        }
    }
}

void UMainMenuWidget::NativeDestruct()
{
    if(NewGameButton)
    {
        NewGameButton->MainMenuButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::OnNewGameClicked);
    }
    if(SettingsButton)
    {
        SettingsButton->MainMenuButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::OnSettingClicked);
    }
    if(QuitButton)
    {
        QuitButton->MainMenuButton->OnClicked.RemoveDynamic(this, &UMainMenuWidget::OnQuitClicked);
    }
    if(MainSoundOptionPanel)
    {
        MainSoundOptionPanel->OnCloseButtonClickedDelegate.RemoveDynamic(this, &UMainMenuWidget::OnSoundOptionCloseClicked);
    }

    Super::NativeDestruct();
}

void UMainMenuWidget::OnNewGameClicked()
{
    if (UGameStateSubsystem* GameStateSubsystem = UGameStateSubsystem::Get(this))
    {
        GameStateSubsystem->TravelToState(EGameState::Lobby);
    }
}

void UMainMenuWidget::OnSettingClicked()
{
    MenuWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UMainMenuWidget::OnQuitClicked()
{
	APlayerController* PC = GetOwningPlayer();

	UKismetSystemLibrary::QuitGame(
		this,
		PC,
		EQuitPreference::Quit,
		false
	);
}

void UMainMenuWidget::OnSoundOptionCloseClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Sound Option Close Clicked"));

    if (MenuWidgetSwitcher)
    {
        MenuWidgetSwitcher->SetActiveWidgetIndex(0);
    }
}
