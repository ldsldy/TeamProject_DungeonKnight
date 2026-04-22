// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameMenu/InGameMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "UI/InGameMenu/MenuPlayerStatWidget.h"
#include "UI/InGameMenu/PauseMenuWidget.h"
#include "UI/MainMenu/MainMenuSoundOptionWidget.h"
#include "UI/ConfirmPopupWidget.h"
#include "Subsystem/GameStateSubsystem.h"

void UInGameMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (PauseMenuPanel)
    {
        PauseMenuPanel->GetContinueButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnContinueButtonClicked);
        PauseMenuPanel->GetShowCharacterStatButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnShowCharacterStatButtonClicked);
        PauseMenuPanel->GetSettingButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnSettingButtonClicked);
        PauseMenuPanel->GetGiveUpButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnGiveUpButtonClicked);
        PauseMenuPanel->GetQuitButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnQuitButtonClicked);
    }
    if (PlayingPlayerStatPanel)
    {
        PlayingPlayerStatPanel->GetBackButton()->OnClicked.AddDynamic(this, &UInGameMenuWidget::OnBackToPauseClicked);
    }
    if (InGameSoundOptionMenuPanel)
    {
        InGameSoundOptionMenuPanel->OnCloseButtonClickedDelegate.AddDynamic(this, &UInGameMenuWidget::OnBackToPauseClicked);
    }

    SetIsFocusable(true);
}

void UInGameMenuWidget::NativeDestruct()
{
    if (PauseMenuPanel)
    {
        PauseMenuPanel->GetContinueButton()->OnClicked.RemoveDynamic(this, &UInGameMenuWidget::OnContinueButtonClicked);
        PauseMenuPanel->GetShowCharacterStatButton()->OnClicked.RemoveDynamic(this, &UInGameMenuWidget::OnShowCharacterStatButtonClicked);
        PauseMenuPanel->GetSettingButton()->OnClicked.RemoveDynamic(this, &UInGameMenuWidget::OnSettingButtonClicked);
        PauseMenuPanel->GetGiveUpButton()->OnClicked.RemoveDynamic(this, &UInGameMenuWidget::OnGiveUpButtonClicked);
        PauseMenuPanel->GetQuitButton()->OnClicked.RemoveDynamic(this, &UInGameMenuWidget::OnQuitButtonClicked);
    }
    if (PlayingPlayerStatPanel)
    {
        PlayingPlayerStatPanel->GetBackButton()->OnClicked.RemoveDynamic(this, &UInGameMenuWidget::OnBackToPauseClicked);
    }
    if (InGameSoundOptionMenuPanel)
    {
        InGameSoundOptionMenuPanel->OnCloseButtonClickedDelegate.RemoveDynamic(this, &UInGameMenuWidget::OnBackToPauseClicked);
    }
    Super::NativeDestruct();
}

void UInGameMenuWidget::InitializePauseMenu()
{
    if (MenuWidgetSwitcher && PauseMenuPanel)
    {
        MenuWidgetSwitcher->SetActiveWidgetIndex(0);
    }
}

void UInGameMenuWidget::OnContinueButtonClicked()
{
    // 컨트로러에 메뉴 닫기 요청 보내기
    if (OnInGameMenuClosed.IsBound())
    {
        OnInGameMenuClosed.Broadcast();
    }
}

void UInGameMenuWidget::OnShowCharacterStatButtonClicked()
{
    // 위젯 스위처를 사용하여 플레이어 스탯 패널로 전환
    MenuWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UInGameMenuWidget::OnSettingButtonClicked()
{
    // 위젯 스위처로 설정 패널로 전환
    MenuWidgetSwitcher->SetActiveWidgetIndex(2);
}

void UInGameMenuWidget::OnGiveUpButtonClicked()
{
    // 확인 팝업 생성 후에 이벤트 연결
    if (ConfirmPopupWidgetClass)
    {
        UConfirmPopupWidget* GiveUpConfirmPopup =
            CreateWidget<UConfirmPopupWidget>(GetWorld(), ConfirmPopupWidgetClass);

        GiveUpConfirmPopup->AddToViewport(100);

        GiveUpConfirmPopup->OnConfirmed.AddDynamic(this, &UInGameMenuWidget::OnConfirmGiveUp);
    }
}

void UInGameMenuWidget::OnQuitButtonClicked()
{
    if (ConfirmPopupWidgetClass)
    {
        UConfirmPopupWidget* QuitConfirmPopup =
            CreateWidget<UConfirmPopupWidget>(GetWorld(), ConfirmPopupWidgetClass);

        QuitConfirmPopup->AddToViewport(20);

        QuitConfirmPopup->OnConfirmed.AddDynamic(this, &UInGameMenuWidget::OnConfirmQuit);
    }
}

// 게임 포기 확인 시 호출될 함수
void UInGameMenuWidget::OnConfirmGiveUp()
{
    GetGameInstance()->GetSubsystem<UGameStateSubsystem>()->TravelToState(EGameState::Lobby);
}

// 게임 종료 확인 시 호출될 함수
void UInGameMenuWidget::OnConfirmQuit()
{
    GetGameInstance()->GetSubsystem<UGameStateSubsystem>()->TravelToState(EGameState::MainMenu);
}

void UInGameMenuWidget::OnBackToPauseClicked()
{
    if (MenuWidgetSwitcher && PauseMenuPanel)
    {
        MenuWidgetSwitcher->SetActiveWidgetIndex(0);
    }
}
