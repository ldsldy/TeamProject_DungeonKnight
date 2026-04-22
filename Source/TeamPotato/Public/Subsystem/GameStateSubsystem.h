// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Common/GameStateTypes.h"
#include "GameStateSubsystem.generated.h"

class ULevelDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameStateChanged, EGameState, OldState, EGameState, NewState);
/**
 * 
 */
UCLASS()
class TEAMPOTATO_API UGameStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
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
	UFUNCTION(BlueprintCallable, Category = "Game State")
	void TravelToState(EGameState TargetState);

	// =============================================================================
	// Setter / Getter
	// =============================================================================
	ULevelDataAsset* GetLevelDataAsset() const { return LevelDataAsset; }

	// =============================================================================
	// 델리게이트
	// =============================================================================
	FOnGameStateChanged OnGameStateChanged;

private:
	// 실제 레벨 이동 처리
	void ExecuteLevelTravel(EGameState TargetState, const FLevelData& LevelData);

private:
	UPROPERTY()
	EGameState CurrentGameState = EGameState::None;

	UPROPERTY()
	TObjectPtr<ULevelDataAsset> LevelDataAsset;

    UPROPERTY()
    TSoftClassPtr<UUserWidget> LoadingWidget;
};
