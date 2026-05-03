// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "TestPlayerController.generated.h"

class UInputMappingContext;
class UPlayerKilledWidget;
class UMinimapWidget;
class UInGameMenuWidget;
class UMinimapSubsystem;
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

    // 스테이지와 챕터 정보를 받아서 원하는 스테이지와 챕터에서 퍽 선택 화면을 띄움
protected:
    // --- 빙의 시점에 강제로 Input을 GameModeOnly로 바꿈 ---
    virtual void OnPossess(APawn* InPawn) override;

	virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupInputComponent() override;

    // --- 플레이어 움직임이 미니맵 업데이트 임계값을 넘었는지 확인 ---
    UFUNCTION()
    void IsMinimapUpdateThresholdReached();

    // --- 미니맵 플레이어 위치 업데이트 ---
    UFUNCTION()
    void UpdateMinimapPlayerPosition();

    UFUNCTION()
    void HandleMinimapInitialized();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

    // --- 일시 정지 후 메뉴 처리 ---
    UFUNCTION()
    void OnPauseInput();

    // --- 일시 정지 후 미니맵 처리 ---
    UFUNCTION()
    void OnMinimapInput();

private:
    UFUNCTION()
    void OnAddPlayerKilledWidget();

protected:
    //IA
    // 일시 정지 입력
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
    TObjectPtr<UInputAction> IA_Pause = nullptr;

    // 미니맵 입력
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InputAction")
    TObjectPtr<UInputAction> IA_Minimap = nullptr;

private:
	int32 priority = 1;

    // ===============================
    // --- 메뉴 UI 관련 변수 --- 
    // ===============================
    bool bIsMenuOpen = false;

    UPROPERTY()
    TObjectPtr<UInGameMenuWidget> InGameMenuWidget = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UInGameMenuWidget> InGameMenuWidgetClass;

    // ===============================
    // --- 퍽 UI 관련 변수 ---
    // ===============================
    // ===============================
    // --- 사망 UI 위젯 관련 변수 ---
    // ===============================
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPlayerKilledWidget> PlayerKilledWidget;

    // ===============================
    // --- 미니맵 관련 변수 ---
    // ===============================
    bool bIsMinimapOpen = false;

    UPROPERTY(EditAnywhere, Category = "UI")
    TObjectPtr<UMinimapWidget> MinimapWidgetRef = nullptr;

    TObjectPtr<UMinimapSubsystem> MinimapSubsystem = nullptr;

    FVector CurrentPawnLocation = FVector::ZeroVector;
    float CurrentPawnYaw = 0.f;

    FVector LastPawnLocation = FVector::ZeroVector;
    float LastPawnYaw = 0.f;

    // --- 미니맵 거리 업데이트 임계값 ---
    UPROPERTY(EditDefaultsOnly, Category = "Minimap")
    float MinimapUpdateThreshold = 10.f;

    // --- 미니맵 회전 업데이트 임계값 ---
    UPROPERTY(EditDefaultsOnly, Category = "Minimap")
    float MinimapYawUpdateThreshold = 5.f;

    FTimerHandle MinimapUpdateTimer;
};
