// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/GameStateSubsystem.h"
#include "Common/MyGameSettings.h"
#include "Data/LevelDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "Blueprint/UserWidget.h"
#include "ContentStreaming.h"
#include "ShaderPipelineCache.h"
#include "Engine/Engine.h"
#include "UObject/UObjectGlobals.h"

UGameStateSubsystem* UGameStateSubsystem::Get(const UObject* WorldContextObject)
{
    if (!WorldContextObject || !GEngine)
    {
        return nullptr;
    }

    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
    if (!World)
    {
        return nullptr;
    }

    UGameInstance* GameInstance = World->GetGameInstance();
    return GameInstance ? GameInstance->GetSubsystem<UGameStateSubsystem>() : nullptr;
}

void UGameStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (const UMyGameSettings* Settings = UMyGameSettings::Get())
    {
        if (!Settings->LevelDataAsset.IsNull())
        {
            LevelDataAsset = Settings->LevelDataAsset.LoadSynchronous();
            UE_LOG(LogTemp, Log, TEXT("GameStateSubsystem: LevelDataAsset loaded successfully."));
        }

        SetGameState(Settings->DefaultStartState);

        if (!Settings->LoadingWidgetClass.IsNull())
        {
            LoadingWidget = Settings->LoadingWidgetClass.LoadSynchronous();
        }
    }

	// 맵 로드 완료 시점을 알기 위해 PostLoadMapWithWorld 델리게이트에 바인딩
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameStateSubsystem::HandlePostLoadMap);
}

void UGameStateSubsystem::Deinitialize()
{
    ForceEndLoadingGate();

    FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

    Super::Deinitialize();
}

void UGameStateSubsystem::SetGameState(EGameState NewState)
{
    if (CurrentGameState == NewState) return;

    EGameState OldState = CurrentGameState;
    CurrentGameState = NewState;

    UE_LOG(LogTemp, Log, TEXT("Game state changed from %d to %d"), static_cast<int32>(OldState), static_cast<int32>(NewState));
    OnGameStateChanged.Broadcast(OldState, NewState);
}

void UGameStateSubsystem::TravelToState(EGameState TargetState, bool bCreateLoadingWidget)
{
    if (!LevelDataAsset)
    {
        return;
    }

    FLevelData LevelData;
    if (LevelDataAsset->GetLevelDataByState(TargetState, LevelData))
    {
        ExecuteLevelTravel(TargetState, LevelData, bCreateLoadingWidget);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TravelToState failed: state %d is not configured in LevelDataAsset."), static_cast<int32>(TargetState));
    }
}

void UGameStateSubsystem::BeginLoadingGate(bool bCreateLoadingWidget, bool bWaitForDungeonReady)
{
    if (!bCreateLoadingWidget)
    {
        ForceEndLoadingGate();
        return;
    }

    if (LoadingWidget.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("BeginLoadingGate skipped: LoadingWidgetClass is not set."));
        return;
    }

    if (!ActiveLoadingScreen)
    {
        ActiveLoadingScreen = CreateWidget<UUserWidget>(GetGameInstance(), LoadingWidget.Get());
        if (!ActiveLoadingScreen)
        {
            UE_LOG(LogTemp, Warning, TEXT("BeginLoadingGate failed: could not create loading widget."));
            return;
        }
        ActiveLoadingScreen->AddToViewport(1000);
    }

    bWaitingForPostLoadMap = false;
    bWaitingDungeonReadySignal = bWaitForDungeonReady;
    StableReadyFrameCount = 0;
    LoadingScreenShownAtSeconds = FPlatformTime::Seconds();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(LoadingGateTimerHandle);
        World->GetTimerManager().SetTimer(
            LoadingGateTimerHandle,
            this,
            &UGameStateSubsystem::TickLoadingGate,
            1.0f / 60.0f,
            true);
    }
}

void UGameStateSubsystem::NotifyDungeonReadyToWarmup()
{
    if (!ActiveLoadingScreen)
    {
        return;
    }

    bWaitingDungeonReadySignal = false;
}

void UGameStateSubsystem::ForceEndLoadingGate()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(LoadingGateTimerHandle);
    }

    if (ActiveLoadingScreen)
    {
        ActiveLoadingScreen->RemoveFromParent();
        ActiveLoadingScreen = nullptr;
    }

    bWaitingForPostLoadMap = false;
    bWaitingDungeonReadySignal = false;
    StableReadyFrameCount = 0;
}

void UGameStateSubsystem::ExecuteLevelTravel(EGameState TargetState, const FLevelData& LevelData, bool bCreateLoadingWidget)
{
    SetGameState(TargetState);

    if (bCreateLoadingWidget)
    {
        // 일반 레벨 이동은 던전 완료 신호를 강제하지 않고 스트리밍/PSO 안정화 기준으로 닫음
        BeginLoadingGate(true, false);
        bWaitingForPostLoadMap = true;
    }
    else
    {
        ForceEndLoadingGate();
    }

    if (LevelData.LevelAsset.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("LevelData for state %d has no valid level asset."), static_cast<int32>(TargetState));
        if (bCreateLoadingWidget)
        {
            // 로딩 위젯을 생성했지만 레벨 데이터가 유효하지 않은 경우, 로딩 게이트를 강제로 닫음
            ForceEndLoadingGate();
        }
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Level asset path: %s"), *LevelData.LevelAsset.ToSoftObjectPath().ToString());

    const FString LevelPath = LevelData.LevelAsset.GetAssetName();
    TSoftObjectPtr<UWorld> LevelToLoad = LevelData.LevelAsset;
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    Streamable.RequestAsyncLoad(
        LevelToLoad.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda([this, LevelPath]()
        {
            UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelPath));
        }));
}

void UGameStateSubsystem::HandlePostLoadMap(UWorld* LoadedWorld)
{
    if (!LoadedWorld || !ActiveLoadingScreen)
    {
        return;
    }

    bWaitingForPostLoadMap = false;
    StableReadyFrameCount = 0;
    LoadingScreenShownAtSeconds = FPlatformTime::Seconds();
    LoadedWorld->GetTimerManager().ClearTimer(LoadingGateTimerHandle);
    LoadedWorld->GetTimerManager().SetTimer(
        LoadingGateTimerHandle,
        this,
        &UGameStateSubsystem::TickLoadingGate,
        1.0f / 60.0f,
        true);
}

void UGameStateSubsystem::TickLoadingGate()
{
    if (!ActiveLoadingScreen)
    {
        ForceEndLoadingGate();
        return;
    }

    if (bWaitingForPostLoadMap)
    {
        return;
    }

    const double Now = FPlatformTime::Seconds();

    // 로딩 화면이 최소한으로 보여질 시간 동안은 던전 준비 신호와 스트리밍/PSO 상태에 관계없이 기다림
    if ((Now - LoadingScreenShownAtSeconds) < MinLoadingScreenSeconds)
    {
        return;
    }

    if (bWaitingDungeonReadySignal)
    {
        if ((Now - LoadingScreenShownAtSeconds) < DungeonReadyTimeoutSeconds)
        {
            return;
        }

        // 완료 신호가 오지 않아도 타임아웃 이후에는 진행
        bWaitingDungeonReadySignal = false;
    }

    const int32 RemainingStreamingRequests = IStreamingManager::Get().BlockTillAllRequestsFinished(0.0f, false); // 남아있는 스트리밍 요청 수를 가져옴
    const bool bStreamingDone = (RemainingStreamingRequests == 0); // 스트리밍이 완료되었는지 확인
    const bool bPSOPrecacheDone = (FShaderPipelineCache::NumPrecompilesRemaining() == 0); // PSO 프리캐시가 완료되었는지 확인

    if (bStreamingDone && bPSOPrecacheDone)
    {
        // 스트리밍과 PSO 프리캐시가 모두 완료된 경우 안정적으로 준비된 상태로 간주하기 위해 카운터 증가, 그렇지 않으면 카운터 초기화
        ++StableReadyFrameCount;
    }
    else
    {
        StableReadyFrameCount = 0; // 스트리밍 또는 PSO 프리캐시가 완료되지 않은 경우 카운터 초기화
    }

    if (StableReadyFrameCount >= RequiredStableReadyFrames)
    {
        // 스트리밍과 PSO 프리캐시가 모두 완료되고 안정적으로 준비된 상태로 간주하기 위해 필요한 연속 프레임 수를 충족한 경우 로딩 종료
        ForceEndLoadingGate();
    }
}
