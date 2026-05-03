// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TestPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "InputMappingContext.h"
#include "Subsystem/MVVMSubsystem.h"
#include "Subsystem/ViewModel/Fields/ViewModelFieldNames.h"
#include "Subsystem/ViewModel/MinimapViewModel.h"
#include "Subsystem/CharacterSubsystem.h"
#include "UI/InGameMenu/InGameMenuWidget.h"
#include "UI/InGameMenu/PlayerKilledWidget.h"
#include "UI/Minimap/MinimapWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/TestCharacter.h"
#include "Common/MyGameSettings.h"

ATestPlayerController::ATestPlayerController()
{
}

void ATestPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    SetGameOnlyInputMode();
}

void ATestPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem =	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("Player Controller Subsystem Available"));
		Subsystem->AddMappingContext(DefaultMappingContext, priority);
	}

    // 베이스 레이어 위젯 생성 (설정이 없으면 C++ 기본 클래스로 생성)
    // 인게임 메뉴 위젯 생성
    if (InGameMenuWidgetClass)
    {
       InGameMenuWidget =
            CreateWidget<UInGameMenuWidget>(this, InGameMenuWidgetClass);
        InGameMenuWidget->AddToViewport(10);
        InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    // 미니맵 위젯 생성
    if (!MinimapWidgetRef)
    {
        UMyGameSettings* GameSettings = UMyGameSettings::Get();
        if (GameSettings && GameSettings->MinimapWidget)
        {
            UE_LOG(LogTemp, Warning, TEXT("Minimap"));
            UClass* MinimapWidgetClass = GameSettings->MinimapWidget.LoadSynchronous();
            if (MinimapWidgetClass)
            {
                MinimapWidgetRef = CreateWidget<UMinimapWidget>(this, MinimapWidgetClass);
            }
            if (MinimapWidgetRef)
            {
                MinimapWidgetRef->AddToViewport(10);
                MinimapWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
            }
        }
    }

    if (MinimapWidgetRef)
    {
        MinimapWidgetRef->AddToViewport(10);
        MinimapWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
    }

    // 퍽 선택 화면 생성 및 베이스 레이어 등록
    // 인게임 메뉴 처리
    if (InGameMenuWidget)
    {
        InGameMenuWidget->OnInGameMenuClosed.AddDynamic(this, &ATestPlayerController::OnPauseInput);
    }

    // 미니맵 위치 업데이트는 컨트롤러에서 ViewModel 참조를 유지한다.
    if (MinimapWidgetRef)
    {
        if (UMVVMSubsystem* MVVMSubsystem = GetGameInstance()->GetSubsystem<UMVVMSubsystem>())
        {
            MinimapViewModel = MVVMSubsystem->GetMinimapViewModel();
            MinimapViewModel->OnFieldChanged.AddDynamic(this, &ATestPlayerController::HandleMinimapViewModelFieldChanged);
        }
    }

    // 플레이어 사망 델리게이트 바인딩
    ATestCharacter* TestCharacter = Cast<ATestCharacter>(GetPawn());
    if (TestCharacter)
    {
        TestCharacter->OnPlayerKilled.AddDynamic(this, &ATestPlayerController::OnAddPlayerKilledWidget);
    }

    // 플레이어 위치 업데이트 타이머 - Minimap
    GetWorldTimerManager().SetTimer(
        MinimapUpdateTimer,
        this,
        &ATestPlayerController::IsMinimapUpdateThresholdReached,
        0.1f,
        true
    );
}

void ATestPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(MinimapUpdateTimer);

    if (MinimapViewModel)
    {
        MinimapViewModel->OnFieldChanged.RemoveDynamic(this, &ATestPlayerController::HandleMinimapViewModelFieldChanged);
    }

    Super::EndPlay(EndPlayReason);
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

        if(IA_Minimap)
        {
            EnhancedInput->BindAction(IA_Minimap, ETriggerEvent::Started, this, &ATestPlayerController::OnMinimapInput);
        }
    }

}

void ATestPlayerController::OnPauseInput()
{
    if (bIsMenuOpen && InGameMenuWidget)
    {
        // 메뉴 닫기
        // 게임 재개
        SetPause(false);

        SetGameOnlyInputMode();

        InGameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

        bIsMenuOpen = false;
        InGameMenuWidget->InitializePauseMenu(); //메뉴 초기화
    }
    else
    {
        //메뉴 열기
        //게임 정지
        SetPause(true);

        // Ui에 입력 포커스 설정
        SetGameAndUIInputMode(InGameMenuWidget);

        InGameMenuWidget->SetVisibility(ESlateVisibility::Visible);

        bIsMenuOpen = true;
    }
}

void ATestPlayerController::OnMinimapInput()
{
    if(bIsMinimapOpen && MinimapWidgetRef)
    {
        //SetPause(false);
        // 미니맵 닫기
        SetGameOnlyInputMode();
        bShowMouseCursor = false;
        MinimapWidgetRef->SetVisibility(ESlateVisibility::Collapsed);
        bIsMinimapOpen = false;
    }
    else if(!bIsMinimapOpen && MinimapWidgetRef)
    {
        //SetPause(true);

        // 미니맵 열기
        SetGameAndUIInputMode(MinimapWidgetRef);
        bShowMouseCursor = false;
        MinimapWidgetRef->SetVisibility(ESlateVisibility::Visible);
        bIsMinimapOpen = true;
    }
}

void ATestPlayerController::OnAddPlayerKilledWidget()
{
    //UE_LOG(LogTemp, Log, TEXT("Player Killed Widget Added to Viewport"));
    if (PlayerKilledWidget)
    {
        SetGameAndUIInputMode(PlayerKilledWidget);
        PlayerKilledWidget->AddToViewport(100);
    }
}

// 델리게이트로 스테이지 클리어 후 뷰포트에 추가 !!!!!!!!!
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

// --- 플레이어 움직임이 미니맵 업데이트 임계값을 넘었는지 확인 ---
void ATestPlayerController::IsMinimapUpdateThresholdReached()
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    CurrentPawnLocation = ControlledPawn->GetActorLocation();
    CurrentPawnYaw = ControlledPawn->GetActorRotation().Yaw;

    const float DistanceMoved = FVector::DistSquared(CurrentPawnLocation, LastPawnLocation);
    const float YawDifference = FMath::Abs(CurrentPawnYaw - LastPawnYaw);

    //UE_LOG(LogTemp, Warning, TEXT("DistanceMoved: %f, YawDifference: %f"), DistanceMoved, YawDifference);s

    if (DistanceMoved < MinimapUpdateThreshold * MinimapUpdateThreshold &&
        YawDifference < MinimapYawUpdateThreshold)
    {
        return; // 임계값 이하로 이동/회전했으면 업데이트하지 않음
    }

    UpdateMinimapPlayerPosition();
}

// --- 미니맵 플레이어 위치 업데이트 ---
void ATestPlayerController::UpdateMinimapPlayerPosition()
{
    if (!MinimapViewModel)
    {
        if (UMVVMSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UMVVMSubsystem>())
        {
            MinimapViewModel = Subsystem->GetMinimapViewModel();
        }
    }

    if (MinimapViewModel)
    {
        MinimapViewModel->UpdatePlayerPosition(CurrentPawnLocation, CurrentPawnYaw);
        LastPawnLocation = CurrentPawnLocation;
        LastPawnYaw = CurrentPawnYaw;
    }
}

void ATestPlayerController::HandleMinimapViewModelFieldChanged(FName FieldName)
{
    if (FieldName == MinimapVMFields::IsInitialized)
    {
        UpdateMinimapPlayerPosition();
    }
}

void ATestPlayerController::SetUIOnlyInputMode()
{

}
