// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Common/GameStateTypes.h"
#include "GameStateSubsystem.generated.h"

class ULevelDataAsset;
class UUserWidget;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChanged, EGameState, OldState, EGameState, NewState);
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UGameStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintPure, Category = "Game State", meta = (WorldContext = "WorldContextObject"))
    static UGameStateSubsystem* Get(const UObject* WorldContextObject);

	// =============================================================================
	// 초기화, 종료
	// =============================================================================
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// =============================================================================
	// 게임 상태 관리
	// =============================================================================
	UFUNCTION(BlueprintPure, Category = "Game State")
	EGameState GetCurrentGameState() const { return CurrentGameState; }

	UFUNCTION(BlueprintCallable, Category = "Game State")
	void SetGameState(EGameState NewState);

	// =============================================================================
	// 맵 이동
	// =============================================================================
	UFUNCTION(BlueprintCallable, Category = "Game State", meta = (CPP_Default_bCreateLoadingWidget = "true"))
	void TravelToState(EGameState TargetState, bool bCreateLoadingWidget = true);

	// =============================================================================
	// Setter / Getter
	// =============================================================================
	ULevelDataAsset* GetLevelDataAsset() const { return LevelDataAsset; }

    // 로딩 위젯을 생성하고 타이머를 시작하여 던전 준비 신호를 기다리는 로딩 게이트를 시작합니다.
    UFUNCTION(BlueprintCallable, Category = "Loading", meta = (CPP_Default_bCreateLoadingWidget = "true", CPP_Default_bWaitForDungeonReady = "true"))
    void BeginLoadingGate(bool bCreateLoadingWidget = true, bool bWaitForDungeonReady = true);

    UFUNCTION(BlueprintCallable, Category = "Loading")
    void NotifyDungeonReadyToWarmup();

    UFUNCTION(BlueprintCallable, Category = "Loading")
    void ForceEndLoadingGate();

    UFUNCTION(BlueprintPure, Category = "Loading")
    bool IsLoadingGateActive() const { return ActiveLoadingScreen != nullptr; }

	// =============================================================================
	// 델리게이트
	// =============================================================================
	FOnGameStateChanged OnGameStateChanged;

private:
    // 실제 레벨 이동 처리
	void ExecuteLevelTravel(EGameState TargetState, const FLevelData& LevelData, bool bCreateLoadingWidget);

    void HandlePostLoadMap(UWorld* LoadedWorld);
    void TickLoadingGate();

private:
	UPROPERTY()
	EGameState CurrentGameState = EGameState::None;

	UPROPERTY()
	TObjectPtr<ULevelDataAsset> LevelDataAsset;

    UPROPERTY()
    TSoftClassPtr<UUserWidget> LoadingWidget;

    UPROPERTY(EditDefaultsOnly, Category = "UI|Loading")
    float MinLoadingScreenSeconds = 1.0f; // 로딩 화면이 최소한으로 보여질 시간 (초)

    UPROPERTY(EditDefaultsOnly, Category = "UI|Loading")
    float DungeonReadyTimeoutSeconds = 8.0f; // 던전이 준비되었다는 신호를 기다리는 최대 시간 (초)

    UPROPERTY(EditDefaultsOnly, Category = "UI|Loading")
    int32 RequiredStableReadyFrames = 8;  // 스트리밍과 PSO 프리캐시가 완료된 후 안정적으로 준비된 상태로 간주하기 위해 필요한 연속 프레임 수

    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> ActiveLoadingScreen = nullptr; // 현재 활성화된 로딩 화면 위젯 인스턴스

    FTimerHandle LoadingGateTimerHandle;

    bool bWaitingForPostLoadMap = false; // 레벨 트래블 경로에서 PostLoadMap 이전에는 게이트를 닫지 않도록 잠금
    bool bWaitingDungeonReadySignal = false; // 던전 준비 신호를 기다리고 있는지 여부
    int32 StableReadyFrameCount = 0;  // 스트리밍과 PSO 프리캐시가 완료된 후 안정적으로 준비된 상태로 간주하기 위해 필요한 연속 프레임 수를 추적하는 카운터

    double LoadingScreenShownAtSeconds = 0.0; // 로딩 화면이 표시된 시각 (초)
};
